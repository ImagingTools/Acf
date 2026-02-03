# IqtComponentTest Build Setup Guide

## Overview

The IqtComponentTest is a Qt Test-based unit test for the Iqt component of the ACF framework. It has been configured to support both CMake and QMake build systems.

## Test Files

- **CIqtCompTest.h**: Test class definition
- **CIqtCompTest.cpp**: Test implementation (7373 bytes)
- **IqtComponentTest.acc**: ACF component configuration file (requires Arxc processing)
- **main.cpp**: Test entry point
- **CMake/CMakeLists.txt**: CMake build configuration
- **QMake/IqtComponentTest.pro**: QMake project file

## CMake Build Requirements

### 1. Environment Setup

Set the ACFDIR environment variable to point to the ACF framework root:

```bash
export ACFDIR=/path/to/Acf  # Replace with your ACF installation directory
```

### 2. Required Dependencies

The following components must be available in the build environment:

- **Qt Framework**: Qt 5.x or Qt 6.x
  - Qt Core library
  - Qt Test library
  - Qt GUI components (for AcfStdGui)
  - qmake and moc (meta-object compiler)

- **ACF Framework Libraries** (must be built first):
  - libiqt - Iqt component library
  - libifile - File handling library
  - libiprm - Parameter management library
  - libitest - Test support library

- **Build Tools**:
  - CMake 3.26 or later
  - C++ compiler (GCC, Clang, or MSVC)
  - Arxc - ACF code generator (for processing .acc files)

### 3. CMake Configuration

The CMakeLists.txt has been updated to properly include the AcfEnv.cmake configuration:

```bash
cd Tests/IqtComponentTest/CMake
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DACFDIR=$ACFDIR ..
```

### 4. Building

```bash
cmake --build . --config Release
```

Or with make:

```bash
make
```

### 5. Running the Tests

```bash
# From the build directory
./IqtComponentTest

# Or with test output
./IqtComponentTest -v2
```

## QMake Build (Alternative)

If using QMake instead of CMake:

```bash
cd Tests/IqtComponentTest/QMake
qmake IqtComponentTest.pro
make
./IqtComponentTest
```

## CMakeLists.txt Configuration Details

The CMakeLists.txt includes:

1. **cmake_minimum_required**: Sets minimum CMake version to 3.26
2. **ACFDIR Detection**: Reads ACFDIR from environment or CMake variable
3. **AcfEnv.cmake**: Initializes ACF environment configuration
4. **CustomBuild.cmake**: Processes .acc files with Arxc compiler
5. **ApplicationConfig.cmake**: Standard ACF application configuration
6. **Qt Integration**: Configures Qt components and libraries
7. **Component Linking**: Links against required ACF libraries

## Build Directory Structure

```
Tests/IqtComponentTest/
├── CMake/
│   ├── CMakeLists.txt
│   └── build/              # Created during CMake configuration
├── QMake/
│   └── IqtComponentTest.pro
├── CIqtCompTest.h
├── CIqtCompTest.cpp
├── main.cpp
├── IqtComponentTest.acc
├── IqtComponentTest.accl
└── BUILD_SETUP.md          # This file
```

## Troubleshooting

### CMake Configuration Errors

1. **"ACFDIR not defined"**: Ensure `export ACFDIR=/path/to/Acf` is set before running cmake
2. **"Qt not found"**: Set `CMAKE_PREFIX_PATH` to Qt installation directory:
   ```bash
   cmake -DCMAKE_PREFIX_PATH=/path/to/Qt ..
   ```
3. **"Arxc not found"**: Ensure Arxc compiler is in PATH or set in ACFDIR_BUILD

### Missing Libraries

If the build fails due to missing ACF libraries:

1. Ensure all ACF framework components are built first
2. Check that library paths are correctly configured in the Lib directory
3. Verify CMAKE_LIBRARY_PATH includes the ACF Lib directory

### Qt Version Issues

The build system auto-detects Qt version. To force a specific version:

```bash
cmake -DQT_VERSION_MAJOR=6 ..  # For Qt 6
cmake -DQT_VERSION_MAJOR=5 ..  # For Qt 5
```

## Test Execution

The IqtComponentTest uses Qt Test framework and supports:

- Verbose output: `-v2`
- XML output: `-xml -o results.xml`
- Run specific test: `-TestName`

Example:
```bash
./IqtComponentTest -v2 -xml -o test_results.xml
```

## Integration with CI/CD

For TeamCity or other CI systems, ensure:

1. ACFDIR environment variable is set in CI configuration
2. Qt framework is available in CI environment
3. CMAKE_PREFIX_PATH or QTDIR is configured for Qt discovery
4. Test results are captured (use -xml option for structured output)

## Additional Resources

The following resources provide additional information about the build components and tools used:

- [ACF Documentation](https://imagingtools.github.io/Acf/) - Framework documentation
- [Qt Test Framework](https://doc.qt.io/qt-6/qttest-index.html) - Qt Test framework reference (Qt 6)
- [CMake Documentation](https://cmake.org/documentation/) - CMake official documentation

Note: For Qt 5, replace qt-6 with qt-5 in the Qt documentation URL.

## Notes

- The test uses Qt5 or Qt6 depending on system configuration
- The .acc file is processed by the Arxc compiler during build
- All Qt Test features are available for test development
- The test framework supports test suites and test data

