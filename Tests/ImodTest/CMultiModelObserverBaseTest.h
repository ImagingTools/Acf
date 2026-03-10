// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imod/CMultiModelObserverBase.h>
#include <imod/TModelWrap.h>
#include <iser/CSerializableBase.h>


/**
	Test class for CMultiModelObserverBase functionality.
	Tests multi-model observation including:
	- Multiple model attachment/detachment
	- Model count tracking
	- Update notifications from multiple models
	- Change set filtering
*/
class CMultiModelObserverBaseTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();

	// Basic functionality tests
	void testAttachMultipleModels();
	void testDetachModel();
	void testEnsureModelsDetached();
	void testGetModelCount();
	void testGetObservedModel();
	void testIsModelAttached();

	// Update notification tests
	void testOnUpdateFromMultipleModels();
	void testChangeSetFiltering();
	void testSetObservedIds();

	// Edge cases
	void testAttachSameModelMultipleTimes();
	void testDetachNonAttachedModel();
	void testUpdateAfterPartialDetach();

private:
	// Helper class: Simple data model for testing
	class TestDataModel: public iser::CSerializableBase
	{
	public:
		enum ChangeFlags
		{
			CF_VALUE_CHANGED = 0x1,
			CF_STATE_CHANGED = 0x2
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

	// Helper class: Multi-model observer that tracks updates
	class TestObserver: public imod::CMultiModelObserverBase
	{
	public:
		TestObserver() : m_updateCount(0) {}

		int GetUpdateCount() const { return m_updateCount; }
		imod::IModel* GetLastUpdatedModel() const { return m_lastUpdatedModel; }
		const istd::IChangeable::ChangeSet& GetLastChangeSet() const { return m_lastChangeSet; }

		void Reset()
		{
			m_updateCount = 0;
			m_lastUpdatedModel = nullptr;
			m_lastChangeSet.Clear();
		}

		// Track all model updates
		struct UpdateRecord
		{
			imod::IModel* model;
			istd::IChangeable::ChangeSet changeSet;
		};

		const QVector<UpdateRecord>& GetUpdateHistory() const { return m_updateHistory; }

		void ClearHistory()
		{
			m_updateHistory.clear();
		}

		// Reimplemented from CMultiModelObserverBase
		virtual void OnUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override
		{
			m_updateCount++;
			m_lastUpdatedModel = modelPtr;
			m_lastChangeSet = changeSet;

			UpdateRecord record;
			record.model = modelPtr;
			record.changeSet = changeSet;
			m_updateHistory.append(record);
		}

	private:
		int m_updateCount;
		imod::IModel* m_lastUpdatedModel;
		istd::IChangeable::ChangeSet m_lastChangeSet;
		QVector<UpdateRecord> m_updateHistory;
	};

	typedef imod::TModelWrap<TestDataModel> TestModel;

	TestModel* m_model1;
	TestModel* m_model2;
	TestModel* m_model3;
	TestObserver* m_observer;
};
