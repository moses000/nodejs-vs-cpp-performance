# 🚀 Node.js vs C++ Performance Benchmark  
## **Can Node.js Be as Fast as C++?**  

This project explores the performance differences between **pure JavaScript** and **C++ Addons (N-API)** for **CPU-intensive** computations. By leveraging C++ modules in Node.js, we can execute tasks at **native speed**, making Node.js significantly faster for compute-heavy operations.

---

## **🛠 Features**  
✔️ **C++ Addon using Node.js N-API** for enhanced performance  
✔️ **Performance benchmarking** between JavaScript and C++  
✔️ **Fibonacci sequence** computation as a test case  
✔️ Ideal for **AI, cryptography, real-time applications**, and **data processing**  

---

## **📌 Installation & Setup**  

### **1️⃣ Clone the Repository**  
```sh
git clone https://github.com/moses000/nodejs-vs-cpp-performance.git
cd nodejs-vs-cpp-performance
```

### **2️⃣ Install Dependencies**  
```sh
npm install
npm install -g node-gyp
```

### **3️⃣ Build the C++ Addon**  
```sh
node-gyp configure
node-gyp build
```

### **4️⃣ Run Benchmark Tests**  
```sh
node src/index.js
```

---

## **🚀 Benchmarking Results**  

For `fibonacci(40)`, the execution times are:  

| Implementation  | Execution Time  |
|----------------|----------------|
| JavaScript (Node.js) | **20-30 seconds** |
| C++ Addon (Node.js) | **~0.02 seconds** |

✅ **C++ Addon executes ~1000x faster than pure JavaScript!** 🚀  

---

## **📜 License**  
This project is licensed under the **MIT License**. You are free to use, modify, and distribute the code under the license terms.  

---

## **📩 Contributing**  
We welcome contributions to improve this project!  
🔹 **Report Issues** – Open an issue for bugs, suggestions, or discussions  
🔹 **Pull Requests** – Submit improvements, optimizations, or new benchmarks  

---

## **🔗 Connect with Me**  
- 🔥 GitHub: (https://github.com/moses000)
<<<<<<< HEAD
📝 Medium: medium.com/@ 
=======
- 📝 Medium: (https://imoleayomoses.medium.com/) 
>>>>>>> e40f14de3b64cfde9fc5c216718d91cd76957971
- 🌎 Blog: (https://imoleayomoses.com/blog)  

⭐ **If you found this project useful, please star this repository!** 🚀🔥  
