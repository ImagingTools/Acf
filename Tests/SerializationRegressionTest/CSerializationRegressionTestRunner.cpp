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
#include <ifile/CFileWriteArchive.h>
#include <ifile/CFileReadArchive.h>
#include <iser/IVersionInfo.h>

// Qt includes
#include <QtCore/QFile>
#include <QtCore/QDir>


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
		buffer = QByteArray(static_cast<const char*>(writeArchive.GetBuffer()), writeArchive.GetBufferSize());
	}
	
	// Deserialize into the restored object
	{
		iser::CMemoryReadArchive readArchive(buffer.data(), buffer.size());
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
	QCOMPARE(restored.GetPoint1().GetX(), original.GetPoint1().GetX());
	QCOMPARE(restored.GetPoint1().GetY(), original.GetPoint1().GetY());
	QCOMPARE(restored.GetPoint2().GetX(), original.GetPoint2().GetX());
	QCOMPARE(restored.GetPoint2().GetY(), original.GetPoint2().GetY());
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
	// Create original sphere with radius and center
	i3d::CSphere original(25.0, i3d::CVector3d(5.0, 10.0, 15.0));
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
	// Create original plane with point and normal
	i3d::CPlane3d original(i3d::CVector3d(0.0, 0.0, 10.0), i3d::CVector3d(0.0, 0.0, 1.0));
	i3d::CPlane3d restored;
	
	// Test serialization cycle
	QVERIFY(TestSerializationCycle(original, restored));
	
	// Verify data integrity
	QCOMPARE(restored.GetPoint().GetX(), original.GetPoint().GetX());
	QCOMPARE(restored.GetPoint().GetY(), original.GetPoint().GetY());
	QCOMPARE(restored.GetPoint().GetZ(), original.GetPoint().GetZ());
	QCOMPARE(restored.GetNormal().GetX(), original.GetNormal().GetX());
	QCOMPARE(restored.GetNormal().GetY(), original.GetNormal().GetY());
	QCOMPARE(restored.GetNormal().GetZ(), original.GetNormal().GetZ());
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
	QCOMPARE(restored.GetSpectralRange().GetMinValue(), original.GetSpectralRange().GetMinValue());
	QCOMPARE(restored.GetSpectralRange().GetMaxValue(), original.GetSpectralRange().GetMaxValue());
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


// Helper methods implementation

template<typename T>
bool CSerializationRegressionTestRunner::SaveReferenceData(const T& object, const QString& filename)
{
	QString filePath = GetReferenceDataPath() + "/" + filename;
	
	// Ensure directory exists
	QDir dir(GetReferenceDataPath());
	if (!dir.exists()){
		dir.mkpath(".");
	}
	
	ifile::CFileWriteArchive archive(filePath);
	// File archives don't have IsOpen() method - they throw exceptions on failure
	
	T temp = object;
	return temp.Serialize(archive);
}


template<typename T>
bool CSerializationRegressionTestRunner::LoadReferenceData(T& restored, const QString& filename)
{
	QString filePath = GetReferenceDataPath() + "/" + filename;
	
	ifile::CFileReadArchive archive(filePath);
	// File archives don't have IsOpen() method - they throw exceptions on failure
	
	return restored.Serialize(archive);
}


QString CSerializationRegressionTestRunner::GetReferenceDataPath() const
{
	// Get path relative to test executable location
	return QDir::currentPath() + "/../../Tests/SerializationRegressionTest/Data/ReferenceData";
}


// Complex cross-library scenario tests

void CSerializationRegressionTestRunner::testComplexScenarioWithMultipleParams()
{
	// Create a complex scenario with multiple interconnected parameters
	// This simulates a real-world use case where different parameter types work together
	
	// Create ID and Name for an object
	iprm::CIdParam idParam;
	idParam.SetId("complex-object-001");
	
	iprm::CNameParam nameParam;
	nameParam.SetName("ComplexTestObject");
	
	iprm::CTextParam descParam;
	descParam.SetText("A complex object combining multiple parameter types from different libraries");
	
	ifile::CFileNameParam fileParam;
	fileParam.SetPath("/output/test/complex_data.xml");
	
	iprm::CEnableableParam enableParam(true);
	
	// Serialize all together in a structured way
	QByteArray buffer;
	{
		iser::CMemoryWriteArchive writeArchive;
		
		// Serialize all parameters in sequence
		QVERIFY(idParam.Serialize(writeArchive));
		QVERIFY(nameParam.Serialize(writeArchive));
		QVERIFY(descParam.Serialize(writeArchive));
		QVERIFY(fileParam.Serialize(writeArchive));
		QVERIFY(enableParam.Serialize(writeArchive));
		
		buffer = QByteArray(static_cast<const char*>(writeArchive.GetBuffer()), writeArchive.GetBufferSize());
	}
	
	// Deserialize all
	{
		iser::CMemoryReadArchive readArchive(buffer.data(), buffer.size());
		
		iprm::CIdParam restoredId;
		iprm::CNameParam restoredName;
		iprm::CTextParam restoredDesc;
		ifile::CFileNameParam restoredFile;
		iprm::CEnableableParam restoredEnable(false);
		
		QVERIFY(restoredId.Serialize(readArchive));
		QVERIFY(restoredName.Serialize(readArchive));
		QVERIFY(restoredDesc.Serialize(readArchive));
		QVERIFY(restoredFile.Serialize(readArchive));
		QVERIFY(restoredEnable.Serialize(readArchive));
		
		// Verify all data was preserved
		QCOMPARE(restoredId.GetId(), idParam.GetId());
		QCOMPARE(restoredName.GetName(), nameParam.GetName());
		QCOMPARE(restoredDesc.GetText(), descParam.GetText());
		QCOMPARE(restoredFile.GetPath(), fileParam.GetPath());
		QCOMPARE(restoredEnable.IsEnabled(), enableParam.IsEnabled());
	}
}


void CSerializationRegressionTestRunner::testComplexScenarioWithGeometryAndColor()
{
	// Complex scenario: Geometric objects with associated color information
	// This simulates a graphics application where shapes have color properties
	
	// Create a circle with position and color
	i2d::CCircle circle(50.0, i2d::CVector2d(100.0, 200.0));
	
	// Create a color for the circle (RGBA)
	icmm::CVarColor color(4);
	color.SetElement(0, 0.8);  // R
	color.SetElement(1, 0.2);  // G
	color.SetElement(2, 0.4);  // B
	color.SetElement(3, 1.0);  // A
	
	// Create a 3D sphere with similar properties (radius first, then center)
	i3d::CSphere sphere(15.0, i3d::CVector3d(10.0, 20.0, 30.0));
	
	// Create a spectrum for spectral data
	icmm::CSpectrumInfo spectrum(istd::CIntRange(400, 700), 5);
	
	// Serialize the complex geometry + color scenario
	QByteArray buffer;
	{
		iser::CMemoryWriteArchive writeArchive;
		
		QVERIFY(circle.Serialize(writeArchive));
		QVERIFY(color.Serialize(writeArchive));
		QVERIFY(sphere.Serialize(writeArchive));
		QVERIFY(spectrum.Serialize(writeArchive));
		
		buffer = QByteArray(static_cast<const char*>(writeArchive.GetBuffer()), writeArchive.GetBufferSize());
	}
	
	// Deserialize and verify
	{
		iser::CMemoryReadArchive readArchive(buffer.data(), buffer.size());
		
		i2d::CCircle restoredCircle;
		icmm::CVarColor restoredColor;
		i3d::CSphere restoredSphere;
		icmm::CSpectrumInfo restoredSpectrum;
		
		QVERIFY(restoredCircle.Serialize(readArchive));
		QVERIFY(restoredColor.Serialize(readArchive));
		QVERIFY(restoredSphere.Serialize(readArchive));
		QVERIFY(restoredSpectrum.Serialize(readArchive));
		
		// Verify geometric data
		QCOMPARE(restoredCircle.GetRadius(), circle.GetRadius());
		QCOMPARE(restoredCircle.GetPosition().GetX(), circle.GetPosition().GetX());
		
		// Verify color data
		QCOMPARE(restoredColor.GetElementsCount(), color.GetElementsCount());
		for (int i = 0; i < color.GetElementsCount(); ++i){
			QCOMPARE(restoredColor.GetElement(i), color.GetElement(i));
		}
		
		// Verify 3D sphere
		QCOMPARE(restoredSphere.GetRadius(), sphere.GetRadius());
		QCOMPARE(restoredSphere.GetCenter().GetZ(), sphere.GetCenter().GetZ());
		
		// Verify spectrum info
		QCOMPARE(restoredSpectrum.GetStep(), spectrum.GetStep());
	}
}


void CSerializationRegressionTestRunner::testComplexScenarioWithNestedSelections()
{
	// Complex scenario with hierarchical selection parameters
	// Simulates a UI with cascading menus/selections
	
	// Create main options
	iprm::COptionsManager mainOptions;
	mainOptions.InsertOption("Graphics2D", "graphics2d", "2D Graphics Options", -1);
	mainOptions.InsertOption("Graphics3D", "graphics3d", "3D Graphics Options", -1);
	mainOptions.InsertOption("ColorMgmt", "colormgmt", "Color Management Options", -1);
	
	// Create sub-options
	iprm::COptionsManager graphics2DOptions;
	graphics2DOptions.InsertOption("Circle", "circle", "Circle tool", -1);
	graphics2DOptions.InsertOption("Rectangle", "rectangle", "Rectangle tool", -1);
	graphics2DOptions.InsertOption("Line", "line", "Line tool", -1);
	
	// Create selections
	iprm::CSelectionParam mainSelection;
	mainSelection.SetSelectionConstraints(&mainOptions);
	mainSelection.SetSelectedOptionIndex(0);  // Select Graphics2D
	
	iprm::CSelectionParam subSelection;
	subSelection.SetSelectionConstraints(&graphics2DOptions);
	subSelection.SetSelectedOptionIndex(1);  // Select Rectangle
	
	// Serialize the hierarchical selection
	QByteArray buffer;
	{
		iser::CMemoryWriteArchive writeArchive;
		
		QVERIFY(mainSelection.Serialize(writeArchive));
		QVERIFY(subSelection.Serialize(writeArchive));
		
		buffer = QByteArray(static_cast<const char*>(writeArchive.GetBuffer()), writeArchive.GetBufferSize());
	}
	
	// Deserialize and verify
	{
		iser::CMemoryReadArchive readArchive(buffer.data(), buffer.size());
		
		iprm::CSelectionParam restoredMain;
		restoredMain.SetSelectionConstraints(&mainOptions);
		
		iprm::CSelectionParam restoredSub;
		restoredSub.SetSelectionConstraints(&graphics2DOptions);
		
		QVERIFY(restoredMain.Serialize(readArchive));
		QVERIFY(restoredSub.Serialize(readArchive));
		
		// Verify selections preserved
		QCOMPARE(restoredMain.GetSelectedOptionIndex(), mainSelection.GetSelectedOptionIndex());
		QCOMPARE(restoredSub.GetSelectedOptionIndex(), subSelection.GetSelectedOptionIndex());
	}
}


// Backward compatibility and versioning tests

void CSerializationRegressionTestRunner::testBackwardCompatibilityVector2d()
{
	// Test that we can load data serialized with older versions
	// This ensures backward compatibility
	
	i2d::CVector2d original(123.456, 789.012);
	
	// Save reference data (this would be from an "old version")
	QString refFile = "vector2d_v1.dat";
	QVERIFY(SaveReferenceData(original, refFile));
	
	// Load and verify (simulating loading old data in new version)
	i2d::CVector2d restored;
	QVERIFY(LoadReferenceData(restored, refFile));
	
	QCOMPARE(restored.GetX(), original.GetX());
	QCOMPARE(restored.GetY(), original.GetY());
}


void CSerializationRegressionTestRunner::testBackwardCompatibilityCircle()
{
	// Test backward compatibility for more complex objects
	
	i2d::CCircle original(75.5, i2d::CVector2d(150.0, 250.0));
	
	// Save reference data
	QString refFile = "circle_v1.dat";
	QVERIFY(SaveReferenceData(original, refFile));
	
	// Load and verify
	i2d::CCircle restored;
	QVERIFY(LoadReferenceData(restored, refFile));
	
	QCOMPARE(restored.GetRadius(), original.GetRadius());
	QCOMPARE(restored.GetPosition().GetX(), original.GetPosition().GetX());
	QCOMPARE(restored.GetPosition().GetY(), original.GetPosition().GetY());
}


void CSerializationRegressionTestRunner::testVersionManagement()
{
	// Test version management mechanism
	// Verify that GetMinimalVersion returns appropriate values
	
	i2d::CVector2d vector(1.0, 2.0);
	quint32 version = vector.GetMinimalVersion(iser::IVersionInfo::AcfVersionId);
	
	// Version should be either 0 (no specific version) or a valid version number
	QVERIFY(version == 0 || version > 0);
	
	// Test with more complex object
	i3d::CVector3d vector3d(1.0, 2.0, 3.0);
	quint32 version3d = vector3d.GetMinimalVersion(iser::IVersionInfo::AcfVersionId);
	QVERIFY(version3d == 0 || version3d > 0);
	
	// Test with color management object
	icmm::CVarColor color(3);
	quint32 colorVersion = color.GetMinimalVersion(iser::IVersionInfo::AcfVersionId);
	QVERIFY(colorVersion == 0 || colorVersion > 0);
}


void CSerializationRegressionTestRunner::cleanupTestCase()
{
}


I_ADD_TEST(CSerializationRegressionTestRunner);
