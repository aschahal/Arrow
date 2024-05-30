#include "sparrow.h"
#include <iostream>

int main() {
    using namespace sparrow;
    NullableInts nints1{{20,999,40,60}, {std::bitset<32>{"00000000000000000000000000001101"}}};
    NullableInts nints2{{10,10,0,20}, {std::bitset<32>{"00000000000000000000000000001111"}}};

    DropZero(&nints2); // Drop zeros from denominator

    auto divResult = Divide(&nints1, &nints2);
    if (divResult.ok) {
        auto avgResult = Average(&divResult.value);
        if (avgResult.ok) {
            std::cout << avgResult.value << std::endl;
        } else {
            std::cout << "No valid numbers to compute average." << std::endl;
        }
    } else {
        std::cout << "Division operation failed." << std::endl;
    }

    return 0;
}
