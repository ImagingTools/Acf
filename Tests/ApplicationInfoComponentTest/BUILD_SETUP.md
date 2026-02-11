# ApplicationInfoComponentTest Build Setup

This test project verifies the functionality of CApplicationInfoComp component from the ibase library.

## Purpose

Tests the ApplicationInfo component which provides application metadata such as company name, product name, application name, version info, and copyright information.

## Dependencies

- ibase - Base library containing CApplicationInfoComp
- iser - Serialization and version info support
- itest - Test framework
- Qt Core and Test modules

## Building

### CMake
```bash
cd Tests/ApplicationInfoComponentTest/CMake
cmake .
make
```

### QMake
```bash
cd Tests/ApplicationInfoComponentTest/QMake
qmake
make
```

## Running Tests

After building, run the test executable:
```bash
./ApplicationInfoComponentTest
```

## Test Coverage

The test suite covers:
- ApplicationInfo component creation
- Company name retrieval
- Product name and edition
- Application name, ID, and type
- Legal copyright information
- Main version ID
- Version info integration
- Custom attribute values
- Default values verification
