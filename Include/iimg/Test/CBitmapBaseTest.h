// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iimg/CBitmap.h>  // Using CBitmap as concrete implementation of CBitmapBase
#include <itest/CStandardTestExecutor.h>

class CBitmapBaseTest: public QObject
{
	Q_OBJECT
private slots:
	void GetCenterTest();
	void MoveCenterToTest();
	void GetBoundingBoxTest();
	void IsEmptyTest();
	void GetLineBytesCountTest();
	void GetComponentBitsCountTest();
	void GetPixelBitsCountTest();
	void GetComponentsCountTest();
	void CreateImageFromRegionTest();
	void GetColorAtTest();
	void SetColorAtTest();
	void GetColorModelTest();
};
