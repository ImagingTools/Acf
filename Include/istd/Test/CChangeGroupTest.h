// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CChangeGroup.h>
#include <istd/IChangeable.h>
#include <itest/CStandardTestExecutor.h>

// Mock changeable class for testing
class MockChangeableForGroup : public istd::IChangeable
{
public:
	int beginChangeGroupCount = 0;
	int endChangeGroupCount = 0;
	int endChangesCount = 0;
	istd::IChangeable::ChangeSet lastBeginChangeSet;
	istd::IChangeable::ChangeSet lastEndChangeSet;
	istd::IChangeable::ChangeSet lastEndChangesSet;

	virtual void OnBeginChanges() override {}
	virtual void OnEndChanges(const ChangeSet& changeSet) override
	{
		endChangesCount++;
		lastEndChangesSet = changeSet;
	}

	virtual void BeginChangeGroup(const ChangeSet& changeSet) override
	{
		beginChangeGroupCount++;
		lastBeginChangeSet = changeSet;
		IChangeable::BeginChangeGroup(changeSet);
	}

	virtual void EndChangeGroup(const ChangeSet& changeSet) override
	{
		endChangeGroupCount++;
		lastEndChangeSet = changeSet;
		IChangeable::EndChangeGroup(changeSet);
	}
};

class CChangeGroupTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorDestructorTest();
	void IsValidTest();
	void ResetTest();
	void NullChangeableTest();

	void cleanupTestCase();
};


