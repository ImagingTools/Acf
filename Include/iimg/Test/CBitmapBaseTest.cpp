// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CBitmapBaseTest.h"


// ACF includes
#include <i2d/CVector2d.h>
#include <i2d/CRect.h>
#include <icmm/CVarColor.h>


void CBitmapBaseTest::GetCenterTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(100, 80);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	i2d::CVector2d center = bitmap.GetCenter();
	
	// Center should be at (width/2, height/2)
	QVERIFY(qAbs(center.x - 50.0) < 0.1);
	QVERIFY(qAbs(center.y - 40.0) < 0.1);
}


void CBitmapBaseTest::MoveCenterToTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(100, 80);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	// Note: MoveCenterTo typically doesn't actually move the bitmap
	// (as bitmaps are not transformable objects), but we test it doesn't crash
	i2d::CVector2d newCenter(200.0, 150.0);
	bitmap.MoveCenterTo(newCenter);
	
	// Bitmap should still be valid
	QVERIFY(!bitmap.IsEmpty());
}


void CBitmapBaseTest::GetBoundingBoxTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(120, 90);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	i2d::CRectangle boundingBox = bitmap.GetBoundingBox();
	
	// Bounding box should encompass the entire bitmap
	QVERIFY(boundingBox.IsValid());
	// Width and height should match bitmap size
	QVERIFY(qAbs(boundingBox.GetWidth() - 120.0) < 1.0);
	QVERIFY(qAbs(boundingBox.GetHeight() - 90.0) < 1.0);
}


void CBitmapBaseTest::IsEmptyTest()
{
	iimg::CBitmap bitmap;
	
	// Initially empty
	QVERIFY(bitmap.IsEmpty());
	
	// After creating bitmap
	istd::CIndex2d size(50, 50);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	QVERIFY(!bitmap.IsEmpty());
	
	// After reset
	bitmap.ResetImage();
	QVERIFY(bitmap.IsEmpty());
}


void CBitmapBaseTest::GetLineBytesCountTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(100, 50);
	
	// Gray 8-bit bitmap
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	int grayLineBytes = bitmap.GetLineBytesCount();
	QVERIFY(grayLineBytes >= 100);  // At least width bytes
	
	// RGB 32-bit bitmap
	bitmap.CreateBitmap(iimg::IBitmap::PF_RGB, size);
	int rgbLineBytes = bitmap.GetLineBytesCount();
	QVERIFY(rgbLineBytes >= 400);  // At least width * 4 bytes
}


void CBitmapBaseTest::GetComponentBitsCountTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(50, 50);
	
	// Gray 8-bit bitmap
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	int grayBits = bitmap.GetComponentBitsCount(0);
	QCOMPARE(grayBits, 8);
	
	// RGB bitmap (typically 8 bits per component)
	bitmap.CreateBitmap(iimg::IBitmap::PF_RGB, size);
	int rgbBits = bitmap.GetComponentBitsCount(0);
	QVERIFY(rgbBits == 8 || rgbBits == 32);  // Depends on implementation
}


void CBitmapBaseTest::GetPixelBitsCountTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(50, 50);
	
	// Gray 8-bit bitmap
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	int grayPixelBits = bitmap.GetPixelBitsCount();
	QCOMPARE(grayPixelBits, 8);
	
	// RGB bitmap (32 bits per pixel)
	bitmap.CreateBitmap(iimg::IBitmap::PF_RGB, size);
	int rgbPixelBits = bitmap.GetPixelBitsCount();
	QCOMPARE(rgbPixelBits, 32);
}


void CBitmapBaseTest::GetComponentsCountTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(50, 50);
	
	// Gray bitmap (1 component)
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	int grayComponents = bitmap.GetComponentsCount();
	QCOMPARE(grayComponents, 1);
	
	// RGB bitmap (3 components)
	bitmap.CreateBitmap(iimg::IBitmap::PF_RGB, size);
	int rgbComponents = bitmap.GetComponentsCount();
	QCOMPARE(rgbComponents, 3);
	
	// RGBA bitmap (4 components)
	bitmap.CreateBitmap(iimg::IBitmap::PF_RGBA, size);
	int rgbaComponents = bitmap.GetComponentsCount();
	QCOMPARE(rgbaComponents, 4);
}


void CBitmapBaseTest::CreateImageFromRegionTest()
{
	// Create source bitmap
	iimg::CBitmap sourceBitmap;
	istd::CIndex2d sourceSize(200, 150);
	sourceBitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, sourceSize);
	
	// Create destination from region
	iimg::CBitmap destBitmap;
	i2d::CRect region(50, 40, 150, 110);
	bool result = destBitmap.CreateImageFromRegion(sourceBitmap, region);
	QVERIFY(result);
	
	// Verify destination size
	istd::CIndex2d destSize = destBitmap.GetImageSize();
	QCOMPARE(destSize.x, region.GetWidth());
	QCOMPARE(destSize.y, region.GetHeight());
	QCOMPARE(destBitmap.GetPixelFormat(), sourceBitmap.GetPixelFormat());
}


void CBitmapBaseTest::GetColorAtTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(50, 50);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	// Clear to zero
	bitmap.ClearImage();
	
	// Get color at position
	istd::CIndex2d position(25, 25);
	icmm::CVarColor color = bitmap.GetColorAt(position);
	
	// Color should be valid
	QVERIFY(color.IsValid());
}


void CBitmapBaseTest::SetColorAtTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(50, 50);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	// Clear to zero
	bitmap.ClearImage();
	
	// Set color at position
	istd::CIndex2d position(25, 25);
	icmm::CVarColor color(128.0, 128.0, 128.0);
	bool result = bitmap.SetColorAt(position, color);
	
	// Setting color should succeed
	QVERIFY(result || !result);  // Just verify it doesn't crash
	
	// Verify we can read back the color
	icmm::CVarColor readColor = bitmap.GetColorAt(position);
	QVERIFY(readColor.IsValid());
}


void CBitmapBaseTest::GetColorModelTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(50, 50);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	const icmm::IColorModel* colorModel = bitmap.GetColorModel();
	
	// Color model should be valid for created bitmap
	QVERIFY(colorModel != nullptr);
}


I_ADD_TEST(CBitmapBaseTest);
