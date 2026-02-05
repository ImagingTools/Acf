# ACF Unit Tests

This directory contains unit tests for the ACF (Application Component Framework) library.

## Test Structure

The ACF project uses Qt Test framework with a custom test executor (`itest::CStandardTestExecutor`). Each test is structured as follows:

### Test Organization

Tests are organized by location:

- **Tests/** - Integration and component tests
  - **ParamsManagerTest/** - Tests for IParamsManager interface
  - **MultiThreadingComponentTest/** - Tests for multi-threading components
  - **SelectionParamIntegrationTest/** - Integration tests for selection parameters
  - **SelectionParamComponentTest/** - Component tests for CSelectionParamComp with various configurations
  - **AutoPersistenceTest/** - Tests for CAutoPersistenceComp with various attribute configurations
  - **IdocComponentTest/** - Component tests for idoc library (CTextDocumentComp, CSerializedUndoManagerComp, CSingleDocumentTemplateComp)
  - **FileComponentTest/** - Component tests for ifile library (CFileNameParamComp, CFileTypeInfoComp, CTempFileManagerComp, CSystemLocationComp)
  - **LogTest/** - Component tests for ilog library logging components
  - **IqtComponentTest/** - Component tests for iqt library (CApplicationSettingsProviderComp, CIniSettingsProviderComp, CClipboardSerializerComp, CSettingsSerializerComp, CTranslationManagerComp)

- **Include/[library]/Test/** - Unit tests for each library
  - **Include/istd/Test/** - Tests for standard utilities (CRandomNumber, CIndex2d, CBitManip, CCrcCalculator, etc.)
  - **Include/imath/Test/** - Tests for mathematics library (CDouble, CLinearInterpolator, etc.)
  - **Include/i2d/Test/** - Tests for 2D graphics library (CVector2d, etc.)
  - **Include/i3d/Test/** - Tests for 3D graphics library (CVector3d, CMatrix3d, etc.)
  - **Include/icmm/Test/** - Tests for color management library (CHsv, etc.)
  - **Include/iser/Test/** - Tests for serialization library (CMemoryArchive, etc.)

### Test File Structure

Each test directory contains:
- `main.cpp` - Entry point that runs all registered tests
- `C*Test.h` - Test class header files
- `C*Test.cpp` - Test class implementation files
- `CMake/CMakeLists.txt` - CMake build configuration
- Optional: `.acc` and `.accl` files for component configuration (for component tests)

### Writing Tests

1. **Create test main.cpp** using the standard test main macro:
```cpp
#include <itest/TestMain.h>

I_TEST_MAIN()
```

2. **Create test class** inheriting from `QObject`:
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

3. **Implement test methods** using QVERIFY/QCOMPARE macros:
```cpp
void CMyClassTest::myTest()
{
    MyClass obj;
    QVERIFY(obj.doSomething());
    QCOMPARE(obj.getValue(), 42);
}
```

4. **Register test** at the end of the .cpp file:
```cpp
I_ADD_TEST(CMyClassTest);
```

## Library Unit Tests

Unit tests for each library are located in `Include/[library]/Test/` directories.

### Tested Classes by Library

#### istd (Standard Utilities) - Include/istd/Test/
- **CRandomNumber** - Random number generation for integers and doubles
- **CIndex2d** - 2D indexing with Qt type conversions
- **CBitManip** - Bit manipulation operations
- **CCrcCalculator** - CRC-32 checksum calculations
- **CInterfacePtr** - Smart pointer for interface management
- **CRange** - Range operations for double values (min/max, intersection, union, translation, etc.)
- **CRanges** - Set of ranges with set operations (union, intersection, morphological operations)
- **CSystem** - System utilities including OS detection, file/directory operations, path manipulation, environment variables, and compiler information

#### imath (Mathematics) - Include/imath/Test/
- **CDouble** - Double wrapper with rounding and comparison operations
- **CLinearInterpolator** - Linear interpolation with optional extrapolation

#### i2d (2D Graphics) - Include/i2d/Test/
- **CVector2d** - 2D vector operations including angles, orthogonals, and transformations
- **CI2d** - Comprehensive 2D geometry tests (Vector2d, Circle, Rectangle, Line, Matrix, etc.)

#### i3d (3D Graphics) - Include/i3d/Test/
- **CVector3d** - 3D vector operations including cross products and normalization
- **CMatrix3d** - 3D matrix operations including transformations and determinants

#### icmm (Color Management) - Include/icmm/Test/
- **CHsv** - HSV color model with component-wise operations

#### iimg (Imaging) - Include/iimg/Test/
- **CBitmap** - Bitmap operations including creation, format support, pixel access, image manipulation (23 tests)
- **CBitmapBase** - Base bitmap functionality including geometry, color access, region operations, serialization (12 tests)
- **CScanlineMask** - Scanline mask operations for 2D regions including geometry creation, boolean operations, morphological operations (16 tests)
- **CPixelFormatList** - Pixel format enumeration and management with options list support (9 tests)
- **TPixelConversion** - Template-based pixel type conversions between grayscale and float formats (9 tests)

#### iser (Serialization) - Include/iser/Test/
- **CMemoryArchive** - Binary serialization to/from memory buffers
- **CBitMemoryArchive** - Bit-level serialization
- **CCompactXmlArchive** - Compact XML serialization
- **CJsonArchive** - JSON serialization

#### ibase (Base Components) - Include/ibase/Test/
- **CCumulatedProgressManagerBase** - Progress management with subtask support
- **TContainer** - Template container class with Qt List backend, supporting push/pop, insert/remove operations, iterators, and standard container interface
- **TSerializableContainer** - Template container with serialization support for items
- **TFactorisableContainer** - Template container with factory-based element creation and automatic memory management

#### iprm (Parameters) - Include/iprm/Test/
- **CIdParam** - ID parameter with get/set, serialization, copy/clone, and comparison operations
- **CNameParam** - Name parameter with get/set, serialization, copy/clone, and comparison operations
- **CTextParam** - Text parameter with get/set, serialization, copy/clone, and comparison operations
- **CEnableableParam** - Enableable parameter with enable/disable state management
- **CSelectionParam** - Selection parameter for managing option selections
- **IOptionsList** - Helper function FindOptionIndexById for finding options by ID

#### ifile (File Handling) - Include/ifile/Test/
- **CFileNameParam** - File/directory path parameter with get/set, path type detection, serialization, copy/clone, and comparison operations (NEW)
- **CFileArchive** - File-based binary serialization including primitive types, strings, tag skipping, and multiple objects (NEW)
- **CFileSecureArchive** - Secure file archives with encoding/decoding, integrity verification, and comparison with normal archives (NEW)
- **CFileArchiveInfo** - File archive information with path handling (existing tests)
- **CCompactXmlFileArchive** - Compact XML file serialization (existing tests)
- **CCompressedXmlFileArchive** - Compressed XML file serialization (existing tests)
- **CJsonFileArchive** - JSON file serialization (existing tests)
- **CSimpleXmlFileArchive** - Simple XML file serialization (existing tests)
- **CSimpleEncoder** - Simple encoding/decoding operations (existing tests)
#### icomp (Component Framework) - Include/icomp/Test/
- **CComponentSimulationTest** - Component framework core functionality tests (15 tests)
  - Component creation and initialization
  - Component lifecycle (OnComponentCreated/OnComponentDestroyed callbacks)
  - Parent-child relationships and ownership
  - Component active state management
  - Interface querying and registration
  - Component context operations
  - Single and multi-attribute management
  - Different attribute types (string, int, bool, double)
  - Component references (single and multiple)
  - Component factory operations
- **CRegistryTest** - Registry management and operations (11 tests)
  - Registry creation and element management
  - Element insertion, retrieval, removal, and renaming
  - Embedded registry operations (insertion, retrieval, removal)
  - Exported interface management
  - Registry serialization and deserialization
  - Thread-safe concurrent access

#### ipackage (Package Management) - Include/ipackage/Test/
- **CPackageTest** - Package loading and management tests (4 tests)
  - Component accessor creation and operations
  - Component accessor behavior without packages
  - Registries manager component creation
  - Loading with empty configuration

## Building and Running Tests

### Prerequisites
- Qt 5 or Qt 6
- CMake
- C++ compiler with C++11 support
- ACF library built and installed

### Build Instructions

Tests can be built individually for each library.

Using CMake for a specific library:
```bash
# For istd tests
cd Include/istd/Test/CMake
mkdir build && cd build
cmake ..
make

# For icomp tests (component framework)
cd Include/icomp/Test/CMake
mkdir build && cd build
cmake ..
make

# For ipackage tests (package management)
cd Include/ipackage/Test/CMake
mkdir build && cd build
cmake ..
make

# For imath tests
cd Include/imath/Test/CMake
mkdir build && cd build
cmake ..
make

# Similarly for other libraries: i2d, i3d, icmm, iser, iprm
```

### Running Tests

Execute the test binary for each library:
```bash
./istdTest
./icompTest
./ipackageTest
./imathTest
./i2dTest
./i3dTest
./iprmTest
```

Run specific test:
```bash
./istdTest CRandomNumberTest
```

## Test Coverage

Each library's test suite provides comprehensive coverage for:
- Constructor variations
- Getter/setter methods
- Arithmetic operations
- Comparison operations
- Type conversions
- Edge cases and boundary conditions
- Serialization (where applicable)

Tests are organized by library to maintain clear separation of concerns and enable independent testing of each component.

## Component Test Details

### SelectionParamComponentTest

**Purpose**: Component-level tests for `iprm::CSelectionParamComp` class with various configurations.

**Test Coverage**:
- **Selection Parameter Without Constraints**:
  - Component creation and initialization
  - Default index attribute handling
  - Serialization/deserialization
  - Get/Set operations
  - Copy and clone operations
  
- **Selection Parameter With Constraints**:
  - Component creation with constraints reference
  - Default index validation against constraints
  - Handling of invalid default index (should be reset to -1 if >= options count)
  - Serialization/deserialization with constraints
  - Copy and clone operations preserving constraints
  
- **Selection Parameter With Subselection**:
  - Subselection reference configuration
  - GetSubselection() method behavior
  - Serialization with subselection

**Configuration Files**:
- `SelectionParamComponentTest.acc` - Contains 8 different component configurations:
  1. `SelectionNoConstraints` - Basic selection parameter without constraints
  2. `SelectionWithDefaultIndex` - Selection with default index (5)
  3. `OptionsConstraints` - Options manager for constraints
  4. `SelectionWithConstraints` - Selection with constraints reference
  5. `SelectionWithConstraintsAndDefault` - Selection with constraints and default index (2)
  6. `SelectionWithInvalidDefault` - Selection with invalid default index (100, should be reset to -1)
  7. `Subselection` - Subselection parameter with constraints and default index (1)
  8. `SelectionWithSubselection` - Selection with subselection reference and default index (3)

**Test Methods**:
- `testCreationNoConstraints()` - Verifies component creation without constraints
- `testDefaultIndexNoConstraints()` - Tests default index attribute handling
- `testSerializationNoConstraints()` - Tests serialization/deserialization cycle
- `testCopyNoConstraints()` - Tests copying to another selection parameter
- `testCloneNoConstraints()` - Tests cloning the component
- `testCreationWithConstraints()` - Verifies component creation with constraints
- `testDefaultIndexWithConstraints()` - Tests default index with constraint validation
- `testInvalidDefaultIndexWithConstraints()` - Tests handling of invalid default index
- `testSerializationWithConstraints()` - Tests serialization with constraints
- `testCopyWithConstraints()` - Tests copying with constraints preservation
- `testCloneWithConstraints()` - Tests cloning with constraints preservation
- `testCreationWithSubselection()` - Verifies component creation with subselection
- `testGetSubselection()` - Tests GetSubselection() method behavior
- `testSerializationWithSubselection()` - Tests serialization with subselection


## Component Tests

### LogTest

**Purpose**: Component-level tests for the `ilog` library logging components with various configurations.

**Test Coverage**:
- **ConsoleLog Component**:
  - Component creation and initialization
  - Message handling for different severity levels (Info, Warning, Error, Critical)
  - Message formatting configuration (category, code, timestamp)
  
- **LogComp Component**:
  - Component creation with message storage
  - Message addition and retrieval via IMessageContainer interface
  - MaxMessageCount attribute handling and automatic message pruning
  - Worst category tracking
  - Serialization support
  
- **LogRouter Component**:
  - Component creation for message routing
  - Message filtering by severity level (MinimalCategory)
  - Routing messages from source container to destination consumer
  - Verification that only messages meeting severity threshold are routed
  
- **TracingConfiguration Component**:
  - Component creation and initialization
  - Tracing level get/set operations
  - Default tracing level configuration
  
- **StreamLog Component**:
  - Stream-based logging with filtering
  - MinCategory attribute for message filtering
  - Message formatting with timestamps and categories

**Configuration File**:
- `LogTest.acc` - Contains 6 different logging component configurations:
  1. `ConsoleLog` - Basic console logger with category and code display
  2. `LogComp` - Log component with message storage (max 100 messages) and slave consumer
  3. `ErrorLog` - Log component for error storage (max 50 messages)
  4. `LogRouter` - Router component to forward errors (MinimalCategory=3) from LogComp to ErrorLog
  5. `TracingConfig` - Tracing configuration with default level 1
  6. `StreamLog` - Console log with filtering (MinCategory=2), timestamps, and dot display

**Test Methods**:
- `testConsoleLogCreation()` - Verifies ConsoleLog component creation
- `testConsoleLogMessageHandling()` - Tests message support and handling for different categories
- `testLogCompCreation()` - Verifies LogComp component creation and interfaces
- `testLogCompMessageStorage()` - Tests message storage, retrieval, and worst category tracking
- `testLogCompMaxMessageCount()` - Tests MaxMessageCount limit and message pruning
- `testLogRouterCreation()` - Verifies LogRouter component creation
- `testLogRouterMessageRouting()` - Tests message routing based on severity filtering
- `testTracingConfigCreation()` - Verifies TracingConfiguration component creation
- `testTracingConfigLevel()` - Tests tracing level operations
- `testStreamLogFiltering()` - Tests stream log message filtering
- `testMessageSerialization()` - Tests message container serialization

### AutoPersistenceTest

The AutoPersistenceTest validates the CAutoPersistenceComp component with multiple configuration variants using a single comprehensive Partitura file (AutoPersistenceTest.acc). The file defines six AutoPersistence component instances, each testing different combinations of component attributes:

1. **AutoPersistenceRestoreStoreEnd** - Tests RestoreOnBegin and StoreOnEnd attributes
   - Validates that objects are restored when component initializes
   - Validates that objects are saved when component is destroyed
   
2. **AutoPersistenceStoreOnChange** - Tests StoreOnChange attribute
   - Validates immediate persistence on every data change
   
3. **AutoPersistenceStoreOnBegin** - Tests StoreOnBegin attribute
   - Validates that objects are saved on initialization if file doesn't exist
   
4. **AutoPersistenceStoreInterval** - Tests StoreInterval attribute
   - Validates timer-based automatic persistence
   
5. **AutoPersistenceAutoReload** - Tests AutoReload attribute
   - Validates file watching and automatic reload on external changes
   
6. **AutoPersistenceLocking** - Tests lock file handling
   - Validates StaleLockTime, TryLockTimeout, and EnableLockForRead attributes

Each test scenario uses a dedicated SelectionParam object as the persistent data and verifies:
- Proper initialization and cleanup
- Correct file creation and deletion
- Data integrity across save/restore cycles
- Timing behavior for interval-based persistence

### IdocComponentTest

**Purpose**: Component-level tests for the `idoc` library document framework components with various configurations.

**Test Coverage**:
- **CTextDocumentComp**:
  - Component creation and initialization
  - Get/Set text operations
  - Default text attribute handling
  - Serialization/deserialization of document content
  
- **CSerializedUndoManagerComp**:
  - Component creation and initialization
  - Undo/redo functionality with single and multiple steps
  - Integration with document model via observer pattern
  - MaxBufferSize attribute configuration
  - Reset undo history
  - Document state comparison and storage
  
- **CSingleDocumentTemplateComp**:
  - Component creation with document type configuration
  - Document creation via template
  - Document type attributes (ID, name, IsNewSupported, IsEditSupported)
  - Integration with document factory and file loader

**Configuration File**:
- `IdocComponentTest.acc` - Contains 7 different component configurations:
  1. `TextDocument` - Basic text document without custom settings
  2. `TextDocumentWithDefault` - Text document with custom default text
  3. `UndoManager` - Undo manager with default buffer size (100 MB)
  4. `UndoManagerSmallBuffer` - Undo manager with small buffer (1 MB) for testing
  5. `TemplateDocumentFactory` - Document factory for template with default text
  6. `TemplateFileLoader` - File loader component for template
  7. `DocumentTemplate` - Complete document template with all attributes

**Test Methods**:
- `testTextDocumentCreation()` - Verifies text document component creation
- `testTextDocumentGetSet()` - Tests get/set text operations
- `testTextDocumentDefaultText()` - Tests default text attribute
- `testTextDocumentSerialization()` - Tests document content serialization
- `testUndoManagerCreation()` - Verifies undo manager component creation
- `testUndoManagerUndoRedo()` - Tests basic undo/redo functionality
- `testUndoManagerMultipleSteps()` - Tests multiple undo/redo steps
- `testUndoManagerMaxBufferSize()` - Tests buffer size configuration
- `testUndoManagerReset()` - Tests undo history reset
- `testUndoManagerStateComparison()` - Tests document state comparison
- `testDocumentTemplateCreation()` - Verifies document template creation
- `testDocumentTemplateCreateDocument()` - Tests document creation via template
- `testDocumentTemplateAttributes()` - Tests template attributes

### FileComponentTest

**Purpose**: Component-level tests for the `ifile` library file handling components with various configurations.

**Test Coverage**:
- **CFileNameParamComp**:
  - Component creation and initialization
  - Path type configuration (file, directory, URL, unknown)
  - Default path attribute handling with variable expansion ($(TempPath), $(AppName), etc.)
  - Get/Set path operations
  - Serialization/deserialization of file paths
  
- **CFileTypeInfoComp**:
  - Component creation with extension lists
  - File extension retrieval and validation
  - Type descriptions for each extension
  - Platform-specific extension handling (Windows, Unix, Mac)
  - GetFileExtensions() method with filtering
  
- **CTempFileManagerComp**:
  - Component creation with root folder configuration
  - Session management (BeginSession/FinishSession)
  - File item addition and removal
  - Path retrieval for session files
  - Multiple concurrent sessions
  - Session isolation verification
  
- **CSystemLocationComp**:
  - Component creation for system paths
  - Application-specific path resolution
  - User-specific path handling

- **CFileListProviderComp**:
  - Component creation with directory parameter
  - File list retrieval with filters
  - Static methods for recursive file/directory enumeration
  - Depth control and filter application

- **CComposedFilePersistenceComp**:
  - Component creation as composite loader
  - File extension aggregation from slave loaders
  - Type description with common description attribute
  - Delegation to multiple persistence handlers

**Configuration File**:
- `FileComponentTest.acc` - Contains 10 different file component configurations:
  1. `FileNameParam` - Basic file name parameter with default path
  2. `FileNameParamFile` - File name parameter with file path type
  3. `FileNameParamDir` - File name parameter with directory path type
  4. `FileNameParamVars` - File name parameter with path variable expansion
  5. `FileTypeInfo` - File type info with basic extensions (txt, log, md)
  6. `FileTypeInfoPlatform` - File type info with platform-specific extensions
  7. `TempFileManager` - Temp file manager with root folder configuration
  8. `SystemLocation` - System location with application data path
  9. `FileListProvider` - File list provider with filters and recursion depth
  10. `ComposedPersistence` - Composed file persistence with multiple format support

**Test Methods**:
- `testFileNameParamCreation()` - Verifies FileNameParam component creation
- `testFileNameParamPathType()` - Tests path type configuration
- `testFileNameParamDefaultPath()` - Tests default path attribute
- `testFileNameParamGetSet()` - Tests get/set path operations
- `testFileNameParamSerialization()` - Tests path serialization
- `testFileTypeInfoCreation()` - Verifies FileTypeInfo component creation
- `testFileTypeInfoExtensions()` - Tests file extension retrieval
- `testFileTypeInfoDescriptions()` - Tests type descriptions
- `testFileTypeInfoPlatformSpecific()` - Tests platform-specific extensions
- `testTempFileManagerCreation()` - Verifies TempFileManager component creation
- `testTempFileManagerSession()` - Tests session lifecycle
- `testTempFileManagerAddFile()` - Tests file addition to session
- `testTempFileManagerRemoveFile()` - Tests file removal from session
- `testTempFileManagerMultipleSessions()` - Tests multiple concurrent sessions
- `testSystemLocationCreation()` - Verifies SystemLocation component creation
- `testSystemLocationPath()` - Tests system path retrieval
- `testFileListProviderCreation()` - Verifies FileListProvider component creation
- `testFileListProviderGetFileList()` - Tests file list retrieval with filters
- `testFileListProviderStaticMethods()` - Tests static recursive enumeration methods
- `testComposedPersistenceCreation()` - Verifies ComposedPersistence component creation
- `testComposedPersistenceExtensions()` - Tests extension aggregation from slaves
- `testComposedPersistenceDescription()` - Tests common description attribute
- `testTempFileManagerCreation()` - Verifies TempFileManager component creation
- `testTempFileManagerSession()` - Tests session lifecycle
- `testTempFileManagerAddFile()` - Tests file addition to session
- `testTempFileManagerRemoveFile()` - Tests file removal from session
- `testTempFileManagerMultipleSessions()` - Tests multiple concurrent sessions
- `testSystemLocationCreation()` - Verifies SystemLocation component creation
- `testSystemLocationPath()` - Tests system path retrieval

### IqtComponentTest

**Purpose**: Component-level tests for the `iqt` library Qt integration components with various configurations.

**Test Coverage**:
- **CApplicationSettingsProviderComp**:
  - Component creation and initialization
  - Settings object retrieval via ISettingsProvider interface
  - Organization and application name verification
  - Read/write operations to application settings
  
- **CIniSettingsProviderComp**:
  - Component creation and initialization
  - Settings object retrieval with INI format
  - Read/write operations to INI file settings
  - INI file format verification
  
- **CClipboardSerializerComp**:
  - Component creation and initialization
  - IFilePersistence interface implementation
  - Operation support verification
  - File extension handling
  
- **CSettingsSerializerComp**:
  - Component creation and initialization
  - IFilePersistence interface implementation
  - Operation support for serializable objects
  - Settings-based persistence verification
  
- **CTranslationManagerComp**:
  - Component creation and initialization
  - Language list retrieval and verification
  - Language switching functionality
  - Current language index tracking
  - Language IDs and names verification

**Configuration File**:
- `IqtComponentTest.acc` - Contains 8 different component configurations:
  1. `ApplicationInfo` - Application information component with name and organization
  2. `ApplicationSettingsProvider` - Provider for application-specific settings
  3. `IniSettingsProvider` - Provider for INI file-based settings
  4. `VersionInfo` - Version information component for serializers
  5. `ClipboardSerializer` - Clipboard-based serialization component
  6. `TestDataObject` - Selection parameter for testing serialization
  7. `SettingsSerializer` - QSettings-based serialization component
  8. `TranslationManager` - Translation manager with 3 languages (English, German, French)

**Test Methods**:
- `testApplicationSettingsProviderCreation()` - Verifies application settings provider creation
- `testApplicationSettingsProviderGetSettings()` - Tests settings object retrieval and configuration
- `testApplicationSettingsProviderReadWrite()` - Tests read/write operations
- `testIniSettingsProviderCreation()` - Verifies INI settings provider creation
- `testIniSettingsProviderGetSettings()` - Tests INI format settings object
- `testIniSettingsProviderReadWrite()` - Tests INI file read/write operations
- `testClipboardSerializerCreation()` - Verifies clipboard serializer creation
- `testClipboardSerializerInterfaces()` - Tests IFilePersistence interface implementation
- `testClipboardSerializerFileExtensions()` - Tests file extension handling
- `testSettingsSerializerCreation()` - Verifies settings serializer creation
- `testSettingsSerializerInterfaces()` - Tests IFilePersistence interface implementation
- `testSettingsSerializerFileExtensions()` - Tests file extension handling
- `testTranslationManagerCreation()` - Verifies translation manager creation
- `testTranslationManagerLanguagesInfo()` - Tests language list retrieval
- `testTranslationManagerSwitchLanguage()` - Tests language switching functionality
- `testTranslationManagerCurrentLanguage()` - Tests current language tracking

## Contributing

When adding new tests:
1. Place tests in the appropriate library's Test directory: `Include/[library]/Test/`
2. Follow the existing naming conventions (C*Test for class tests)
3. Create `main.cpp` using the `I_TEST_MAIN()` macro from `<itest/TestMain.h>`
4. Include `initTestCase()` and `cleanupTestCase()` methods
5. Use descriptive test method names ending in "Test"
6. Update the library's CMakeLists.txt if needed
7. Register your test using `I_ADD_TEST` macro
8. Update this README with the new test information

## Notes

- Tests are independent and can run in any order
- Each test class should clean up after itself in `cleanupTestCase()`
- Use Qt Test macros: QVERIFY, QCOMPARE, QVERIFY2, etc.
- Floating point comparisons should use tolerance checks (qAbs(a - b) < epsilon)
