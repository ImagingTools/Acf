// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>


class CTristimulusSpecificationTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void DefaultConstructorTest();
	void ParameterConstructorTest();
	void CopyConstructorTest();
	void FactoryIdTest();
	void EqualityTest();
	void CopyFromTest();
	void CloneMeTest();
	void SerializeTest();
	void GetD50TwoDegreeTest();
};
