#!/bin/bash
# Test script for Build Error Analysis workflow
# This validates the artifact extraction and error parsing logic locally

set -euo pipefail

echo "=== Build Error Analysis Workflow Test ==="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counters
TESTS_PASSED=0
TESTS_FAILED=0

# Function to run a test
run_test() {
    local test_name="$1"
    local test_func="$2"
    
    echo "-------------------------------------------"
    echo "Test: $test_name"
    echo "-------------------------------------------"
    
    if $test_func; then
        echo -e "${GREEN}✅ PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}❌ FAIL${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    echo ""
}

# Test 1: Artifact file extraction logic
test_artifact_extraction() {
    echo "Testing artifact extraction logic..."
    
    # Create mock artifact directory
    local tmp_dir=$(mktemp -d)
    mkdir -p "$tmp_dir/teamcity-artifacts"
    
    # Create mock build ID files
    echo "12345" > "$tmp_dir/teamcity-artifacts/teamcity-build-id-windows.txt"
    echo "67890" > "$tmp_dir/teamcity-artifacts/teamcity-build-id-linux.txt"
    
    # Run extraction logic
    cd "$tmp_dir"
    BUILD_IDS=""
    
    if [ -d "./teamcity-artifacts" ]; then
        shopt -s nullglob
        for id_file in ./teamcity-artifacts/teamcity-build-id-*.txt; do
            if [ -f "$id_file" ]; then
                BUILD_ID=$(tr -d '[:space:]' < "$id_file")
                if [ -n "$BUILD_ID" ]; then
                    echo "Found build ID: $BUILD_ID (from $(basename "$id_file"))"
                    BUILD_IDS="$BUILD_IDS $BUILD_ID"
                fi
            fi
        done
        shopt -u nullglob
        BUILD_IDS=$(echo "$BUILD_IDS" | xargs)
    fi
    
    # Cleanup
    rm -rf "$tmp_dir"
    
    # Verify (order may vary, so sort and check)
    BUILD_IDS_SORTED=$(echo "$BUILD_IDS" | tr ' ' '\n' | sort -n | tr '\n' ' ' | xargs)
    EXPECTED_SORTED="12345 67890"
    
    if [ "$BUILD_IDS_SORTED" = "$EXPECTED_SORTED" ]; then
        echo "Build IDs extracted correctly: $BUILD_IDS (sorted: $BUILD_IDS_SORTED)"
        return 0
    else
        echo "Expected '$EXPECTED_SORTED' (sorted), got '$BUILD_IDS_SORTED'"
        return 1
    fi
}

# Test 2: Handling missing artifacts directory
test_missing_artifacts() {
    echo "Testing missing artifacts directory handling..."
    
    local tmp_dir=$(mktemp -d)
    cd "$tmp_dir"
    
    BUILD_IDS=""
    
    if [ -d "./teamcity-artifacts" ]; then
        echo "ERROR: Directory should not exist"
        rm -rf "$tmp_dir"
        return 1
    else
        echo "No artifacts directory found (expected)"
    fi
    
    BUILD_IDS=$(echo "$BUILD_IDS" | xargs)
    
    # Cleanup
    rm -rf "$tmp_dir"
    
    if [ -z "$BUILD_IDS" ]; then
        echo "Build IDs empty as expected"
        return 0
    else
        echo "Expected empty BUILD_IDS, got '$BUILD_IDS'"
        return 1
    fi
}

# Test 3: Handling empty build ID files
test_empty_build_id_files() {
    echo "Testing empty build ID file handling..."
    
    local tmp_dir=$(mktemp -d)
    mkdir -p "$tmp_dir/teamcity-artifacts"
    
    # Create empty file
    touch "$tmp_dir/teamcity-artifacts/teamcity-build-id-windows.txt"
    # Create file with only whitespace
    echo "   " > "$tmp_dir/teamcity-artifacts/teamcity-build-id-linux.txt"
    
    cd "$tmp_dir"
    BUILD_IDS=""
    
    if [ -d "./teamcity-artifacts" ]; then
        shopt -s nullglob
        for id_file in ./teamcity-artifacts/teamcity-build-id-*.txt; do
            if [ -f "$id_file" ]; then
                BUILD_ID=$(tr -d '[:space:]' < "$id_file")
                if [ -n "$BUILD_ID" ]; then
                    BUILD_IDS="$BUILD_IDS $BUILD_ID"
                fi
            fi
        done
        shopt -u nullglob
        BUILD_IDS=$(echo "$BUILD_IDS" | xargs)
    fi
    
    # Cleanup
    rm -rf "$tmp_dir"
    
    if [ -z "$BUILD_IDS" ]; then
        echo "Empty files handled correctly - no IDs extracted"
        return 0
    else
        echo "Expected no IDs, got '$BUILD_IDS'"
        return 1
    fi
}

# Test 4: Whitespace trimming
test_whitespace_trimming() {
    echo "Testing whitespace trimming..."
    
    local tmp_dir=$(mktemp -d)
    mkdir -p "$tmp_dir/teamcity-artifacts"
    
    # Create files with various whitespace
    echo "  12345  " > "$tmp_dir/teamcity-artifacts/teamcity-build-id-windows.txt"
    echo -e "67890\n" > "$tmp_dir/teamcity-artifacts/teamcity-build-id-linux.txt"
    
    cd "$tmp_dir"
    BUILD_IDS=""
    
    if [ -d "./teamcity-artifacts" ]; then
        shopt -s nullglob
        for id_file in ./teamcity-artifacts/teamcity-build-id-*.txt; do
            if [ -f "$id_file" ]; then
                BUILD_ID=$(tr -d '[:space:]' < "$id_file")
                if [ -n "$BUILD_ID" ]; then
                    BUILD_IDS="$BUILD_IDS $BUILD_ID"
                fi
            fi
        done
        shopt -u nullglob
        BUILD_IDS=$(echo "$BUILD_IDS" | xargs)
    fi
    
    # Cleanup
    rm -rf "$tmp_dir"
    
    # Verify (order may vary, so sort and check)
    BUILD_IDS_SORTED=$(echo "$BUILD_IDS" | tr ' ' '\n' | sort -n | tr '\n' ' ' | xargs)
    EXPECTED_SORTED="12345 67890"
    
    if [ "$BUILD_IDS_SORTED" = "$EXPECTED_SORTED" ]; then
        echo "Whitespace trimmed correctly: '$BUILD_IDS' (sorted: $BUILD_IDS_SORTED)"
        return 0
    else
        echo "Expected '$EXPECTED_SORTED' (sorted), got '$BUILD_IDS_SORTED'"
        return 1
    fi
}

# Test 5: Single platform only
test_single_platform() {
    echo "Testing single platform artifact..."
    
    local tmp_dir=$(mktemp -d)
    mkdir -p "$tmp_dir/teamcity-artifacts"
    
    # Only Windows build
    echo "99999" > "$tmp_dir/teamcity-artifacts/teamcity-build-id-windows.txt"
    
    cd "$tmp_dir"
    BUILD_IDS=""
    
    if [ -d "./teamcity-artifacts" ]; then
        shopt -s nullglob
        for id_file in ./teamcity-artifacts/teamcity-build-id-*.txt; do
            if [ -f "$id_file" ]; then
                BUILD_ID=$(tr -d '[:space:]' < "$id_file")
                if [ -n "$BUILD_ID" ]; then
                    BUILD_IDS="$BUILD_IDS $BUILD_ID"
                fi
            fi
        done
        shopt -u nullglob
        BUILD_IDS=$(echo "$BUILD_IDS" | xargs)
    fi
    
    # Cleanup
    rm -rf "$tmp_dir"
    
    if [ "$BUILD_IDS" = "99999" ]; then
        echo "Single platform handled correctly: '$BUILD_IDS'"
        return 0
    else
        echo "Expected '99999', got '$BUILD_IDS'"
        return 1
    fi
}

# Test 6: Error pattern matching
test_error_pattern_matching() {
    echo "Testing error pattern matching..."
    
    # Sample build log
    BUILD_LOG='
[12:34:56] Building project...
[12:34:57] Compiling source files...
[12:34:58] error: ILogger was not declared in this scope
[12:34:59]   42 | ILogger* logger = GetLogger
[12:35:00]      | ^~~~~~~
[12:35:01] error: no matching function for call to Initialize
[12:35:02] ld: undefined reference to ilog::LogRouter::LogMessage
[12:35:03] CMake Error at CMakeLists.txt:42 find_package
[12:35:04]   Could not find a package configuration file provided by Qt5
[12:35:05] Build finished with errors
'
    
    # Test compilation errors
    COMPILE_ERRORS=$(echo "$BUILD_LOG" | grep -iE "error[: ]|fatal[: ]" | head -n 50 || echo "")
    
    # Test linker errors
    LINKER_ERRORS=$(echo "$BUILD_LOG" | grep -E "undefined reference|unresolved external|cannot find -l" | head -n 20 || echo "")
    
    # Test CMake errors
    CMAKE_ERRORS=$(echo "$BUILD_LOG" | grep -E "CMake Error" | head -n 20 || echo "")
    
    local found_compile=0
    local found_linker=0
    local found_cmake=0
    
    if [ -n "$COMPILE_ERRORS" ]; then
        echo "✓ Found compilation errors"
        found_compile=1
    fi
    
    if [ -n "$LINKER_ERRORS" ]; then
        echo "✓ Found linker errors"
        found_linker=1
    fi
    
    if [ -n "$CMAKE_ERRORS" ]; then
        echo "✓ Found CMake errors"
        found_cmake=1
    fi
    
    if [ $found_compile -eq 1 ] && [ $found_linker -eq 1 ] && [ $found_cmake -eq 1 ]; then
        return 0
    else
        echo "Not all error types detected"
        return 1
    fi
}

# Test 7: Nullglob behavior
test_nullglob_behavior() {
    echo "Testing nullglob behavior with no matching files..."
    
    local tmp_dir=$(mktemp -d)
    mkdir -p "$tmp_dir/teamcity-artifacts"
    
    # Create non-matching files
    echo "12345" > "$tmp_dir/teamcity-artifacts/other-file.txt"
    
    cd "$tmp_dir"
    BUILD_IDS=""
    
    if [ -d "./teamcity-artifacts" ]; then
        shopt -s nullglob
        for id_file in ./teamcity-artifacts/teamcity-build-id-*.txt; do
            if [ -f "$id_file" ]; then
                BUILD_ID=$(tr -d '[:space:]' < "$id_file")
                if [ -n "$BUILD_ID" ]; then
                    BUILD_IDS="$BUILD_IDS $BUILD_ID"
                fi
            fi
        done
        shopt -u nullglob
        BUILD_IDS=$(echo "$BUILD_IDS" | xargs)
    fi
    
    # Cleanup
    rm -rf "$tmp_dir"
    
    if [ -z "$BUILD_IDS" ]; then
        echo "Nullglob prevented errors with no matching files"
        return 0
    else
        echo "Expected empty BUILD_IDS, got '$BUILD_IDS'"
        return 1
    fi
}

# Run all tests
echo "Starting Build Error Analysis workflow tests..."
echo ""

run_test "Artifact Extraction" test_artifact_extraction
run_test "Missing Artifacts Directory" test_missing_artifacts
run_test "Empty Build ID Files" test_empty_build_id_files
run_test "Whitespace Trimming" test_whitespace_trimming
run_test "Single Platform" test_single_platform
run_test "Error Pattern Matching" test_error_pattern_matching
run_test "Nullglob Behavior" test_nullglob_behavior

# Summary
echo "==========================================="
echo "Test Summary"
echo "==========================================="
echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Failed: ${RED}$TESTS_FAILED${NC}"
echo "Total: $((TESTS_PASSED + TESTS_FAILED))"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed!${NC}"
    exit 1
fi
