#ifndef SPARROW_H
#define SPARROW_H

#include <bitset>
#include <vector>
#include <iostream>
#include <memory>

namespace sparrow {

struct NullableInts {
    std::vector<int> nums;
    std::vector<std::bitset<32>> valid;
};

struct AverageResult {
    float value;
    bool ok;
};

struct DivideResult {
    NullableInts value;
    bool ok;
};

// Function declarations
NullableInts* StrsToNullableInts(const std::vector<std::string>& inputs);
int NullableIntsToArray(const NullableInts& nullableInts, int** array);
std::ostream& operator<<(std::ostream& os, const NullableInts& obj);
void DropZero(NullableInts* nullableInts);
AverageResult Average(const NullableInts* nullableInts);
AverageResult Average(const NullableInts& nullableInts);
DivideResult Divide(const NullableInts* nints1, const NullableInts* nints2);

// IntColumn class declaration
class IntColumn {
private:
    std::vector<int> nums;
    std::vector<std::bitset<32>> valid;

public:
    IntColumn();
    IntColumn(const std::vector<std::string>& inputs);
    void DropZero();
    AverageResult Average();
    IntColumn operator/(const IntColumn& other);
    int Size();
    const int* operator[](int idx);

    friend std::ostream& operator<<(std::ostream& os, const IntColumn& col);
};

// NamedIntColumn class declaration
class NamedIntColumn {
public:
    std::string name;
    IntColumn* col;

    // Constructor
    NamedIntColumn(const std::string& name, const std::vector<std::string>& inputs);
    
    // Copy constructor
    NamedIntColumn(const NamedIntColumn& other);
    
    // Move constructor
    NamedIntColumn(NamedIntColumn&& other);
    
    // Copy assignment operator
    void operator=(const NamedIntColumn& other);
    
    // Move assignment operator
    void operator=(NamedIntColumn&& other);
    
    // Destructor
    ~NamedIntColumn();
};

class Table {
private:
    std::vector<std::shared_ptr<NamedIntColumn>> columns;

public:
    Table() = default;
    Table(const std::string& filePath);

    void AddCol(const NamedIntColumn& col);
    void AddCol(std::shared_ptr<NamedIntColumn> col);

    std::shared_ptr<NamedIntColumn> operator[](const std::string& colName);

    friend std::ostream& operator<<(std::ostream& os, const Table& table);
};


// warmup namespace
namespace warmup {
    unsigned CountBits(std::bitset<32> bits, unsigned pos, unsigned len);
    unsigned CountBits(const std::vector<std::bitset<32>>& bitsVector);
    std::pair<std::vector<std::bitset<32>>, bool> BitAnd(const std::vector<std::bitset<32>>& a, const std::vector<std::bitset<32>>& b);
}

} // namespace sparrow

#endif // SPARROW_H
