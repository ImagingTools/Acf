// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


class CCumulatedProgressManagerBaseTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void DoOverflowTest1();
};


