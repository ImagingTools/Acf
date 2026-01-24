// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


class CComponentSimulationTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();

	void DoSimiulationTest();
};


