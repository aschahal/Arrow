#include "sparrow.h"
#include <fstream>
#include <sstream>

namespace sparrow {

// NullableInts implementations
std::ostream& operator<<(std::ostream& os, const NullableInts& obj) {
    os << "NullableInts: [";
    for (size_t i = 0; i < obj.nums.size(); ++i) {
        if (obj.valid[i / 32][i % 32]) {
            os << obj.nums[i];
        } else {
            os << "null";
        }
        if (i < obj.nums.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

void DropZero(NullableInts* nullableInts) {
    for (size_t i = 0; i < nullableInts->nums.size(); ++i) {
        if (nullableInts->nums[i] == 0) {
            nullableInts->valid[i / 32].reset(i % 32);
        }
    }
}

AverageResult Average(const NullableInts* nullableInts) {
    if (nullableInts == nullptr) {
        return {0, false};  // Handle null pointers safely
    }

    long sum = 0;
    size_t count = 0;
    for (size_t i = 0; i < nullableInts->nums.size(); ++i) {
        if (nullableInts->valid[i / 32][i % 32]) {
            sum += nullableInts->nums[i];
            ++count;
        }
    }
    if (count > 0) {
        return {static_cast<float>(sum) / count, true};
    }
    return {0, false};
}

AverageResult Average(const NullableInts& nullableInts) {
    return Average(&nullableInts);
}

DivideResult Divide(const NullableInts* nints1, const NullableInts* nints2) {
    auto [validBits, valid] = warmup::BitAnd(nints1->valid, nints2->valid);
    if (!valid || nints1->nums.size() != nints2->nums.size()) {
        return {{}, false};
    }

    NullableInts result;
    result.nums.resize(nints1->nums.size());
    result.valid = validBits;

    for (size_t i = 0; i < nints1->nums.size(); ++i) {
        if (validBits[i / 32][i % 32] && nints2->nums[i] != 0) {
            result.nums[i] = nints1->nums[i] / nints2->nums[i];
        } else {
            result.valid[i / 32].reset(i % 32);
        }
    }
    return {result, true};
}

NullableInts* StrsToNullableInts(const std::vector<std::string>& inputs) {
    auto* result = new NullableInts;
    result->nums.resize(inputs.size());
    result->valid.resize((inputs.size() + 31) / 32);

    for (size_t i = 0; i < inputs.size(); ++i) {
        if (inputs[i] == "null") {
            result->valid[i / 32].reset(i % 32);
        } else {
            result->nums[i] = std::stoi(inputs[i]);
            result->valid[i / 32].set(i % 32);
        }
    }
    return result;
}

int NullableIntsToArray(const NullableInts& nullableInts, int** array) {
    std::vector<int> validNums;
    for (size_t i = 0; i < nullableInts.nums.size(); ++i) {
        if (nullableInts.valid[i / 32][i % 32]) {
            validNums.push_back(nullableInts.nums[i]);
        }
    }

    *array = new int[validNums.size()];
    std::copy(validNums.begin(), validNums.end(), *array);
    return validNums.size();
}

// IntColumn implementations
IntColumn::IntColumn() : nums(), valid() {}

IntColumn::IntColumn(const std::vector<std::string>& inputs) {
    nums.resize(inputs.size());
    valid.resize((inputs.size() + 31) / 32);

    for (size_t i = 0; i < inputs.size(); ++i) {
        if (inputs[i] == "null") {
            valid[i / 32].reset(i % 32);
        } else {
            nums[i] = std::stoi(inputs[i]);
            valid[i / 32].set(i % 32);
        }
    }
}

void IntColumn::DropZero() {
    for (size_t i = 0; i < nums.size(); ++i) {
        if (nums[i] == 0) {
            valid[i / 32].reset(i % 32);
        }
    }
}

AverageResult IntColumn::Average() {
    long sum = 0;
    size_t count = 0;
    for (size_t i = 0; i < nums.size(); ++i) {
        if (valid[i / 32][i % 32]) {
            sum += nums[i];
            ++count;
        }
    }
    if (count > 0) {
        return {static_cast<float>(sum) / count, true};
    }
    return {0, false};
}

IntColumn IntColumn::operator/(const IntColumn& other) {
    IntColumn result;
    if (nums.size() != other.nums.size()) return result;

    result.nums.resize(nums.size());
    result.valid = warmup::BitAnd(valid, other.valid).first;

    for (size_t i = 0; i < nums.size(); ++i) {
        if (result.valid[i / 32][i % 32] && other.nums[i] != 0) {
            result.nums[i] = nums[i] / other.nums[i];
        } else {
            result.valid[i / 32].reset(i % 32);
        }
    }
    return result;
}

int IntColumn::Size() {
    return nums.size();
}

const int* IntColumn::operator[](int idx) {
    if (idx < 0) idx = nums.size() + idx;
    if (idx < 0 || idx >= static_cast<int>(nums.size()) || !valid[idx / 32][idx % 32]) {
        return nullptr;
    }
    return &nums[idx];
}

std::ostream& operator<<(std::ostream& os, const IntColumn& col) {
    os << "IntColumn:\n";
    for (size_t i = 0; i < col.nums.size(); ++i) {
        if (col.valid[i / 32][i % 32]) {
            os << col.nums[i] << "\n";
        } else {
            os << "null\n";
        }
    }
    return os;
}

// Constructor
NamedIntColumn::NamedIntColumn(const std::string& name, const std::vector<std::string>& inputs) 
    : name(name), col(new IntColumn(inputs)) {}

// Copy constructor
NamedIntColumn::NamedIntColumn(const NamedIntColumn& other) 
    : name(other.name), col(other.col ? new IntColumn(*other.col) : nullptr) {}

// Move constructor
NamedIntColumn::NamedIntColumn(NamedIntColumn&& other) 
    : name(std::move(other.name)), col(other.col) {
    other.col = nullptr;
}

// Copy assignment operator
void NamedIntColumn::operator=(const NamedIntColumn& other) {
    if (this == &other) return;
    
    delete col;
    name = other.name;
    col = other.col ? new IntColumn(*other.col) : nullptr;
}

// Move assignment operator
void NamedIntColumn::operator=(NamedIntColumn&& other) {
    if (this == &other) return;
    
    delete col;
    name = std::move(other.name);
    col = other.col;
    other.col = nullptr;
}

// Destructor
NamedIntColumn::~NamedIntColumn() {
    delete col;
}

Table::Table(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) return;

    size_t numRows, numCols;
    file >> numRows >> numCols;

    std::vector<std::vector<std::string>> colData(numCols, std::vector<std::string>(numRows));
    std::vector<std::string> colNames(numCols);

    for (size_t i = 0; i < numCols; ++i) {
        file >> colNames[i];
        for (size_t j = 0; j < numRows; ++j) {
            file >> colData[i][j];
        }
        AddCol(std::make_shared<NamedIntColumn>(colNames[i], colData[i]));
    }
}

void Table::AddCol(const NamedIntColumn& col) {
    columns.push_back(std::make_shared<NamedIntColumn>(col));
}

void Table::AddCol(std::shared_ptr<NamedIntColumn> col) {
    columns.push_back(col);
}

std::shared_ptr<NamedIntColumn> Table::operator[](const std::string& colName) {
    for (const auto& col : columns) {
        if (col->name == colName) {
            return col;
        }
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
    for (size_t i = 0; i < table.columns.size(); ++i) {
        os << table.columns[i]->name;
        if (i < table.columns.size() - 1) {
            os << ",";
        }
    }
    os << "\n";

    if (!table.columns.empty()) {
        size_t numRows = table.columns[0]->col->Size();
        for (size_t row = 0; row < numRows; ++row) {
            for (size_t col = 0; col < table.columns.size(); ++col) {
                const int* val = (*(table.columns[col]->col))[row];
                os << (val ? std::to_string(*val) : "null");
                if (col < table.columns.size() - 1) {
                    os << ",";
                }
            }
            os << "\n";
        }
    }
    return os;
}

// warmup namespace implementations
namespace warmup {

unsigned CountBits(std::bitset<32> bits, unsigned pos, unsigned len) {
    if (pos + len > 32) len = 32 - pos;

    unsigned count = 0;
    for (unsigned i = pos; i < pos + len; ++i) {
        if (bits.test(i)) ++count;
    }
    return count;
}

unsigned CountBits(const std::vector<std::bitset<32>>& bitsVector) {
    unsigned count = 0;
    for (const auto& bits : bitsVector) {
        count += bits.count();
    }
    return count;
}

std::pair<std::vector<std::bitset<32>>, bool> BitAnd(const std::vector<std::bitset<32>>& a, const std::vector<std::bitset<32>>& b) {
    std::vector<std::bitset<32>> result;
    bool valid = a.size() == b.size();

    if (valid) {
        for (size_t i = 0; i < a.size(); ++i) {
            result.push_back(a[i] & b[i]);
        }
    }

    return {result, valid};
}

} // namespace warmup

} // namespace sparrow
