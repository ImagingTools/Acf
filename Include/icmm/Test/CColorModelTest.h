// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>


class CColorModelTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void RgbColorModelTest();
	void RgbaColorModelTest();
	void CieLabColorModelTest();
	void CmyColorModelTest();
	void CmykColorModelTest();
	void SpectralColorModelTest();
	void CreateColorTransformationTest();
};
