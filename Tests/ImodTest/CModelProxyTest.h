// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imod/CModelProxy.h>
#include <imod/TModelWrap.h>
#include <imod/CSingleModelObserverBase.h>
#include <iser/CSerializableBase.h>


/**
	Test class for CModelProxy functionality.
	Tests the model proxy pattern including:
	- Proxy observer attachment
	- Model switching behind proxy
	- Notification forwarding
	- Observer persistence across model switches
*/
class CModelProxyTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();

	// Basic functionality tests
	void testProxyCreation();
	void testSetModelPtr();
	void testResetModel();
	void testObserverAttachment();

	// Model switching tests
	void testModelSwitching();
	void testNotificationForwarding();
	void testObserverPersistenceAcrossSwitches();
	void testDetachObserverFromProxy();

	// Edge cases
	void testSwitchToNullModel();
	void testMultipleObservers();
	void testResetWithObservers();

private:
	// Helper class: Simple data model
	class TestDataModel: public iser::CSerializableBase
	{
	public:
		enum ChangeFlags
		{
			CF_VALUE_CHANGED = 0x1
		};

		TestDataModel(int initialValue = 0) : m_value(initialValue) {}

		void SetValue(int value)
		{
			if (m_value != value)
			{
				istd::IChangeable::ChangeSet changeSet;
				changeSet.Set(CF_VALUE_CHANGED);
				BeginChanges(changeSet);
				m_value = value;
				EndChanges(changeSet);
			}
		}

		int GetValue() const { return m_value; }

		// IChangeable interface
		virtual int GetSupportedOperations() const override
		{
			return istd::IChangeable::SO_CLONE | istd::IChangeable::SO_COPY | istd::IChangeable::SO_RESET;
		}

		virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override
		{
			TestDataModel* clone = new TestDataModel(m_value);
			return istd::TUniqueInterfacePtr<istd::IChangeable>(clone);
		}

		virtual bool CopyFrom(const istd::IChangeable& object,
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			const TestDataModel* other = dynamic_cast<const TestDataModel*>(&object);
			if (other)
			{
				m_value = other->m_value;
				return true;
			}
			return false;
		}

		virtual bool ResetData(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			m_value = 0;
			return true;
		}

		virtual bool Serialize(iser::IArchive& archive) override
		{
			archive.SerializeValue("value", m_value);
			return true;
		}

	private:
		int m_value;
	};

	// Helper class: Test observer
	class TestObserver: public imod::CSingleModelObserverBase
	{
	public:
		TestObserver() : m_updateCount(0) {}

		int GetUpdateCount() const { return m_updateCount; }
		const istd::IChangeable::ChangeSet& GetLastChangeSet() const { return m_lastChangeSet; }

		void Reset()
		{
			m_updateCount = 0;
			m_lastChangeSet.Clear();
		}

		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override
		{
			m_updateCount++;
			m_lastChangeSet = changeSet;
		}

	private:
		int m_updateCount;
		istd::IChangeable::ChangeSet m_lastChangeSet;
	};

	typedef imod::TModelWrap<TestDataModel> TestModel;

	imod::CModelProxy* m_proxy;
	TestModel* m_model1;
	TestModel* m_model2;
	TestObserver* m_observer;
};
