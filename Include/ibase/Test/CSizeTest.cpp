// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/Test/CSizeTest.h>


// ACF includes
#include <ibase/CSize.h>
#include <iser/CMemoryArchive.h>


void CSizeTest::initTestCase()
{
	// Setup
}


void CSizeTest::cleanupTestCase()
{
	// Cleanup
}


// Constructor tests

void CSizeTest::testDefaultConstructor()
{
	ibase::CSize size;
	
	// Default constructor should initialize to 0, 0
	QCOMPARE(size.GetX(), 0);
	QCOMPARE(size.GetY(), 0);
}


void CSizeTest::testParameterizedConstructor()
{
	ibase::CSize size(100, 200);
	
	QCOMPARE(size.GetX(), 100);
	QCOMPARE(size.GetY(), 200);
}


void CSizeTest::testIndex2dConstructor()
{
	istd::CIndex2d index(50, 75);
	ibase::CSize size(index);
	
	QCOMPARE(size.GetX(), 50);
	QCOMPARE(size.GetY(), 75);
}


void CSizeTest::testQPointConstructor()
{
	QPoint point(30, 40);
	ibase::CSize size(point);
	
	QCOMPARE(size.GetX(), 30);
	QCOMPARE(size.GetY(), 40);
}


void CSizeTest::testQSizeConstructor()
{
	QSize qSize(80, 90);
	ibase::CSize size(qSize);
	
	QCOMPARE(size.GetX(), 80);
	QCOMPARE(size.GetY(), 90);
}


// Operator tests

void CSizeTest::testAdditionOperator()
{
	ibase::CSize size1(10, 20);
	ibase::CSize size2(5, 15);
	
	ibase::CSize result = size1 + size2;
	
	QCOMPARE(result.GetX(), 15);
	QCOMPARE(result.GetY(), 35);
	
	// Verify original sizes are unchanged
	QCOMPARE(size1.GetX(), 10);
	QCOMPARE(size1.GetY(), 20);
}


void CSizeTest::testSubtractionOperator()
{
	ibase::CSize size1(20, 30);
	ibase::CSize size2(5, 10);
	
	ibase::CSize result = size1 - size2;
	
	QCOMPARE(result.GetX(), 15);
	QCOMPARE(result.GetY(), 20);
	
	// Verify original sizes are unchanged
	QCOMPARE(size1.GetX(), 20);
	QCOMPARE(size1.GetY(), 30);
}


void CSizeTest::testPlusEqualsOperator()
{
	ibase::CSize size1(10, 20);
	ibase::CSize size2(5, 15);
	
	size1 += size2;
	
	QCOMPARE(size1.GetX(), 15);
	QCOMPARE(size1.GetY(), 35);
}


void CSizeTest::testMinusEqualsOperator()
{
	ibase::CSize size1(20, 30);
	ibase::CSize size2(5, 10);
	
	size1 -= size2;
	
	QCOMPARE(size1.GetX(), 15);
	QCOMPARE(size1.GetY(), 20);
}


// Method tests

void CSizeTest::testIsNull()
{
	// Size with both dimensions zero should be null
	ibase::CSize size1(0, 0);
	QVERIFY(size1.IsNull());
	
	// Size with width zero should be null
	ibase::CSize size2(0, 10);
	QVERIFY(size2.IsNull());
	
	// Size with height zero should be null
	ibase::CSize size3(10, 0);
	QVERIFY(size3.IsNull());
	
	// Size with both dimensions non-zero should not be null
	ibase::CSize size4(10, 20);
	QVERIFY(!size4.IsNull());
}


void CSizeTest::testSerialization()
{
	// Create a size to serialize
	ibase::CSize originalSize(100, 200);
	
	// Serialize to memory
	iser::CMemoryArchive archiveWrite;
	archiveWrite.BeginStoring();
	QVERIFY(originalSize.Serialize(archiveWrite));
	archiveWrite.EndStoring();
	
	// Deserialize from memory
	ibase::CSize deserializedSize;
	iser::CMemoryArchive archiveRead(archiveWrite.GetData());
	archiveRead.BeginRestoring();
	QVERIFY(deserializedSize.Serialize(archiveRead));
	archiveRead.EndRestoring();
	
	// Verify the deserialized size matches the original
	QCOMPARE(deserializedSize.GetX(), 100);
	QCOMPARE(deserializedSize.GetY(), 200);
}
