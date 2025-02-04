const addon = require('../build/Release/addon.node');

// Pure JavaScript Fibonacci
function fibonacci(n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// JavaScript Performance Test
console.time("JS Fibonacci");
console.log(fibonacci(40));
console.timeEnd("JS Fibonacci");

// C++ Addon Performance Test
console.time("C++ Fibonacci");
console.log(addon.fibonacci(40));
console.timeEnd("C++ Fibonacci");
