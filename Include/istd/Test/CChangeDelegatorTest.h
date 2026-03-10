// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CChangeDelegator.h>
#include <istd/IChangeable.h>
#include <itest/CStandardTestExecutor.h>

// Mock changeable class for testing
class MockChangeable : public istd::IChangeable
{
public:
	int beginChangesCount = 0;
	int endChangesCount = 0;
	istd::IChangeable::ChangeSet lastChangeSet;

	virtual void OnBeginChanges() override
	{
		beginChangesCount++;
	}

	virtual void OnEndChanges(const ChangeSet& changeSet) override
	{
		endChangesCount++;
		lastChangeSet = changeSet;
	}
};

class CChangeDelegatorTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void DefaultConstructorTest();
	void ConstructorWithSlaveTest();
	void SetGetSlavePtrTest();
	void DelegateBeginChangesTest();
	void DelegateEndChangesTest();

	void cleanupTestCase();
};


