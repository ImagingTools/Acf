// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imod/CModelBase.h>
#include <imod/CSingleModelObserverBase.h>
#include <iser/CSerializableBase.h>


/**
	Test class for CModelBase functionality.
	Tests the basic model implementation including:
	- Observer attachment/detachment
	- Observer notifications (before/after changes)
	- Multiple observer management
	- Thread-safe operations
	- Change set accumulation
*/
class CModelBaseTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();

	// Observer attachment/detachment tests
	void testAttachObserver();
	void testDetachObserver();
	void testDetachAllObservers();
	void testIsAttached();
	void testGetObserverCount();
	void testGetObservers();

	// Notification tests
	void testNotifyBeforeChange();
	void testNotifyAfterChange();
	void testChangeSetAccumulation();
	void testMultipleObserversNotification();

	// Edge cases
	void testAttachSameObserverTwice();
	void testDetachUnattachedObserver();
	void testNotifyWithNoObservers();

private:
	// Helper class: Concrete model implementation for testing
	class TestModel: public imod::CModelBase, public iser::CSerializableBase
	{
	public:
		enum ChangeFlags
		{
			CF_VALUE_CHANGED = 0x1,
			CF_NAME_CHANGED = 0x2,
			CF_STATUS_CHANGED = 0x4
		};

		TestModel() : m_value(0) {}

		void SetValue(int value)
		{
			if (m_value != value)
			{
				istd::IChangeable::ChangeSet changeSet;
				changeSet.Set(CF_VALUE_CHANGED);
				NotifyBeforeChange(changeSet, false);
				m_value = value;
				NotifyAfterChange(changeSet);
			}
		}

		int GetValue() const { return m_value; }

		void SetName(const QString& name)
		{
			if (m_name != name)
			{
				istd::IChangeable::ChangeSet changeSet;
				changeSet.Set(CF_NAME_CHANGED);
				NotifyBeforeChange(changeSet, false);
				m_name = name;
				NotifyAfterChange(changeSet);
			}
		}

		QString GetName() const { return m_name; }

		void BeginGroupChanges()
		{
			istd::IChangeable::ChangeSet changeSet;
			changeSet.Set(CF_VALUE_CHANGED);
			changeSet.Set(CF_NAME_CHANGED);
			NotifyBeforeChange(changeSet, true);
		}

		void EndGroupChanges()
		{
			istd::IChangeable::ChangeSet changeSet = GetCumulatedChanges();
			NotifyAfterChange(changeSet);
		}

		// Implement required virtual methods
		virtual void OnBeginGlobalChanges() override {}
		virtual void OnEndGlobalChanges(const istd::IChangeable::ChangeSet& changeSet) override {}

		// IChangeable interface
		virtual int GetSupportedOperations() const override
		{
			return istd::IChangeable::SO_CLONE | istd::IChangeable::SO_COPY | istd::IChangeable::SO_RESET;
		}

		virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override
		{
			TestModel* clone = new TestModel();
			clone->m_value = m_value;
			clone->m_name = m_name;
			return istd::TUniqueInterfacePtr<istd::IChangeable>(clone);
		}

		virtual bool CopyFrom(const istd::IChangeable& object,
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			const TestModel* other = dynamic_cast<const TestModel*>(&object);
			if (other)
			{
				m_value = other->m_value;
				m_name = other->m_name;
				return true;
			}
			return false;
		}

		virtual bool ResetData(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			m_value = 0;
			m_name.clear();
			return true;
		}

		virtual bool Serialize(iser::IArchive& archive) override
		{
			archive.SerializeValue("value", m_value);
			archive.SerializeValue("name", m_name);
			return true;
		}

	private:
		int m_value;
		QString m_name;
	};

	// Helper class: Test observer that records notifications
	class TestObserver: public imod::CSingleModelObserverBase
	{
	public:
		TestObserver() : m_beforeUpdateCount(0), m_afterUpdateCount(0) {}

		int GetBeforeUpdateCount() const { return m_beforeUpdateCount; }
		int GetAfterUpdateCount() const { return m_afterUpdateCount; }
		const istd::IChangeable::ChangeSet& GetLastChangeSet() const { return m_lastChangeSet; }

		void Reset()
		{
			m_beforeUpdateCount = 0;
			m_afterUpdateCount = 0;
			m_lastChangeSet.Clear();
		}

		// Reimplemented from IObserver
		virtual void BeforeUpdate(imod::IModel* modelPtr) override
		{
			CSingleModelObserverBase::BeforeUpdate(modelPtr);
			m_beforeUpdateCount++;
		}

		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override
		{
			m_afterUpdateCount++;
			m_lastChangeSet = changeSet;
		}

	private:
		int m_beforeUpdateCount;
		int m_afterUpdateCount;
		istd::IChangeable::ChangeSet m_lastChangeSet;
	};

	TestModel* m_model;
	TestObserver* m_observer;
};
