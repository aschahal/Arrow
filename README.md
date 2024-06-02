# Project Arrow

Project Arrow is a prototype that simplifies and optimizes the functionalities found in the Apache Arrow library. Apache Arrow is a cross-language development platform for in-memory data, designed to improve the speed and efficiency of data processing and communication. Arrow facilitates efficient data interchange between systems without the need for serialization or deserialization.

Project Arrow aims to emulate some of these functionalities but in a simplified manner. The project is structured into 4 stages:

**Tooling and Basics** (Part 1): For the first stage, created a shared library and a simple program that utilizes it. The focus is on compiling, building libraries, and managing simple data manipulations with bitsets and vectors. The program should do a bitwise and operation between two bitsets:

- 11100000000001100000000000100010
- 01110000000001111000000000100001 

& print how many 1's there are in the result. 

**Structs and Pointers** (Part 2): The second stage introduces a struct, **NullableInts**, to manage series of integers where some values may be missing, akin to NaN (not a number) in floating point operations. The struct pairs integers with bitsets to efficiently indicate valid data points. This should perform element-wise division of nints1 by nints2. For example: 

- NullableInts nints1{.nums={20,999,40,60}, .valid=vector{bitset<32>{"00000000000000000000000000001101"}}};
- NullableInts nints2{.nums={10,10,0,20}, .valid=vector{bitset<32>{"00000000000000000000000000001111"}}}; 
  
This computes the average of the resulting numbers, and prints it out. Drops any zero values from the denominator prior to doing the division.

**References and Conversions** (Part 3): The third stage enhances the previous functionalities by implementing reference handling, const correctness, and string-to-integer conversions to facilitate more complex data operations. It should accept command line arguments that are either ints or "null". Uses StrsToNullableInts to convert these args to a NullableInts, call Average on it, then print the result. 

For example, ./p3 2 3 should print 2.5. ./p3 2 null 3 would also print 2.5 since Average ignores missing values. Something like ./p3 hello may crash.

**Object Oriented Programming** (Part 4): The fourth stage involves refactoring previous code to use classes, focusing on encapsulation, inheritance, and polymorphism through an **IntColumn** class that manages operations previously handled by NullableInts. For example, if you run *./p4 4 null 2 0 -3*, the output should be:

- IntColumn:
  4
  null
  2
  null
  -3
  Avg: 1

There are two substages to Part 4:

- **Copying and Moving** (4a): Stage 4a focuses on implementing copying and moving semantics for classes, particularly with a NamedIntColumn class that includes a name and a pointer to an IntColumn.

- **Table** (4b):  Stage 4b involves creating a Table class that acts as a collection of NamedIntColumn objects, supporting file I/O and shared pointers for efficient memory management and operations like adding and dividing columns. Suppose tbl1.txt describes two columns like this:

3
2
A
10
20
30
B
10
5
15
We ought to be able to read it in and perform some computations to produce additional columns, like this:

sparrow::Table t{"tbl1.txt"};
t.AddCol(*t["A"] / *t["B"]);
t.AddCol(*t["A"] / 10);
cout << t;
The result would look like this:

A,B,A/B,A/10
10,10,1,1
20,5,4,2
30,15,2,3

Project Arrow allows efficient handling of data columns and operations like addition, subtraction, and conditional operations without explicit loops or data serialization, mimicking the functionalities of the Apache Arrow project in a learning-focused environment.
