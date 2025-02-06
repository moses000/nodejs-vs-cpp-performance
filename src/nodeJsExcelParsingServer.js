const express = require('express');
const { Worker } = require('worker_threads');
const path = require('path');

const app = express();
const port = 3000;

// Middleware to parse multipart form data (Excel upload)
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// Endpoint to handle file upload
app.post('/upload', (req, res) => {
    const excelFilePath = req.body.filePath;  // Assume this is passed from the front end

    // Immediate feedback to the user
    res.json({ message: "File is being processed. You will receive a notification once it's done." });

    // Offload the task to a Worker Thread
    processExcelFileInBackground(excelFilePath);
});

// Function to offload the work to a worker thread
function processExcelFileInBackground(excelFilePath) {
    const worker = new Worker(path.resolve(__dirname, 'worker.js'), {
        workerData: { filePath: excelFilePath },
    });

    // Listen for the result from the worker
    worker.on('message', (result) => {
        // After processing is done, send completion notification
        notifyUser(result);
    });

    worker.on('error', (err) => {
        console.error('Worker thread error:', err);
    });

    worker.on('exit', (code) => {
        if (code !== 0) {
            console.error(`Worker stopped with exit code ${code}`);
        }
    });
}

// Function to send a notification (could be email, push, WebSocket, etc.)
function notifyUser(result) {
    console.log('Processing completed:', result);
    // Here, you can implement an email notification or WebSocket push to notify the user
    // Example: sendEmailNotification(result);
}

app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});
