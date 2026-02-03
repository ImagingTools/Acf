# SerializationRegressionTest

## Purpose

Cross-library component tests for regression of data model serialization. This test suite validates that data models from different ACF libraries can be correctly serialized and deserialized, ensuring data integrity across save/restore cycles.

## Test Coverage

This test validates serialization functionality for data models from the following libraries:

### i2d Library (2D Graphics)
- **CVector2d** - 2D vector serialization
- **CPosition2d** - 2D position serialization
- **CCircle** - Circle with center and radius
- **CRectangle** - Rectangle with bounds
- **CLine2d** - Line with begin and end points

### i3d Library (3D Graphics)
- **CVector3d** - 3D vector serialization
- **CBox3d** - 3D bounding box with min/max points
- **CSphere** - Sphere with center and radius
- **CPlane3d** - Plane with normal and distance

### icmm Library (Color Management)
- **CVarColor** - Variable-length color with multiple components
- **CSpectrumInfo** - Spectral information with range and step

### imath Library (Mathematics)
- **CVarVector** - Variable-length vector

### iprm Library (Parameters)
- **CIdParam** - ID parameter with byte array
- **CNameParam** - Name parameter with QString
- **CTextParam** - Text parameter with QString
- **CEnableableParam** - Enableable parameter with boolean state
- **CSelectionParam** - Selection parameter with options

### ifile Library (File Management)
- **CFileNameParam** - File name parameter with path

### iimg Library (Image Processing)
- **CScanlineMask** - Scanline mask for 2D regions

## Test Structure

Each test:
1. Creates an original object with specific test values
2. Serializes the object to a memory buffer using `CMemoryWriteArchive`
3. Deserializes from the buffer into a new object using `CMemoryReadArchive`
4. Verifies that all data was correctly preserved

## Building and Running

### CMake Build

```bash
cd Tests/SerializationRegressionTest/CMake
mkdir build && cd build
cmake ..
make
./SerializationRegressionTest
```

### QMake Build

```bash
cd Tests/SerializationRegressionTest/QMake
qmake
make
./SerializationRegressionTest
```

## Test Implementation

The test uses a template method `TestSerializationCycle<T>()` that:
- Takes an original object and an empty restored object
- Performs the serialization/deserialization cycle
- Returns true if successful, false otherwise

Each specific test then verifies that the restored object matches the original object by comparing individual fields.

## Expected Results

All tests should pass, indicating that:
- Data models correctly implement the `ISerializable::Serialize()` method
- Serialization preserves all object data
- Deserialization correctly reconstructs objects from archived data
- The serialization system works consistently across different libraries

## Regression Testing

This test suite serves as a regression test to ensure that:
- Future changes don't break serialization compatibility
- Data models maintain their serialization contracts
- Cross-library serialization remains functional
