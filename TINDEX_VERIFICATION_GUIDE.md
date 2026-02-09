# TIndex::Increase() Change - External Repository Verification Guide

## Background

In commit 4ff9db8, `TIndex::Increase()` and `Decrease()` were changed from **column-major** to **row-major** iteration order to match standard nested loop patterns.

**What changed:**
- **Before**: `for (int i = 0; i < Dimensions; ++i)` - dimension 0 first
- **After**: `for (int i = Dimensions - 1; i >= 0; --i)` - last dimension first

**Example (2x3 array):**
- **Column-major (old)**: [0,0] → [1,0] → [0,1] → [1,1] → [0,2] → [1,2]
- **Row-major (new)**: [0,0] → [0,1] → [0,2] → [1,0] → [1,1] → [1,2]

## Repositories to Check

The following repositories may have code depending on iteration order:
- **ImagingTools/AcfSln**
- **ImagingTools/IAcf**
- **ImagingTools/ImtCore**
- **ImagingTools/Acula**

## How to Verify

### Step 1: Run the Check Script

Copy `check_tindex_usage.sh` to each repository and run:

```bash
./check_tindex_usage.sh > tindex_check_results.txt
```

Review `tindex_check_results.txt` for any usage patterns.

### Step 2: Manual Code Review

For each usage found, classify it:

#### ✅ SAFE Pattern (order-independent)
```cpp
// Example: Processing all elements
TIndex<3> index;
do {
    double value = array[index];
    processValue(value);  // Order doesn't matter
} while (index.Increase(bounds));
```

#### ⚠️ RISKY Pattern (order-dependent)
```cpp
// Example: Building linearized lookup table
TIndex<2> index;
int linearIdx = 0;
do {
    // PROBLEM: linearIdx depends on iteration order!
    lookupTable[linearIdx++] = computeValue(index);
} while (index.Increase(bounds));
```

#### 🔴 CRITICAL Pattern (paired operations)
```cpp
// Example: Two loops must match
TIndex<2> idx1;
int col = 0;
do {
    matrix.Set(col++, row, compute(idx1));
} while (idx1.Increase(size));

// Later...
TIndex<2> idx2;
int col2 = 0;
do {
    // PROBLEM: Must use same order as first loop!
    result[idx2] = matrix.Get(col2++, row);
} while (idx2.Increase(size));
```

### Step 3: Review TArray Iterator Usage

Search for iterator usage:
```bash
grep -rn "\.Begin()\|\.End()" --include="*.cpp" --include="*.h"
```

**TArray iterators now traverse in row-major order.** Check if any code assumes column-major order.

### Step 4: Run Tests

After analysis, run all repository tests:
```bash
# Build and test
cmake --build . --target all
ctest --output-on-failure
```

## Common Issues to Look For

### 1. Image Processing Code
```cpp
// Example: Scanning image pixels
TArray<Pixel, 2> image;
for (auto it = image.Begin(); it != image.End(); ++it) {
    // Was: column-by-column (all rows in column 0, then column 1, etc.)
    // Now: row-by-row (all columns in row 0, then row 1, etc.)
    processPixel(*it);
}
```

**Impact**: If code assumes column-major scanning, it may process pixels in wrong order.

### 2. Multi-dimensional Data Structures
```cpp
// Example: 3D volume data
TArray<double, 3> volume(size_x, size_y, size_z);
int idx = 0;
TIndex<3> pos;
do {
    linearArray[idx++] = volume[pos];  // Order matters!
} while (pos.Increase(volume.GetSizes()));
```

**Impact**: Linear array will have different order.

### 3. Matrix Operations
```cpp
// Building matrix from coefficients
TIndex<2> coeffIdx;
int matrixCol = 0;
do {
    matrix.SetColumn(matrixCol++, computeValue(coeffIdx));
} while (coeffIdx.Increase(coeffSize));
```

**Impact**: Matrix columns will be in different order.

## Verification Checklist

For each repository:

- [ ] Run `check_tindex_usage.sh` and review results
- [ ] Classify each usage as SAFE/RISKY/CRITICAL
- [ ] Review TArray iterator usage
- [ ] Check image processing code
- [ ] Check multi-dimensional array code
- [ ] Run all unit tests
- [ ] Run integration tests
- [ ] Check for performance regressions

## Known Safe Code in Acf

These patterns in Acf have been verified safe:

1. **TArray::Iterator** - Intended change, now row-major
2. **TMultidimensionalPolynomial** - Uses consistent order in paired loops
3. **CVarMatrix::GetTriangleDecomposed** - Uses `IncreaseAt()`, not affected

## Contact

If you find code that appears to be affected, please report:
- Repository name
- File and line number
- Code pattern (SAFE/RISKY/CRITICAL)
- Description of the dependency on iteration order

## References

- Original change: Acf commit 4ff9db8
- Analysis: Acf commit d02be3f
- Test coverage: TArrayTest::IteratorTest() in Acf
