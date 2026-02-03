// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <iser/ISerializable.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>

/**
	Cross-library component tests for regression of data model serialization.
	
	This test suite validates serialization and deserialization functionality
	for data models from different ACF libraries (i2d, i3d, icmm, imath, iprm, ifile, iimg).
	It ensures that objects can be correctly saved to and restored from archives.
*/
class CSerializationRegressionTestRunner: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();

	// i2d library tests
	void testVector2dSerialization();
	void testPosition2dSerialization();
	void testCircleSerialization();
	void testRectangleSerialization();
	void testLine2dSerialization();

	// i3d library tests
	void testVector3dSerialization();
	void testBox3dSerialization();
	void testSphereSerialization();
	void testPlane3dSerialization();

	// icmm library tests
	void testVarColorSerialization();
	void testSpectrumInfoSerialization();

	// imath library tests
	void testVarVectorSerialization();

	// iprm library tests
	void testIdParamSerialization();
	void testNameParamSerialization();
	void testTextParamSerialization();
	void testEnableableParamSerialization();
	void testSelectionParamSerialization();

	// ifile library tests
	void testFileNameParamSerialization();

	// iimg library tests
	void testScanlineMaskSerialization();

	// Complex cross-library scenarios with dependency injection
	void testComplexScenarioWithMultipleParams();
	void testComplexScenarioWithGeometryAndColor();
	void testComplexScenarioWithNestedSelections();

	// Backward compatibility and versioning tests
	void testBackwardCompatibilityVector2d();
	void testBackwardCompatibilityCircle();
	void testVersionManagement();

	void cleanupTestCase();

private:
	/**
		Helper template to test serialization/deserialization cycle for any serializable object.
		
		\param original The original object to serialize
		\param restored The object to deserialize into
		\return true if serialization round-trip was successful
	*/
	template<typename T>
	bool TestSerializationCycle(const T& original, T& restored);

	/**
		Helper to save reference data to file for backward compatibility testing.
		
		\param object The object to serialize
		\param filename The filename to save to
		\return true if save was successful
	*/
	template<typename T>
	bool SaveReferenceData(const T& object, const QString& filename);

	/**
		Helper to load and verify reference data from file for backward compatibility testing.
		
		\param restored The object to deserialize into
		\param filename The filename to load from
		\return true if load was successful
	*/
	template<typename T>
	bool LoadReferenceData(T& restored, const QString& filename);

	/**
		Get the path to the reference data directory.
	*/
	QString GetReferenceDataPath() const;
};

