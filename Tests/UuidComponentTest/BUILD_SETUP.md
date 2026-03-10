# UuidComponentTest Build Setup

This test project verifies the functionality of CUuidComp component from the ibase library.

## Purpose

Tests the UUID component which provides automatic creation of Universally Unique Identifiers (UUIDs).

## Dependencies

- ibase - Base library containing CUuidComp
- iprm - Parameter management for INameParam interface
- iser - Serialization support
- itest - Test framework
- Qt Core and Test modules

## Building

### CMake
```bash
cd Tests/UuidComponentTest/CMake
cmake .
make
```

### QMake
```bash
cd Tests/UuidComponentTest/QMake
qmake
make
```

## Running Tests

After building, run the test executable:
```bash
./UuidComponentTest
```

## Test Coverage

The test suite covers:
- UUID component creation
- UUID generation
- UUID uniqueness verification
- UUID format validation (RFC 4122)
- UUID serialization and deserialization
