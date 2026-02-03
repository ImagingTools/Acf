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

// ACF includes - iprm library
#include <iprm/CIdParam.h>
#include <iprm/CNameParam.h>
#include <iprm/CTextParam.h>
#include <iprm/CEnableableParam.h>
#include <iprm/CSelectionParam.h>
#include <iprm/COptionsManager.h>

// ACF includes - ifile library
#include <ifile/CFileNameParam.h>

// ACF includes - iimg library
#include <iimg/CScanlineMask.h>

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


// iprm library tests

void CSerializationRegressionTestRunner::testIdParamSerialization()
{
	// Create original ID parameter with specific value
	iprm::CIdParam original;
	original.SetId("test-object-id-12345");
	
	iprm::CIdParam restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetId(), original.GetId());
}


void CSerializationRegressionTestRunner::testNameParamSerialization()
{
	// Create original name parameter with specific value
	iprm::CNameParam original;
	original.SetName("TestObjectName");
	
	iprm::CNameParam restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetName(), original.GetName());
}


void CSerializationRegressionTestRunner::testTextParamSerialization()
{
	// Create original text parameter with specific value
	iprm::CTextParam original;
	original.SetText("This is a test text parameter with special characters: äöü ß €");
	
	iprm::CTextParam restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetText(), original.GetText());
}


void CSerializationRegressionTestRunner::testEnableableParamSerialization()
{
	// Create original enableable parameter (enabled)
	iprm::CEnableableParam original(true);
	
	iprm::CEnableableParam restored(false);
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.IsEnabled(), original.IsEnabled());
	QVERIFY(restored.IsEnabled());
}


void CSerializationRegressionTestRunner::testSelectionParamSerialization()
{
	// Create options for selection
	iprm::COptionsManager options;
	options.AddOption("Option1", "First option");
	options.AddOption("Option2", "Second option");
	options.AddOption("Option3", "Third option");
	
	// Create original selection parameter with specific selection
	iprm::CSelectionParam original;
	original.SetSelectionConstraints(&options);
	original.SetSelectedOptionIndex(1);  // Select "Option2"
	
	iprm::CSelectionParam restored;
	restored.SetSelectionConstraints(&options);  // Must set constraints before deserializing
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetSelectedOptionIndex(), original.GetSelectedOptionIndex());
	QCOMPARE(restored.GetSelectedOptionIndex(), 1);
}


// ifile library tests

void CSerializationRegressionTestRunner::testFileNameParamSerialization()
{
	// Create original filename parameter with specific path
	ifile::CFileNameParam original;
	original.SetPath("/path/to/test/file.txt");
	
	ifile::CFileNameParam restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetPath(), original.GetPath());
}


// iimg library tests

void CSerializationRegressionTestRunner::testScanlineMaskSerialization()
{
	// Create original scanline mask with specific geometry
	iimg::CScanlineMask original;
	// Add a simple rectangle region to the mask
	original.AddRectangle(i2d::CRectangle(10.0, 20.0, 100.0, 150.0));
	
	iimg::CScanlineMask restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity - compare bounding boxes
	QCOMPARE(restored.GetBoundingBox().GetLeft(), original.GetBoundingBox().GetLeft());
	QCOMPARE(restored.GetBoundingBox().GetTop(), original.GetBoundingBox().GetTop());
	QCOMPARE(restored.GetBoundingBox().GetRight(), original.GetBoundingBox().GetRight());
	QCOMPARE(restored.GetBoundingBox().GetBottom(), original.GetBoundingBox().GetBottom());
}


void CSerializationRegressionTestRunner::cleanupTestCase()
{
}


I_ADD_TEST(CSerializationRegressionTestRunner);
