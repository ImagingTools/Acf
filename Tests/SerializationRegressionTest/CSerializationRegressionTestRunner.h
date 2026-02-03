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
};

