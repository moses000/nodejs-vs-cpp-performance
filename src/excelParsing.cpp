#include <napi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <sstream>
#include <curl/curl.h>
#include "xlnt/xlnt.hpp"

// Custom structure to hold feedback
struct Feedback {
    bool success;
    std::string message;
};

// Error handling function to throw exceptions
void checkFileValidity(const std::string& filePath, Feedback& feedback) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        feedback.success = false;
        feedback.message = "File could not be opened: " + filePath;
        throw std::runtime_error(feedback.message);
    }
    feedback.success = true;
}

// Function to upload the image to the server
std::string uploadImageToServer(const std::string& imagePath, const std::string& serverUrl, Feedback& feedback) {
    CURL* curl;
    CURLcode res;
    std::string serverResponse;

    try {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (!curl) {
            feedback.success = false;
            feedback.message = "Failed to initialize CURL.";
            throw std::runtime_error(feedback.message);
        }

        std::ifstream imageFile(imagePath, std::ios::binary);
        if (!imageFile.is_open()) {
            feedback.success = false;
            feedback.message = "Failed to open image file: " + imagePath;
            throw std::runtime_error(feedback.message);
        }

        std::stringstream imageStream;
        imageStream << imageFile.rdbuf();
        std::string imageData = imageStream.str();

        struct curl_httppost* formPost = NULL;
        struct curl_httppost* lastPtr = NULL;

        // Add image data as form-data
        curl_formadd(&formPost,
                     &lastPtr,
                     CURLFORM_COPYNAME, "file",
                     CURLFORM_BUFFER, "image.png",
                     CURLFORM_BUFFERPTR, imageData.c_str(),
                     CURLFORM_BUFFERLENGTH, imageData.length(),
                     CURLFORM_CONTENTTYPE, "application/octet-stream",
                     CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_URL, serverUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formPost);

        // Perform the HTTP POST request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            feedback.success = false;
            feedback.message = "Image upload failed: " + std::string(curl_easy_strerror(res));
            throw std::runtime_error(feedback.message);
        }

        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code == 200) {
            serverResponse = "Image uploaded successfully.";
        } else {
            feedback.success = false;
            feedback.message = "Failed to upload image, server responded with code: " + std::to_string(response_code);
            throw std::runtime_error(feedback.message);
        }

        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error during image upload: " << ex.what() << std::endl;
        feedback.success = false;
        feedback.message = ex.what();
        return "Upload failed";
    }

    feedback.success = true;
    return serverResponse;
}

// Function to process Excel file and extract product data
std::vector<std::map<std::string, std::string>> processExcelData(const std::string& excelFilePath, Feedback& feedback) {
    std::vector<std::map<std::string, std::string>> productData;

    try {
        // Check if the file is valid
        checkFileValidity(excelFilePath, feedback);
        if (!feedback.success) {
            return productData;
        }

        xlnt::workbook wb;
        wb.load(excelFilePath);
        xlnt::worksheet ws = wb.active_sheet();

        for (auto row : ws.rows()) {
            if (row[0].value<std::string>().empty()) continue;  // Skip empty rows

            std::map<std::string, std::string> product;
            product["productID"] = row[0].value<std::string>();   // Product ID
            product["description"] = row[1].value<std::string>(); // Product Description
            product["price"] = row[2].value<std::string>();       // Product Price

            // Extract the image path and upload the image
            std::string imagePath = row[3].value<std::string>();  // Image file path in Excel
            if (imagePath.empty()) {
                product["imageURL"] = "No image provided";
            } else {
                std::string serverUrl = "https://your-image-upload-server.com/upload";
                std::string uploadResponse = uploadImageToServer(imagePath, serverUrl, feedback);
                product["imageURL"] = uploadResponse;  // Store upload response (can be a URL or message)
            }

            productData.push_back(product);
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error processing Excel data: " << ex.what() << std::endl;
        feedback.success = false;
        feedback.message = ex.what();
    }

    return productData;
}

// Wrapper function for Node.js
Napi::Object processExcelWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Object result = Napi::Object::New(env);

    // Validate and extract the Excel file path from arguments
    std::string excelFilePath = info[0].As<Napi::String>().Utf8Value();
    Feedback feedback;

    std::vector<std::map<std::string, std::string>> resultData = processExcelData(excelFilePath, feedback);

    if (feedback.success) {
        Napi::Array resultArray = Napi::Array::New(env, resultData.size());
        for (size_t i = 0; i < resultData.size(); ++i) {
            Napi::Object product = Napi::Object::New(env);
            product.Set("productID", resultData[i]["productID"]);
            product.Set("description", resultData[i]["description"]);
            product.Set("price", resultData[i]["price"]);
            product.Set("imageURL", resultData[i]["imageURL"]);

            resultArray[i] = product;
        }
        result.Set("success", Napi::Boolean::New(env, true));
        result.Set("data", resultArray);
    } else {
        result.Set("success", Napi::Boolean::New(env, false));
        result.Set("message", Napi::String::New(env, feedback.message));
    }

    return result;
}

// Export function to Node.js
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("processExcel", Napi::Function::New(env, processExcelWrapped));
    return exports;
}

NODE_API_MODULE(addon, Init)
