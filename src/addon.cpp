#include <napi.h>

// Recursive Fibonacci function (CPU-heavy)
long long Fibonacci(int n) {
    if (n <= 1) return n;
    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

// Wrapper function for Node.js
Napi::Number FibonacciWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int n = info[0].As<Napi::Number>().Int32Value();
    return Napi::Number::New(env, Fibonacci(n));
}

// Export function to Node.js
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("fibonacci", Napi::Function::New(env, FibonacciWrapped));
    return exports;
}

NODE_API_MODULE(addon, Init)
