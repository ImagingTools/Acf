#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>


class CGraphData2dTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	void testConstruction();
	void testAddCurve();
	void testRemoveCurve();
	void testClearCurves();
	void testAxisLabels();
	void testAxisRanges();
	void testLegendVisibility();
	void testGridVisibility();
	void testBoundingBox();
	void cleanupTestCase();
};


