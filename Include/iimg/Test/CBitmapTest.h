// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iimg/CBitmap.h>
#include <itest/CStandardTestExecutor.h>

class CBitmapTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void CopyConstructorTest();
	void QImageConstructorTest();
	void CreateBitmapTest();
	void CreateBitmapGrayTest();
	void CreateBitmapRGBTest();
	void CreateBitmapRGBATest();
	void GetPixelFormatTest();
	void IsFormatSupportedTest();
	void GetImageSizeTest();
	void ResetImageTest();
	void ClearImageTest();
	void GetLinesPtrTest();
	void GetLinesDifferenceTest();
	void CopyFromTest();
	void CloneMeTest();
	void GetFactoryIdTest();
	void CopyImageFromTest();
	void GetQImageTest();

	void cleanupTestCase();

private:
	iimg::CBitmap* m_bitmapPtr = nullptr;
};
