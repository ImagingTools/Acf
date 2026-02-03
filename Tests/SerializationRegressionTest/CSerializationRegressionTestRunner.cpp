// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CSerializationRegressionTestRunner.h"

// ACF includes - i2d library
#include <i2d/CVector2d.h>
#include <i2d/CPosition2d.h>
#include <i2d/CCircle.h>
#include <i2d/CRectangle.h>
#include <i2d/CLine2d.h>

// ACF includes - i3d library
#include <i3d/CVector3d.h>
#include <i3d/CBox3d.h>
#include <i3d/CSphere.h>
#include <i3d/CPlane3d.h>

// ACF includes - icmm library
#include <icmm/CVarColor.h>
#include <icmm/CSpectrumInfo.h>

// ACF includes - imath library
#include <imath/CVarVector.h>

// ACF includes - serialization
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


void CSerializationRegressionTestRunner::initTestCase()
{
}


template<typename T>
bool CSerializationRegressionTestRunner::TestSerializationCycle(const T& original, T& restored)
{
	// Create memory buffer for serialization
	QByteArray buffer;
	
	// Serialize the original object
	{
		iser::CMemoryWriteArchive writeArchive;
		T temp = original;
		if (!temp.Serialize(writeArchive)){
			return false;
		}
		buffer = writeArchive.GetByteArray();
	}
	
	// Deserialize into the restored object
	{
		iser::CMemoryReadArchive readArchive(buffer);
		if (!restored.Serialize(readArchive)){
			return false;
		}
	}
	
	return true;
}


// i2d library tests

void CSerializationRegressionTestRunner::testVector2dSerialization()
{
	// Create original vector with specific values
	i2d::CVector2d original(123.456, 789.012);
	i2d::CVector2d restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetX(), original.GetX());
	QCOMPARE(restored.GetY(), original.GetY());
}


void CSerializationRegressionTestRunner::testPosition2dSerialization()
{
	// Create original position with specific values
	i2d::CPosition2d original;
	original.SetPosition(i2d::CVector2d(45.67, 89.01));
	
	i2d::CPosition2d restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetPosition().GetX(), original.GetPosition().GetX());
	QCOMPARE(restored.GetPosition().GetY(), original.GetPosition().GetY());
}


void CSerializationRegressionTestRunner::testCircleSerialization()
{
	// Create original circle with center and radius
	i2d::CCircle original(50.0, i2d::CVector2d(100.0, 200.0));
	i2d::CCircle restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetRadius(), original.GetRadius());
	QCOMPARE(restored.GetPosition().GetX(), original.GetPosition().GetX());
	QCOMPARE(restored.GetPosition().GetY(), original.GetPosition().GetY());
}


void CSerializationRegressionTestRunner::testRectangleSerialization()
{
	// Create original rectangle
	i2d::CRectangle original(10.0, 20.0, 100.0, 150.0);
	i2d::CRectangle restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetLeft(), original.GetLeft());
	QCOMPARE(restored.GetTop(), original.GetTop());
	QCOMPARE(restored.GetRight(), original.GetRight());
	QCOMPARE(restored.GetBottom(), original.GetBottom());
}


void CSerializationRegressionTestRunner::testLine2dSerialization()
{
	// Create original line from two points
	i2d::CLine2d original(i2d::CVector2d(0.0, 0.0), i2d::CVector2d(100.0, 100.0));
	i2d::CLine2d restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetBeginPoint().GetX(), original.GetBeginPoint().GetX());
	QCOMPARE(restored.GetBeginPoint().GetY(), original.GetBeginPoint().GetY());
	QCOMPARE(restored.GetEndPoint().GetX(), original.GetEndPoint().GetX());
	QCOMPARE(restored.GetEndPoint().GetY(), original.GetEndPoint().GetY());
}


// i3d library tests

void CSerializationRegressionTestRunner::testVector3dSerialization()
{
	// Create original vector with specific values
	i3d::CVector3d original(11.11, 22.22, 33.33);
	i3d::CVector3d restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetX(), original.GetX());
	QCOMPARE(restored.GetY(), original.GetY());
	QCOMPARE(restored.GetZ(), original.GetZ());
}


void CSerializationRegressionTestRunner::testBox3dSerialization()
{
	// Create original box with min and max points
	i3d::CBox3d original(i3d::CVector3d(0.0, 0.0, 0.0), i3d::CVector3d(10.0, 20.0, 30.0));
	i3d::CBox3d restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetMin().GetX(), original.GetMin().GetX());
	QCOMPARE(restored.GetMin().GetY(), original.GetMin().GetY());
	QCOMPARE(restored.GetMin().GetZ(), original.GetMin().GetZ());
	QCOMPARE(restored.GetMax().GetX(), original.GetMax().GetX());
	QCOMPARE(restored.GetMax().GetY(), original.GetMax().GetY());
	QCOMPARE(restored.GetMax().GetZ(), original.GetMax().GetZ());
}


void CSerializationRegressionTestRunner::testSphereSerialization()
{
	// Create original sphere with center and radius
	i3d::CSphere original(i3d::CVector3d(5.0, 10.0, 15.0), 25.0);
	i3d::CSphere restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetCenter().GetX(), original.GetCenter().GetX());
	QCOMPARE(restored.GetCenter().GetY(), original.GetCenter().GetY());
	QCOMPARE(restored.GetCenter().GetZ(), original.GetCenter().GetZ());
	QCOMPARE(restored.GetRadius(), original.GetRadius());
}


void CSerializationRegressionTestRunner::testPlane3dSerialization()
{
	// Create original plane with normal and distance
	i3d::CPlane3d original(i3d::CVector3d(0.0, 0.0, 1.0), 10.0);
	i3d::CPlane3d restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetNormal().GetX(), original.GetNormal().GetX());
	QCOMPARE(restored.GetNormal().GetY(), original.GetNormal().GetY());
	QCOMPARE(restored.GetNormal().GetZ(), original.GetNormal().GetZ());
	QCOMPARE(restored.GetDistance(), original.GetDistance());
}


// icmm library tests

void CSerializationRegressionTestRunner::testVarColorSerialization()
{
	// Create original color with 4 components (RGBA)
	icmm::CVarColor original(4);
	original.SetElement(0, 0.5);   // R
	original.SetElement(1, 0.75);  // G
	original.SetElement(2, 0.25);  // B
	original.SetElement(3, 1.0);   // A
	
	icmm::CVarColor restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetElementsCount(), original.GetElementsCount());
	for (int i = 0; i < original.GetElementsCount(); ++i){
		QCOMPARE(restored.GetElement(i), original.GetElement(i));
	}
}


void CSerializationRegressionTestRunner::testSpectrumInfoSerialization()
{
	// Create original spectrum info with range and step
	icmm::CSpectrumInfo original(istd::CIntRange(400, 700), 10);
	icmm::CSpectrumInfo restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetSpectralRange().GetMin(), original.GetSpectralRange().GetMin());
	QCOMPARE(restored.GetSpectralRange().GetMax(), original.GetSpectralRange().GetMax());
	QCOMPARE(restored.GetStep(), original.GetStep());
	QCOMPARE(restored.GetSamplesCount(), original.GetSamplesCount());
}


// imath library tests

void CSerializationRegressionTestRunner::testVarVectorSerialization()
{
	// Create original vector with 5 elements
	imath::CVarVector original(5);
	original.SetElement(0, 1.1);
	original.SetElement(1, 2.2);
	original.SetElement(2, 3.3);
	original.SetElement(3, 4.4);
	original.SetElement(4, 5.5);
	
	imath::CVarVector restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetElementsCount(), original.GetElementsCount());
	for (int i = 0; i < original.GetElementsCount(); ++i){
		QCOMPARE(restored.GetElement(i), original.GetElement(i));
	}
}


void CSerializationRegressionTestRunner::cleanupTestCase()
{
}


I_ADD_TEST(CSerializationRegressionTestRunner);
