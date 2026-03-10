#!/bin/bash
# Script to check for TIndex::Increase() usage that might be affected by row-major change
# Run this in AcfSln, IAcf, ImtCore, and Acula repositories

echo "=== Checking for TIndex::Increase() usage ==="
echo ""

echo "1. Direct Increase() calls:"
grep -rn "\.Increase(" --include="*.cpp" --include="*.h" . | grep -v "IncreaseCount\|IncreaseRef\|IncreaseAt" | head -20

echo ""
echo "2. TArray iterator usage:"
grep -rn "TArray.*::.*Iterator\|\.Begin()\|\.End()" --include="*.cpp" --include="*.h" . | head -20

echo ""
echo "3. do-while loops with Increase:"
grep -rn "do.*{" --include="*.cpp" --include="*.h" . -A10 | grep -B5 "\.Increase(" | head -40

echo ""
echo "4. Check for TMultidimensionalPolynomial usage:"
grep -rn "TMultidimensionalPolynomial\|ApproximateCoefficientsFromFulcrums" --include="*.cpp" --include="*.h" . | head -20

echo ""
echo "=== Analysis Guidelines ==="
echo "SAFE: Code that processes all elements (order doesn't matter)"
echo "RISKY: Code that builds lookup tables with linear indexing"
echo "CRITICAL: Paired loops that must use same iteration order"
echo ""
echo "Row-major order: Last dimension increments first"
echo "Example 2x3: [0,0] -> [0,1] -> [0,2] -> [1,0] -> [1,1] -> [1,2]"
