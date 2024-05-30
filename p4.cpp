#include "sparrow.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::vector<std::string> inputs(argv + 1, argv + argc);
    sparrow::IntColumn col(inputs);

    col.DropZero();
    std::cout << col;

    auto avgResult = col.Average();
    if (avgResult.ok) {
        std::cout << "Avg: " << avgResult.value << "\n";
    } else {
        std::cout << "Avg: failed\n";
    }

    return 0;
}
