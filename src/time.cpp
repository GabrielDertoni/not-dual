#include <chrono>

#include "includes/time.hpp"

Timestamp getNow() {
    return std::chrono::high_resolution_clock::now();
}

