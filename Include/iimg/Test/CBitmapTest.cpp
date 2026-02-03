// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CBitmapTest.h"


// protected slots

void CBitmapTest::initTestCase()
{
	m_bitmapPtr = new iimg::CBitmap();
	QVERIFY(m_bitmapPtr != nullptr);
}


void CBitmapTest::ConstructorTest()
{
	iimg::CBitmap bitmap;
	
	// New bitmap should have zero size
	istd::CIndex2d size = bitmap.GetImageSize();
	QVERIFY(size.GetX() == 0);
	QVERIFY(size.GetY() == 0);
	
	// Check pixel format is unknown for empty bitmap
	QVERIFY(bitmap.GetPixelFormat() == iimg::IBitmap::PF_UNKNOWN);
}


void CBitmapTest::CopyConstructorTest()
{
	// Create a bitmap with some data
	iimg::CBitmap original;
	istd::CIndex2d size(100, 50);
	bool result = original.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	QVERIFY(result);
	
	// Copy it
	iimg::CBitmap copy(original);
	
	// Verify copy has same properties
	istd::CIndex2d copySize = copy.GetImageSize();
	QCOMPARE(copySize.GetX(), size.GetX());
	QCOMPARE(copySize.GetY(), size.GetY());
	QCOMPARE(copy.GetPixelFormat(), original.GetPixelFormat());
}


void CBitmapTest::QImageConstructorTest()
{
	// Create a QImage
	QImage image(200, 100, QImage::Format_RGB32);
	image.fill(Qt::red);
	
	// Create bitmap from QImage
	iimg::CBitmap bitmap(image);
	
	// Verify properties
	istd::CIndex2d size = bitmap.GetImageSize();
	QCOMPARE(size.GetX(), 200);
	QCOMPARE(size.GetY(), 100);
	QCOMPARE(bitmap.GetPixelFormat(), iimg::IBitmap::PF_RGB);
}


void CBitmapTest::CreateBitmapTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(256, 128);
	
	bool result = bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	QVERIFY(result);
	
	// Verify size
	istd::CIndex2d actualSize = bitmap.GetImageSize();
	QCOMPARE(actualSize.GetX(), size.GetX());
	QCOMPARE(actualSize.GetY(), size.GetY());
	
	// Verify format
	QCOMPARE(bitmap.GetPixelFormat(), iimg::IBitmap::PF_GRAY);
}


void CBitmapTest::CreateBitmapGrayTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(64, 64);
	
	bool result = bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	QVERIFY(result);
	QCOMPARE(bitmap.GetPixelFormat(), iimg::IBitmap::PF_GRAY);
	
	// Verify we can get line pointers
	const void* linePtr = bitmap.GetLinePtr(0);
	QVERIFY(linePtr != nullptr);
}


void CBitmapTest::CreateBitmapRGBTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(128, 96);
	
	bool result = bitmap.CreateBitmap(iimg::IBitmap::PF_RGB, size);
	QVERIFY(result);
	QCOMPARE(bitmap.GetPixelFormat(), iimg::IBitmap::PF_RGB);
	
	istd::CIndex2d actualSize = bitmap.GetImageSize();
	QCOMPARE(actualSize.GetX(), size.GetX());
	QCOMPARE(actualSize.GetY(), size.GetY());
}


void CBitmapTest::CreateBitmapRGBATest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(256, 256);
	
	bool result = bitmap.CreateBitmap(iimg::IBitmap::PF_RGBA, size);
	QVERIFY(result);
	QCOMPARE(bitmap.GetPixelFormat(), iimg::IBitmap::PF_RGBA);
	
	istd::CIndex2d actualSize = bitmap.GetImageSize();
	QCOMPARE(actualSize.GetX(), size.GetX());
	QCOMPARE(actualSize.GetY(), size.GetY());
}


void CBitmapTest::GetPixelFormatTest()
{
	iimg::CBitmap bitmap;
	
	// Empty bitmap should have unknown format
	QVERIFY(bitmap.GetPixelFormat() == iimg::IBitmap::PF_UNKNOWN);
	
	// Create with specific format
	istd::CIndex2d size(100, 100);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	QCOMPARE(bitmap.GetPixelFormat(), iimg::IBitmap::PF_GRAY);
}


void CBitmapTest::IsFormatSupportedTest()
{
	iimg::CBitmap bitmap;
	
	// Check commonly supported formats
	QVERIFY(bitmap.IsFormatSupported(iimg::IBitmap::PF_MONO));
	QVERIFY(bitmap.IsFormatSupported(iimg::IBitmap::PF_GRAY));
	QVERIFY(bitmap.IsFormatSupported(iimg::IBitmap::PF_RGB));
	QVERIFY(bitmap.IsFormatSupported(iimg::IBitmap::PF_RGBA));
	
	// Unknown format should not be supported
	QVERIFY(!bitmap.IsFormatSupported(iimg::IBitmap::PF_UNKNOWN));
}


void CBitmapTest::GetImageSizeTest()
{
	iimg::CBitmap bitmap;
	
	// Empty bitmap
	istd::CIndex2d emptySize = bitmap.GetImageSize();
	QVERIFY(emptySize.GetX() == 0);
	QVERIFY(emptySize.GetY() == 0);
	
	// After creating bitmap
	istd::CIndex2d size(320, 240);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	istd::CIndex2d actualSize = bitmap.GetImageSize();
	QCOMPARE(actualSize.GetX(), size.GetX());
	QCOMPARE(actualSize.GetY(), size.GetY());
}


void CBitmapTest::ResetImageTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(100, 100);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	// Verify bitmap is not empty
	istd::CIndex2d actualSize = bitmap.GetImageSize();
	QVERIFY(actualSize.GetX() > 0);
	QVERIFY(actualSize.GetY() > 0);
	
	// Reset the image
	bitmap.ResetImage();
	
	// Verify bitmap is now empty
	istd::CIndex2d emptySize = bitmap.GetImageSize();
	QVERIFY(emptySize.GetX() == 0);
	QVERIFY(emptySize.GetY() == 0);
}


void CBitmapTest::ClearImageTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(50, 50);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	// Fill with some data
	void* linePtr = bitmap.GetLinePtr(0);
	if (linePtr)
	{
		memset(linePtr, 255, size.GetX());
	}
	
	// Clear the image (should fill with zeros)
	bitmap.ClearImage();
	
	// Verify image is still valid but cleared
	istd::CIndex2d actualSize = bitmap.GetImageSize();
	QCOMPARE(actualSize.GetX(), size.GetX());
	QCOMPARE(actualSize.GetY(), size.GetY());
	
	// Check first pixel is zero
	const void* constLinePtr = bitmap.GetLinePtr(0);
	QVERIFY(constLinePtr != nullptr);
	const quint8* pixelPtr = static_cast<const quint8*>(constLinePtr);
	QCOMPARE(pixelPtr[0], quint8(0));
}


void CBitmapTest::GetLinesPtrTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(100, 50);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	// Test const version
	const void* constLinePtr = bitmap.GetLinePtr(0);
	QVERIFY(constLinePtr != nullptr);
	
	// Test non-const version
	void* linePtr = bitmap.GetLinePtr(10);
	QVERIFY(linePtr != nullptr);
	
	// Test invalid line index - behavior is implementation-defined
	// but should either return nullptr or be caught by bounds checking
	const void* invalidPtr = bitmap.GetLinePtr(100);
	// Don't assert on behavior since it's implementation-defined
	Q_UNUSED(invalidPtr);
}


void CBitmapTest::GetLinesDifferenceTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(100, 50);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	int linesDiff = bitmap.GetLinesDifference();
	// Lines difference should be non-negative
	QVERIFY(linesDiff >= 0);
	
	// For most formats, should be at least the width
	QVERIFY(linesDiff >= size.GetX());
}


void CBitmapTest::CopyFromTest()
{
	iimg::CBitmap source;
	istd::CIndex2d size(100, 100);
	source.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	iimg::CBitmap destination;
	bool result = destination.CopyFrom(source);
	QVERIFY(result);
	
	// Verify properties match
	QCOMPARE(destination.GetImageSize().GetX(), source.GetImageSize().GetX());
	QCOMPARE(destination.GetImageSize().GetY(), source.GetImageSize().GetY());
	QCOMPARE(destination.GetPixelFormat(), source.GetPixelFormat());
}


void CBitmapTest::CloneMeTest()
{
	iimg::CBitmap original;
	istd::CIndex2d size(75, 75);
	original.CreateBitmap(iimg::IBitmap::PF_RGB, size);
	
	auto clone = original.CloneMe();
	QVERIFY(clone.GetPtr() != nullptr);
	
	// Verify clone is a bitmap
	iimg::CBitmap* clonedBitmap = dynamic_cast<iimg::CBitmap*>(clone.GetPtr());
	QVERIFY(clonedBitmap != nullptr);
	
	// Verify properties match
	QCOMPARE(clonedBitmap->GetImageSize().GetX(), original.GetImageSize().GetX());
	QCOMPARE(clonedBitmap->GetImageSize().GetY(), original.GetImageSize().GetY());
	QCOMPARE(clonedBitmap->GetPixelFormat(), original.GetPixelFormat());
}


void CBitmapTest::GetFactoryIdTest()
{
	iimg::CBitmap bitmap;
	QByteArray factoryId = bitmap.GetFactoryId();
	
	// Factory ID should not be empty
	QVERIFY(!factoryId.isEmpty());
	
	// Should match the type name
	QCOMPARE(factoryId, iimg::CBitmap::GetTypeName());
}


void CBitmapTest::CopyImageFromTest()
{
	// Create a QImage
	QImage image(150, 100, QImage::Format_RGB32);
	image.fill(Qt::blue);
	
	iimg::CBitmap bitmap;
	bool result = bitmap.CopyImageFrom(image);
	QVERIFY(result);
	
	// Verify size
	istd::CIndex2d size = bitmap.GetImageSize();
	QCOMPARE(size.GetX(), image.width());
	QCOMPARE(size.GetY(), image.height());
}


void CBitmapTest::GetQImageTest()
{
	iimg::CBitmap bitmap;
	istd::CIndex2d size(200, 150);
	bitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, size);
	
	const QImage& image = bitmap.GetQImage();
	
	// Verify QImage properties
	QCOMPARE(image.width(), size.GetX());
	QCOMPARE(image.height(), size.GetY());
	QVERIFY(!image.isNull());
}


void CBitmapTest::cleanupTestCase()
{
	delete m_bitmapPtr;
	m_bitmapPtr = nullptr;
}


I_ADD_TEST(CBitmapTest);
