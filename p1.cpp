#include "sparrow.h"
#include <iostream>
#include <bitset>

int main() {
    std::vector<std::bitset<32>> a = {std::bitset<32>("11100000000001100000000000100010")};
    std::vector<std::bitset<32>> b = {std::bitset<32>("01110000000001111000000000100001")};

    auto [result, valid] = sparrow::warmup::BitAnd(a, b);
    if (valid) {
        std::cout << sparrow::warmup::CountBits(result) << std::endl;
    }

    return 0;
}

