#include "sparrow.h"
#include <iostream>
#include <string>
#include <vector>

using namespace sparrow;

int main(int argc, char** argv) {
    std::vector<std::string> inputs(argc - 1);
    for (int i = 1; i < argc; ++i) {
        inputs[i - 1] = argv[i];
    }

    NullableInts* nullableInts = StrsToNullableInts(inputs);
    auto avgResult = Average(nullableInts);
    if (avgResult.ok) {
        std::cout << avgResult.value << std::endl;
    } else {
        std::cout << "failed" << std::endl;
    }

    delete nullableInts;
    return 0;
}
