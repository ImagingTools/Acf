// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imod/CMultiModelDispatcherBase.h>
#include <imod/TModelWrap.h>
#include <iser/CSerializableBase.h>


/**
	Test class for CMultiModelDispatcherBase functionality.
	Tests the multi-model dispatcher including:
	- Model registration with unique IDs
	- Model unregistration
	- Change notification dispatch
	- Relevant flags filtering
	- Model retrieval by ID
*/
class CMultiModelDispatcherBaseTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();

	// Basic functionality tests
	void testRegisterModel();
	void testUnregisterModel();
	void testUnregisterAllModels();
	void testGetObjectAt();

	// Notification tests
	void testOnModelChanged();
	void testMultipleModelNotifications();
	void testRelevantFlagsFiltering();
	void testModelIdDispatching();

	// Edge cases
	void testRegisterSameIdTwice();
	void testUnregisterNonExistentModel();
	void testEmptyDispatcher();

private:
	// Helper class: Simple data model for testing
	class TestDataModel: public iser::CSerializableBase
	{
	public:
		enum ChangeFlags
		{
			CF_VALUE_CHANGED = 0x1,
			CF_STATE_CHANGED = 0x2,
			CF_CONFIG_CHANGED = 0x4
		};

		TestDataModel() : m_value(0), m_state(0) {}

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

		void SetState(int state)
		{
			if (m_state != state)
			{
				istd::IChangeable::ChangeSet changeSet;
				changeSet.Set(CF_STATE_CHANGED);
				BeginChanges(changeSet);
				m_state = state;
				EndChanges(changeSet);
			}
		}

		int GetState() const { return m_state; }

		void SetConfig(int config)
		{
			istd::IChangeable::ChangeSet changeSet;
			changeSet.Set(CF_CONFIG_CHANGED);
			BeginChanges(changeSet);
			EndChanges(changeSet);
		}

		// IChangeable interface
		virtual int GetSupportedOperations() const override
		{
			return istd::IChangeable::SO_CLONE | istd::IChangeable::SO_COPY | istd::IChangeable::SO_RESET;
		}

		virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override
		{
			TestDataModel* clone = new TestDataModel();
			clone->m_value = m_value;
			clone->m_state = m_state;
			return istd::TUniqueInterfacePtr<istd::IChangeable>(clone);
		}

		virtual bool CopyFrom(const istd::IChangeable& object,
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			const TestDataModel* other = dynamic_cast<const TestDataModel*>(&object);
			if (other)
			{
				m_value = other->m_value;
				m_state = other->m_state;
				return true;
			}
			return false;
		}

		virtual bool ResetData(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			m_value = 0;
			m_state = 0;
			return true;
		}

		virtual bool Serialize(iser::IArchive& archive) override
		{
			archive.SerializeValue("value", m_value);
			archive.SerializeValue("state", m_state);
			return true;
		}

	private:
		int m_value;
		int m_state;
	};

	// Helper class: Concrete dispatcher implementation for testing
	class TestDispatcher: public imod::CMultiModelDispatcherBase
	{
	public:
		TestDispatcher() : m_notificationCount(0), m_lastModelId(-1) {}

		int GetNotificationCount() const { return m_notificationCount; }
		int GetLastModelId() const { return m_lastModelId; }
		const istd::IChangeable::ChangeSet& GetLastChangeSet() const { return m_lastChangeSet; }

		void Reset()
		{
			m_notificationCount = 0;
			m_lastModelId = -1;
			m_lastChangeSet.Clear();
			m_changeHistory.clear();
		}

		struct ChangeRecord
		{
			int modelId;
			istd::IChangeable::ChangeSet changeSet;
		};

		const QVector<ChangeRecord>& GetChangeHistory() const { return m_changeHistory; }

	protected:
		// Reimplemented from CMultiModelDispatcherBase
		virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet) override
		{
			m_notificationCount++;
			m_lastModelId = modelId;
			m_lastChangeSet = changeSet;

			ChangeRecord record;
			record.modelId = modelId;
			record.changeSet = changeSet;
			m_changeHistory.append(record);
		}

	private:
		int m_notificationCount;
		int m_lastModelId;
		istd::IChangeable::ChangeSet m_lastChangeSet;
		QVector<ChangeRecord> m_changeHistory;
	};

	typedef imod::TModelWrap<TestDataModel> TestModel;

	TestDispatcher* m_dispatcher;
	TestModel* m_model1;
	TestModel* m_model2;
	TestModel* m_model3;
};
