// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
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


