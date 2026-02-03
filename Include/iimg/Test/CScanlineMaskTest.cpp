// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CScanlineMaskTest.h"


// ACF includes
#include <i2d/CPosition2d.h>


// protected slots

void CScanlineMaskTest::initTestCase()
{
	m_maskPtr = new iimg::CScanlineMask();
	QVERIFY(m_maskPtr != nullptr);
}


void CScanlineMaskTest::ConstructorTest()
{
	iimg::CScanlineMask mask;
	
	// New mask should be empty
	QVERIFY(mask.IsBitmapRegionEmpty());
	
	// Bounding rect should be invalid for empty mask
	i2d::CRect boundingRect = mask.GetBoundingRect();
	QVERIFY(!boundingRect.IsValid());
}


void CScanlineMaskTest::IsBitmapRegionEmptyTest()
{
	iimg::CScanlineMask mask;
	
	// Initially empty
	QVERIFY(mask.IsBitmapRegionEmpty());
	
	// After creating filled region
	i2d::CRect clipArea(0, 0, 100, 100);
	mask.CreateFilled(clipArea);
	QVERIFY(!mask.IsBitmapRegionEmpty());
}


void CScanlineMaskTest::GetBoundingRectTest()
{
	iimg::CScanlineMask mask;
	
	// Empty mask has invalid bounding rect
	i2d::CRect emptyRect = mask.GetBoundingRect();
	QVERIFY(!emptyRect.IsValid());
	
	// Create filled region
	i2d::CRect clipArea(10, 20, 50, 60);
	mask.CreateFilled(clipArea);
	
	i2d::CRect boundingRect = mask.GetBoundingRect();
	QVERIFY(boundingRect.IsValid());
	QCOMPARE(boundingRect.GetLeft(), clipArea.GetLeft());
	QCOMPARE(boundingRect.GetTop(), clipArea.GetTop());
	QCOMPARE(boundingRect.GetRight(), clipArea.GetRight());
	QCOMPARE(boundingRect.GetBottom(), clipArea.GetBottom());
}


void CScanlineMaskTest::CreateFilledTest()
{
	iimg::CScanlineMask mask;
	i2d::CRect clipArea(0, 0, 100, 50);
	
	mask.CreateFilled(clipArea);
	
	// Verify mask is not empty
	QVERIFY(!mask.IsBitmapRegionEmpty());
	
	// Verify bounding rect matches clip area
	i2d::CRect boundingRect = mask.GetBoundingRect();
	QCOMPARE(boundingRect.GetLeft(), clipArea.GetLeft());
	QCOMPARE(boundingRect.GetTop(), clipArea.GetTop());
	QCOMPARE(boundingRect.GetRight(), clipArea.GetRight());
	QCOMPARE(boundingRect.GetBottom(), clipArea.GetBottom());
}


void CScanlineMaskTest::CreateFromCircleTest()
{
	iimg::CScanlineMask mask;
	
	// Create circle centered at (50, 50) with radius 20
	i2d::CVector2d center(50.0, 50.0);
	i2d::CCircle circle(20.0, center);
	
	i2d::CRect clipArea(0, 0, 100, 100);
	mask.CreateFromCircle(circle, &clipArea);
	
	// Verify mask is not empty
	QVERIFY(!mask.IsBitmapRegionEmpty());
	
	// Verify bounding rect is reasonable (within clip area)
	i2d::CRect boundingRect = mask.GetBoundingRect();
	QVERIFY(boundingRect.IsValid());
	QVERIFY(boundingRect.GetLeft() >= clipArea.GetLeft());
	QVERIFY(boundingRect.GetTop() >= clipArea.GetTop());
	QVERIFY(boundingRect.GetRight() <= clipArea.GetRight());
	QVERIFY(boundingRect.GetBottom() <= clipArea.GetBottom());
}


void CScanlineMaskTest::CreateFromRectangleTest()
{
	iimg::CScanlineMask mask;
	
	// Create rectangle - using left, top, width, height
	i2d::CRectangle rect(30.0, 35.0, 40.0, 30.0);  // left=30, top=35, width=40, height=30
	
	i2d::CRect clipArea(0, 0, 100, 100);
	mask.CreateFromRectangle(rect, &clipArea);
	
	// Verify mask is not empty
	QVERIFY(!mask.IsBitmapRegionEmpty());
	
	// Verify bounding rect is valid
	i2d::CRect boundingRect = mask.GetBoundingRect();
	QVERIFY(boundingRect.IsValid());
}


void CScanlineMaskTest::ResetScanlinesTest()
{
	iimg::CScanlineMask mask;
	
	// Create filled region
	i2d::CRect clipArea(0, 0, 100, 100);
	mask.CreateFilled(clipArea);
	QVERIFY(!mask.IsBitmapRegionEmpty());
	
	// Reset some scanlines
	istd::CIntRange verticalRange(20, 30);
	mask.ResetScanlines(verticalRange);
	
	// Mask should still not be completely empty (only partial reset)
	QVERIFY(!mask.IsBitmapRegionEmpty());
	
	// Reset all scanlines
	istd::CIntRange fullRange(0, 100);
	mask.ResetScanlines(fullRange);
	
	// Now mask should be empty
	QVERIFY(mask.IsBitmapRegionEmpty());
}


void CScanlineMaskTest::GetPixelRangesTest()
{
	iimg::CScanlineMask mask;
	
	// Create filled region
	i2d::CRect clipArea(10, 10, 50, 30);
	mask.CreateFilled(clipArea);
	
	// Get pixel ranges for a line within the region
	const istd::CIntRanges* ranges = mask.GetPixelRanges(20);
	QVERIFY(ranges != nullptr);
	
	// For filled region, should have at least one range
	// Check if ranges is not empty
	QVERIFY(!ranges->IsEmpty());
	
	// Get pixel ranges for a line outside the region
	const istd::CIntRanges* outsideRanges = mask.GetPixelRanges(5);
	// Should be nullptr or empty for line outside region
	QVERIFY(outsideRanges == nullptr || outsideRanges->IsEmpty());
}


void CScanlineMaskTest::GetUnionTest()
{
	iimg::CScanlineMask mask1;
	i2d::CRect clipArea1(0, 0, 50, 50);
	mask1.CreateFilled(clipArea1);
	
	iimg::CScanlineMask mask2;
	i2d::CRect clipArea2(25, 25, 75, 75);
	mask2.CreateFilled(clipArea2);
	
	iimg::CScanlineMask result;
	mask1.GetUnion(mask2, result);
	
	// Result should not be empty
	QVERIFY(!result.IsBitmapRegionEmpty());
	
	// Bounding rect of union should contain both regions
	i2d::CRect boundingRect = result.GetBoundingRect();
	QVERIFY(boundingRect.GetLeft() <= clipArea1.GetLeft());
	QVERIFY(boundingRect.GetTop() <= clipArea1.GetTop());
	QVERIFY(boundingRect.GetRight() >= clipArea2.GetRight());
	QVERIFY(boundingRect.GetBottom() >= clipArea2.GetBottom());
}


void CScanlineMaskTest::GetIntersectionTest()
{
	iimg::CScanlineMask mask1;
	i2d::CRect clipArea1(0, 0, 50, 50);
	mask1.CreateFilled(clipArea1);
	
	iimg::CScanlineMask mask2;
	i2d::CRect clipArea2(25, 25, 75, 75);
	mask2.CreateFilled(clipArea2);
	
	iimg::CScanlineMask result;
	mask1.GetIntersection(mask2, result);
	
	// Result should not be empty (there's overlap)
	QVERIFY(!result.IsBitmapRegionEmpty());
	
	// Bounding rect of intersection should be within both regions
	i2d::CRect boundingRect = result.GetBoundingRect();
	QVERIFY(boundingRect.GetLeft() >= 25);
	QVERIFY(boundingRect.GetTop() >= 25);
	QVERIFY(boundingRect.GetRight() <= 50);
	QVERIFY(boundingRect.GetBottom() <= 50);
}


void CScanlineMaskTest::TranslateTest()
{
	iimg::CScanlineMask mask;
	i2d::CRect clipArea(10, 10, 50, 50);
	mask.CreateFilled(clipArea);
	
	i2d::CRect originalBoundingRect = mask.GetBoundingRect();
	
	// Translate by (20, 30)
	int dx = 20;
	int dy = 30;
	mask.Translate(dx, dy);
	
	i2d::CRect translatedBoundingRect = mask.GetBoundingRect();
	
	// Verify translation
	QCOMPARE(translatedBoundingRect.GetLeft(), originalBoundingRect.GetLeft() + dx);
	QCOMPARE(translatedBoundingRect.GetTop(), originalBoundingRect.GetTop() + dy);
	QCOMPARE(translatedBoundingRect.GetRight(), originalBoundingRect.GetRight() + dx);
	QCOMPARE(translatedBoundingRect.GetBottom(), originalBoundingRect.GetBottom() + dy);
}


void CScanlineMaskTest::InvertTest()
{
	iimg::CScanlineMask mask;
	i2d::CRect clipArea(0, 0, 100, 100);
	
	// Create small region in center
	i2d::CRect innerArea(40, 40, 60, 60);
	mask.CreateFilled(innerArea);
	
	i2d::CRect originalBoundingRect = mask.GetBoundingRect();
	
	// Invert within clip area
	mask.Invert(clipArea);
	
	// After inversion, mask should still not be empty
	QVERIFY(!mask.IsBitmapRegionEmpty());
	
	// Bounding rect should expand to clip area
	i2d::CRect invertedBoundingRect = mask.GetBoundingRect();
	QCOMPARE(invertedBoundingRect.GetLeft(), clipArea.GetLeft());
	QCOMPARE(invertedBoundingRect.GetTop(), clipArea.GetTop());
	QCOMPARE(invertedBoundingRect.GetRight(), clipArea.GetRight());
	QCOMPARE(invertedBoundingRect.GetBottom(), clipArea.GetBottom());
}


void CScanlineMaskTest::DilateTest()
{
	iimg::CScanlineMask mask;
	i2d::CRect clipArea(25, 25, 75, 75);
	mask.CreateFilled(clipArea);
	
	i2d::CRect originalBoundingRect = mask.GetBoundingRect();
	
	// Dilate by 5 pixels in all directions
	mask.Dilate(5, 5, 5, 5);
	
	i2d::CRect dilatedBoundingRect = mask.GetBoundingRect();
	
	// Bounding rect should expand
	QVERIFY(dilatedBoundingRect.GetLeft() <= originalBoundingRect.GetLeft());
	QVERIFY(dilatedBoundingRect.GetTop() <= originalBoundingRect.GetTop());
	QVERIFY(dilatedBoundingRect.GetRight() >= originalBoundingRect.GetRight());
	QVERIFY(dilatedBoundingRect.GetBottom() >= originalBoundingRect.GetBottom());
}


void CScanlineMaskTest::ErodeTest()
{
	iimg::CScanlineMask mask;
	i2d::CRect clipArea(0, 0, 100, 100);
	mask.CreateFilled(clipArea);
	
	i2d::CRect originalBoundingRect = mask.GetBoundingRect();
	
	// Erode by 5 pixels in all directions
	mask.Erode(5, 5, 5, 5);
	
	i2d::CRect erodedBoundingRect = mask.GetBoundingRect();
	
	// Bounding rect should shrink (or mask becomes empty if erosion is too large)
	if (!mask.IsBitmapRegionEmpty())
	{
		QVERIFY(erodedBoundingRect.GetLeft() >= originalBoundingRect.GetLeft());
		QVERIFY(erodedBoundingRect.GetTop() >= originalBoundingRect.GetTop());
		QVERIFY(erodedBoundingRect.GetRight() <= originalBoundingRect.GetRight());
		QVERIFY(erodedBoundingRect.GetBottom() <= originalBoundingRect.GetBottom());
	}
}


void CScanlineMaskTest::EqualityOperatorTest()
{
	iimg::CScanlineMask mask1;
	i2d::CRect clipArea(10, 10, 50, 50);
	mask1.CreateFilled(clipArea);
	
	iimg::CScanlineMask mask2;
	mask2.CreateFilled(clipArea);
	
	// Two masks with same content should be equal
	QVERIFY(mask1 == mask2);
	
	// Create different mask
	iimg::CScanlineMask mask3;
	i2d::CRect differentArea(20, 20, 60, 60);
	mask3.CreateFilled(differentArea);
	
	// Different masks should not be equal
	QVERIFY(!(mask1 == mask3));
}


void CScanlineMaskTest::InequalityOperatorTest()
{
	iimg::CScanlineMask mask1;
	i2d::CRect clipArea1(10, 10, 50, 50);
	mask1.CreateFilled(clipArea1);
	
	iimg::CScanlineMask mask2;
	i2d::CRect clipArea2(20, 20, 60, 60);
	mask2.CreateFilled(clipArea2);
	
	// Different masks should be not equal
	QVERIFY(mask1 != mask2);
	
	// Create same mask
	iimg::CScanlineMask mask3;
	mask3.CreateFilled(clipArea1);
	
	// Same masks should not be not equal
	QVERIFY(!(mask1 != mask3));
}


void CScanlineMaskTest::cleanupTestCase()
{
	delete m_maskPtr;
	m_maskPtr = nullptr;
}


I_ADD_TEST(CScanlineMaskTest);
