const { parentPort, workerData } = require('worker_threads');
const fs = require('fs');
const path = require('path');
const { processExcelData } = require('./your-cpp-addon');  // Your C++-binded function for Excel processing

// Worker will process the Excel file
async function processFile() {
    try {
        // Perform your Excel processing, image upload, and MongoDB operations here
        console.log('Processing file:', workerData.filePath);

        // Call the C++ function (assuming you have your C++ addon properly bound)
        let result = await processExcelData(workerData.filePath);
        
        // After processing, notify main thread about success
        parentPort.postMessage({ success: true, data: result });
    } catch (error) {
        console.error('Error processing file:', error);
        parentPort.postMessage({ success: false, message: error.message });
    }
}

// Start the worker task
processFile();
