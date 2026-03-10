// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/IChangeable.h>
#include <itest/CStandardTestExecutor.h>

// Mock changeable class for testing
class MockChangeableForNotifier : public istd::IChangeable
{
public:
	int beginChangesCount = 0;
	int endChangesCount = 0;
	istd::IChangeable::ChangeSet lastBeginChangeSet;
	istd::IChangeable::ChangeSet lastEndChangeSet;

	virtual void OnBeginChanges() override
	{
		beginChangesCount++;
	}

	virtual void OnEndChanges(const ChangeSet& changeSet) override
	{
		endChangesCount++;
		lastEndChangeSet = changeSet;
	}

	virtual void BeginChanges(const ChangeSet& changeSet) override
	{
		lastBeginChangeSet = changeSet;
		IChangeable::BeginChanges(changeSet);
	}

	virtual void EndChanges(const ChangeSet& changeSet) override
	{
		IChangeable::EndChanges(changeSet);
	}
};

class CChangeNotifierTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorDestructorTest();
	void IsValidTest();
	void ResetTest();
	void AbortTest();
	void NullChangeableTest();

	void cleanupTestCase();
};


