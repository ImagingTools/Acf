# AutoPersistenceTest

Component test for CAutoPersistenceComp (ifile package).

## Overview

This test suite validates the CAutoPersistenceComp component, which provides automatic object persistence functionality. The component can save and restore objects based on various triggers and configurations.

## Test Configurations

The test includes six different Partitura (.acc) configurations, each testing different combinations of component attributes:

### 1. AutoPersistenceTestRestoreStoreEnd.acc
- **RestoreOnBegin**: true
- **StoreOnEnd**: true
- **Purpose**: Tests basic save/restore functionality
- **Validates**: Object restoration on component initialization and storage on destruction

### 2. AutoPersistenceTestStoreOnChange.acc
- **StoreOnChange**: true
- **Purpose**: Tests immediate persistence on data changes
- **Validates**: File is updated immediately whenever the object's data changes

### 3. AutoPersistenceTestStoreOnBegin.acc
- **StoreOnBegin**: true
- **Purpose**: Tests initial file creation
- **Validates**: File is created on component initialization if it doesn't exist

### 4. AutoPersistenceTestStoreInterval.acc
- **StoreInterval**: 1 second
- **Purpose**: Tests timer-based automatic persistence
- **Validates**: Object is saved at regular intervals (when changed)

### 5. AutoPersistenceTestAutoReload.acc
- **AutoReload**: true
- **RestoreOnBegin**: true
- **StoreOnEnd**: true
- **Purpose**: Tests file watching and automatic reload
- **Validates**: Component can detect and reload external file changes

### 6. AutoPersistenceTestLocking.acc
- **StoreOnChange**: true
- **StaleLockTime**: 30 seconds
- **TryLockTimeout**: 5 seconds
- **EnableLockForRead**: true
- **Purpose**: Tests lock file mechanism
- **Validates**: Proper file locking behavior during read/write operations

## Test Object

All tests use a `SelectionParam` component as the persistent object. This provides a simple integer value that can be:
- Set and retrieved easily
- Serialized and deserialized
- Used to verify data integrity across save/restore cycles

## Building

From the ACF build directory:
```bash
mkdir build && cd build
cmake ..
make AutoPersistenceTest
```

## Running

```bash
./AutoPersistenceTest
```

Run specific test:
```bash
./AutoPersistenceTest CAutoPersistenceTest::RestoreOnBeginStoreOnEndTest
```

## Test Coverage

The test suite validates:
- Component initialization and cleanup
- File creation and deletion
- Data persistence across component lifecycles
- Different persistence triggers (on change, on end, on interval)
- Lock file handling
- File path management
- Error handling for various scenarios

## Implementation Notes

- Tests use temporary file paths to avoid interference
- Each test cleans up its test files after execution
- Lock files are also cleaned up
- Tests use QThread::msleep and QCoreApplication::processEvents to handle asynchronous operations
- File operations are verified using QFile::exists
