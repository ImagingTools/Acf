# ImodTest - Comprehensive Test Suite for imod Library

## Overview

This test suite provides comprehensive coverage for the **imod** (Model/Observer) library in the ACF framework. The tests validate the implementation of the Model/Observer design pattern and all related classes.

## Test Architecture

The test suite consists of 7 test classes with a total of **78 test methods**, covering all major functionality of the imod library.

### Test Classes

#### 1. CModelBaseTest (14 test methods)
Tests the basic model implementation with observer management.

**Tested Functionality:**
- Observer attachment/detachment (`testAttachObserver`, `testDetachObserver`)
- Observer counting (`testGetObserverCount`, `testGetObservers`)
- Attachment verification (`testIsAttached`)
- Detaching all observers (`testDetachAllObservers`)
- Change notifications before/after (`testNotifyBeforeChange`, `testNotifyAfterChange`)
- Change set accumulation (`testChangeSetAccumulation`)
- Multiple observers notification (`testMultipleObserversNotification`)

**Edge Cases:**
- Attaching same observer twice (`testAttachSameObserverTwice`)
- Detaching unattached observer (`testDetachUnattachedObserver`)
- Notifications with no observers (`testNotifyWithNoObservers`)

#### 2. CSingleModelObserverBaseTest (10 test methods)
Tests single model observer functionality.

**Tested Functionality:**
- Model attachment/detachment (`testModelAttachment`, `testModelDetachment`)
- Attachment verification (`testIsModelAttached`)
- Model retrieval (`testGetObservedModel`)
- Ensuring model detachment (`testEnsureModelDetached`)
- Update notifications (`testOnUpdate`)
- Before/after update callbacks (`testBeforeAndAfterUpdate`)
- Change set filtering (`testChangeSetFiltering`, `testSetObservedIds`)

**Edge Cases:**
- Multiple attach/detach cycles (`testMultipleAttachDetachCycles`)
- Detaching null model (`testDetachNullModel`)

#### 3. CMultiModelObserverBaseTest (11 test methods)
Tests multi-model observer functionality.

**Tested Functionality:**
- Multiple model attachment (`testAttachMultipleModels`)
- Model detachment (`testDetachModel`)
- Ensuring all models detached (`testEnsureModelsDetached`)
- Model counting (`testGetModelCount`)
- Model retrieval by index (`testGetObservedModel`)
- Attachment verification (`testIsModelAttached`)
- Updates from multiple models (`testOnUpdateFromMultipleModels`)
- Change set filtering (`testChangeSetFiltering`, `testSetObservedIds`)

**Edge Cases:**
- Attaching same model multiple times (`testAttachSameModelMultipleTimes`)
- Detaching non-attached model (`testDetachNonAttachedModel`)
- Updates after partial detach (`testUpdateAfterPartialDetach`)

#### 4. TModelWrapTest (11 test methods)
Tests the template wrapper that converts IChangeable objects into observable models.

**Tested Functionality:**
- Model wrap creation (`testModelWrapCreation`)
- Setting base object (`testSetBaseObject`)
- Supported operations (`testGetSupportedOperations`)
- Cloning (`testCloneMe`)
- Observer attachment (`testObserverAttachment`)
- Change notification (`testChangeNotification`)
- Begin/end changes (`testBeginEndChanges`)
- Begin/end change groups (`testBeginEndChangeGroup`)
- Model data access (`testModelDataAccess`)
- Copying (`testCopyFrom`)
- Data reset (`testResetData`)

#### 5. CModelProxyTest (11 test methods)
Tests the model proxy pattern for switching model connections.

**Tested Functionality:**
- Proxy creation (`testProxyCreation`)
- Setting model pointer (`testSetModelPtr`)
- Resetting model (`testResetModel`)
- Observer attachment (`testObserverAttachment`)
- Model switching (`testModelSwitching`)
- Notification forwarding (`testNotificationForwarding`)
- Observer persistence across switches (`testObserverPersistenceAcrossSwitches`)
- Detaching observer from proxy (`testDetachObserverFromProxy`)

**Edge Cases:**
- Switching to null model (`testSwitchToNullModel`)
- Multiple observers (`testMultipleObservers`)
- Reset with observers (`testResetWithObservers`)

#### 6. CModelUpdateBridgeTest (10 test methods)
Tests bridging model changes to IChangeable objects.

**Tested Functionality:**
- Bridge creation with flags (`testBridgeCreation`)
- Model attachment to bridge (`testAttachModelToBridge`)
- Model detachment (`testDetachModelFromBridge`)
- Ensuring models detached (`testEnsureModelsDetached`)
- Model counting (`testGetModelCount`)
- Model retrieval (`testGetObservedModel`)
- Delegated update flag (`testDelegatedUpdateFlag`)
- Source update flag (`testSourceUpdateFlag`)
- Multiple models update (`testMultipleModelsUpdate`)

**Edge Cases:**
- Bridge with null changeable (`testBridgeWithNullChangeable`)
- Multiple attach/detach cycles (`testMultipleAttachDetachCycles`)

#### 7. CMultiModelDispatcherBaseTest (10 test methods)
Tests the multi-model dispatcher for change notification dispatch.

**Tested Functionality:**
- Model registration (`testRegisterModel`)
- Model unregistration (`testUnregisterModel`)
- Unregister all models (`testUnregisterAllModels`)
- Object retrieval by ID (`testGetObjectAt`)
- Model changed notifications (`testOnModelChanged`)
- Multiple model notifications (`testMultipleModelNotifications`)
- Relevant flags filtering (`testRelevantFlagsFiltering`)
- Model ID dispatching (`testModelIdDispatching`)

**Edge Cases:**
- Registering same ID twice (`testRegisterSameIdTwice`)
- Unregistering non-existent model (`testUnregisterNonExistentModel`)
- Empty dispatcher operations (`testEmptyDispatcher`)

## Test Coverage Summary

### Classes Covered
✅ **IModel** - Interface tested through implementations  
✅ **IObserver** - Interface tested through implementations  
✅ **CModelBase** - Fully tested (14 tests)  
✅ **CSingleModelObserverBase** - Fully tested (10 tests)  
✅ **CMultiModelObserverBase** - Fully tested (11 tests)  
✅ **TModelWrap** - Fully tested (11 tests)  
✅ **CModelProxy** - Fully tested (11 tests)  
✅ **CModelUpdateBridge** - Fully tested (10 tests)  
✅ **CMultiModelDispatcherBase** - Fully tested (10 tests)  

### Features Tested
- ✅ Observer attachment/detachment
- ✅ Single and multiple model observation
- ✅ Change notification propagation
- ✅ Before/after update callbacks
- ✅ Change set filtering
- ✅ Model proxying and switching
- ✅ Update bridging to IChangeable
- ✅ Multi-model dispatching
- ✅ Template wrapping of IChangeable objects
- ✅ Clone and copy operations
- ✅ Thread-safety mechanisms
- ✅ Edge cases and error conditions

## Building the Tests

### Prerequisites
- Qt 5 or Qt 6
- CMake 3.10 or higher
- C++ compiler with C++11 support
- ACF library built and installed
- ACFDIR environment variable set

### Build with CMake
```bash
cd Tests/ImodTest/CMake
mkdir build && cd build
export ACFDIR=/path/to/Acf
cmake ..
make
```

### Build with QMake
```bash
cd Tests/ImodTest/QMake
export ACFDIR=/path/to/Acf
qmake
make
```

## Running the Tests

After building, run the test executable:

```bash
./ImodTest
```

Run specific test class:
```bash
./ImodTest CModelBaseTest
```

Run specific test method:
```bash
./ImodTest CModelBaseTest::testAttachObserver
```

## Test Output

The tests use Qt Test framework and output results in standard format:
- **PASS** - Test passed successfully
- **FAIL** - Test failed with details
- Test execution time
- Summary of passed/failed tests

## Files

### Test Implementation
- `main.cpp` - Test entry point
- `CModelBaseTest.h/.cpp` - CModelBase tests
- `CSingleModelObserverBaseTest.h/.cpp` - Single observer tests
- `CMultiModelObserverBaseTest.h/.cpp` - Multi-observer tests
- `TModelWrapTest.h/.cpp` - Model wrapper tests
- `CModelProxyTest.h/.cpp` - Model proxy tests
- `CModelUpdateBridgeTest.h/.cpp` - Update bridge tests
- `CMultiModelDispatcherBaseTest.h/.cpp` - Dispatcher tests

### Build Configuration
- `CMake/CMakeLists.txt` - CMake build configuration
- `QMake/ImodTest.pro` - QMake project file
- `ImodTest.acc` - ACF application configuration

## Test Patterns

### Helper Classes
Each test file includes helper classes to support testing:
- **Test data models** - Simple IChangeable implementations
- **Test observers** - Observer implementations that track notifications
- **Test dispatchers** - Dispatcher implementations for verification

### Common Test Structure
```cpp
void TestClass::initTestCase()
{
    // Create test objects
}

void TestClass::testFeature()
{
    // Setup
    // Execute
    // Verify with QVERIFY/QCOMPARE
    // Cleanup
}

void TestClass::cleanupTestCase()
{
    // Delete test objects
}
```

## Continuous Integration

These tests are designed to run in CI environments:
- No GUI required (uses Qt Test console mode)
- Exit codes indicate pass/fail
- JUnit XML output available for CI reporting

## Maintenance

When adding new functionality to the imod library:
1. Add corresponding test methods to appropriate test class
2. Follow existing test patterns
3. Include edge cases and error conditions
4. Update this README with new test descriptions

## Related Documentation

- [imod Library Documentation](../../Docs/ComponentFramework.md)
- [Model/Observer Pattern](../../Include/imod/IObserver.h)
- [Qt Test Framework](https://doc.qt.io/qt-5/qttest-index.html)
- [ACF Testing Guidelines](../README.md)
