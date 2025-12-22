# ACF Unit Tests

This directory contains unit tests for the ACF (Application Component Framework) library.

## Test Structure

The ACF project uses Qt Test framework with a custom test executor (`itest::CStandardTestExecutor`). Each test is structured as follows:

### Test Organization

- **Tests/** - Root directory for all tests
  - **ParamsManagerTest/** - Tests for IParamsManager interface
  - **MultiThreadingComponentTest/** - Tests for multi-threading components
  - **SelectionParamIntegrationTest/** - Integration tests for selection parameters
  - **UtilityClassesTest/** - Tests for utility classes across multiple modules

### Test File Structure

Each test directory contains:
- `main.cpp` - Entry point that runs all registered tests
- `C*Test.h` - Test class header files
- `C*Test.cpp` - Test class implementation files
- `CMake/CMakeLists.txt` - CMake build configuration
- Optional: `.acc` and `.accl` files for component configuration

### Writing Tests

1. **Create test class** inheriting from `QObject`:
```cpp
class CMyClassTest: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void myTest();
    void cleanupTestCase();
};
```

2. **Implement test methods** using QVERIFY/QCOMPARE macros:
```cpp
void CMyClassTest::myTest()
{
    MyClass obj;
    QVERIFY(obj.doSomething());
    QCOMPARE(obj.getValue(), 42);
}
```

3. **Register test** at the end of the .cpp file:
```cpp
I_ADD_TEST(CMyClassTest);
```

## UtilityClassesTest

This test suite covers fundamental utility classes from multiple ACF modules:

### Tested Classes

#### istd (Standard Utilities)
- **CRandomNumber** - Random number generation for integers and doubles
- **CIndex2d** - 2D indexing with Qt type conversions
- **CBitManip** - Bit manipulation operations
- **CCrcCalculator** - CRC-32 checksum calculations

#### imath (Mathematics)
- **CDouble** - Double wrapper with rounding and comparison operations
- **CLinearInterpolator** - Linear interpolation with optional extrapolation

#### i2d (2D Graphics)
- **CVector2d** - 2D vector operations including angles, orthogonals, and transformations

#### i3d (3D Graphics)
- **CVector3d** - 3D vector operations including cross products and normalization
- **CMatrix3d** - 3D matrix operations including transformations and determinants

#### icmm (Color Management)
- **CHsv** - HSV color model with component-wise operations

#### iser (Serialization)
- **CMemoryArchive** - Binary serialization to/from memory buffers

## Building and Running Tests

### Prerequisites
- Qt 5 or Qt 6
- CMake
- C++ compiler with C++11 support
- ACF library built and installed

### Build Instructions

Using CMake:
```bash
cd Tests/UtilityClassesTest/CMake
mkdir build && cd build
cmake ..
make
```

### Running Tests

Execute the test binary:
```bash
./UtilityClassesTest
```

Run specific test:
```bash
./UtilityClassesTest CDoubleTest
```

## Test Coverage

The UtilityClassesTest suite provides comprehensive coverage for:
- Constructor variations
- Getter/setter methods
- Arithmetic operations
- Comparison operations
- Type conversions
- Edge cases and boundary conditions
- Serialization (where applicable)

## Contributing

When adding new tests:
1. Follow the existing naming conventions (C*Test for class tests)
2. Include `initTestCase()` and `cleanupTestCase()` methods
3. Use descriptive test method names ending in "Test"
4. Add your test files to CMakeLists.txt
5. Register your test using `I_ADD_TEST` macro
6. Document the tested class in this README

## Notes

- Tests are independent and can run in any order
- Each test class should clean up after itself in `cleanupTestCase()`
- Use Qt Test macros: QVERIFY, QCOMPARE, QVERIFY2, etc.
- Floating point comparisons should use tolerance checks (qAbs(a - b) < epsilon)
