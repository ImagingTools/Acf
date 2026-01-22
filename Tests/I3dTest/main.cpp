// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CI3dTest.h"


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtTest/QtTest>


int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
	CI3dTest testInstance;
	return QTest::qExec(&testInstance, argc, argv);
}
