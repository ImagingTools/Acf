#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <i3d/i3d.h>


/**
	Unit tests for i3d library primitives and geometric algorithms.
*/
class CI3dTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	
	// Vector tests
	void testVector3dBasics();
	void testVector3dOperations();
	
	// Plane tests
	void testPlane3dConstruction();
	void testPlane3dDistance();
	void testPlane3dProjection();
	
	// Line tests
	void testLine3dBasics();
	void testLine3dDistance();
	void testLine3dClosestPoint();
	
	// Sphere tests
	void testSphereBasics();
	void testSphereIntersection();
	
	// Box tests
	void testBox3dBasics();
	void testBox3dIntersection();
	
	// Quaternion tests
	void testQuaternionRotation();
	void testQuaternionSlerp();
	
	// Affine transform tests
	void testAffine3dBasics();
	void testAffine3dComposition();
	
	// Geometry utility tests
	void testAngleCalculations();
	void testLinePlaneIntersection();
	void testBarycentricCoordinates();
	
	void cleanupTestCase();
};
