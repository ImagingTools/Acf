// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iimg/CScanlineMask.h>
#include <i2d/CRect.h>
#include <i2d/CCircle.h>
#include <i2d/CRectangle.h>
#include <itest/CStandardTestExecutor.h>

class CScanlineMaskTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void IsBitmapRegionEmptyTest();
	void GetBoundingRectTest();
	void CreateFilledTest();
	void CreateFromCircleTest();
	void CreateFromRectangleTest();
	void ResetScanlinesTest();
	void GetPixelRangesTest();
	void GetUnionTest();
	void GetIntersectionTest();
	void TranslateTest();
	void InvertTest();
	void DilateTest();
	void ErodeTest();
	void EqualityOperatorTest();
	void InequalityOperatorTest();

	void cleanupTestCase();

private:
	iimg::CScanlineMask* m_maskPtr = nullptr;
};
