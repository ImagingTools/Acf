// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <ibase/CListObserver.h>


/**
	Test implementation of CListObserver for testing purposes.
*/
class CTestListObserver : public ibase::CListObserver
{
public:
	CTestListObserver()
		: m_resetCalled(false)
		, m_lastRemovedIndex(-1)
		, m_lastAddedIndex(-1)
		, m_lastUpdatedIndex(-1)
	{
	}
	
	void Reset()
	{
		m_resetCalled = false;
		m_lastRemovedIndex = -1;
		m_lastAddedIndex = -1;
		m_lastUpdatedIndex = -1;
	}
	
	bool m_resetCalled;
	qsizetype m_lastRemovedIndex;
	qsizetype m_lastAddedIndex;
	qsizetype m_lastUpdatedIndex;

protected:
	virtual void OnListReset() override
	{
		m_resetCalled = true;
	}
	
	virtual void OnBeforeElementRemoved(qsizetype index) override
	{
		m_lastRemovedIndex = index;
	}
	
	virtual void OnAfterElementAdded(qsizetype index) override
	{
		m_lastAddedIndex = index;
	}
	
	virtual void OnAfterElementUpdated(qsizetype index) override
	{
		m_lastUpdatedIndex = index;
	}
};


/**
	Unit tests for CListObserver class.
*/
class CListObserverTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	
	// Routing tests
	void testOnUpdateWithReset();
	void testOnUpdateWithElementAdded();
	void testOnUpdateWithElementRemoved();
	void testOnUpdateWithElementUpdated();
	void testMultipleNotifications();
	
	void cleanupTestCase();
};


I_ADD_TEST(CListObserverTest)
