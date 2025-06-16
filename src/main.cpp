#include <iostream>

#include "complex.h"

#include <chrono>

int main() {
    std::cout << "Hello, world!\n";

    using timer = std::chrono::high_resolution_clock;
    auto start = timer::now();

    complexMain(0, nullptr);

    auto end = timer::now();
    auto duration = end - start;

        std::cout << "Total time: "
              << std::chrono::duration<double, std::milli>(duration).count()
              << " ms\n";
    return 0;
}