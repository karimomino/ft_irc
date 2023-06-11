#include <iostream>

// Example functions with different argument types and return types
void Function1() {
    std::cout << "Function 1 called." << std::endl;
}

void Function2(int arg) {
    std::cout << "Function 2 called with arg: " << arg << std::endl;
}

int Function3(double arg1, int arg2) {
    std::cout << "Function 3 called with arg1: " << arg1 << ", arg2: " << arg2 << std::endl;
    return arg2;
}

// Wrapper function for function with no arguments
void CallFunction0(void* funPtr) {
    void (*functionPtr)() = reinterpret_cast<void (*)()>(funPtr);
    functionPtr();
}

// Wrapper function for function with one argument
void CallFunction1(void* funPtr, int arg) {
    void (*functionPtr)(int) = reinterpret_cast<void (*)(int)>(funPtr);
    functionPtr(arg);
}

// Wrapper function for function with two arguments
int CallFunction2(void* funPtr, double arg1, int arg2) {
    int (*functionPtr)(double, int) = reinterpret_cast<int (*)(double, int)>(funPtr);
    return functionPtr(arg1, arg2);
}

int main() {
    // Create an array of void* pointers to store the function pointers
    void* fun[3];

    // Assign function pointers to the void* array
    fun[0] = reinterpret_cast<void*>(&Function1);
    fun[1] = reinterpret_cast<void*>(&Function2);
    fun[2] = reinterpret_cast<void*>(&Function3);

    // Invoke the functions using the void* pointers and wrapper functions
    CallFunction0(fun[0]);
    CallFunction1(fun[1], 42);
    int result = CallFunction2(fun[2], 3.14, 42);
    std::cout << "Function 3 returned: " << result << std::endl;

    return 0;
}
