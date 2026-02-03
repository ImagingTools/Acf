// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imod/CSingleModelObserverBase.h>
#include <imod/TModelWrap.h>
#include <iser/CSerializableBase.h>


/**
	Test class for CSingleModelObserverBase functionality.
	Tests single model observation including:
	- Model attachment/detachment
	- Update notifications
	- Change set filtering
	- Model pointer management
*/
class CSingleModelObserverBaseTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();

	// Basic functionality tests
	void testModelAttachment();
	void testModelDetachment();
	void testIsModelAttached();
	void testGetObservedModel();
	void testEnsureModelDetached();

	// Update notification tests
	void testOnUpdate();
	void testBeforeAndAfterUpdate();
	void testChangeSetFiltering();
	void testSetObservedIds();

	// Edge cases
	void testMultipleAttachDetachCycles();
	void testDetachNullModel();

private:
	// Helper class: Simple data model for testing
	class TestDataModel: public iser::CSerializableBase
	{
	public:
		enum ChangeFlags
		{
			CF_DATA_CHANGED = 0x1,
			CF_STATUS_CHANGED = 0x2
		};

		TestDataModel() : m_data(0), m_status(0) {}

		void SetData(int data)
		{
			if (m_data != data)
			{
				istd::IChangeable::ChangeSet changeSet;
				changeSet.Set(CF_DATA_CHANGED);
				BeginChanges(changeSet);
				m_data = data;
				EndChanges(changeSet);
			}
		}

		int GetData() const { return m_data; }

		void SetStatus(int status)
		{
			if (m_status != status)
			{
				istd::IChangeable::ChangeSet changeSet;
				changeSet.Set(CF_STATUS_CHANGED);
				BeginChanges(changeSet);
				m_status = status;
				EndChanges(changeSet);
			}
		}

		int GetStatus() const { return m_status; }

		// IChangeable interface
		virtual int GetSupportedOperations() const override
		{
			return istd::IChangeable::SO_CLONE | istd::IChangeable::SO_COPY | istd::IChangeable::SO_RESET;
		}

		virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override
		{
			TestDataModel* clone = new TestDataModel();
			clone->m_data = m_data;
			clone->m_status = m_status;
			return istd::TUniqueInterfacePtr<istd::IChangeable>(clone);
		}

		virtual bool CopyFrom(const istd::IChangeable& object,
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			const TestDataModel* other = dynamic_cast<const TestDataModel*>(&object);
			if (other)
			{
				m_data = other->m_data;
				m_status = other->m_status;
				return true;
			}
			return false;
		}

		virtual bool ResetData(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			m_data = 0;
			m_status = 0;
			return true;
		}

		virtual bool Serialize(iser::IArchive& archive) override
		{
			archive.SerializeValue("data", m_data);
			archive.SerializeValue("status", m_status);
			return true;
		}

	private:
		int m_data;
		int m_status;
	};

	// Helper class: Test observer that tracks updates
	class TestObserver: public imod::CSingleModelObserverBase
	{
	public:
		TestObserver() : m_updateCount(0), m_beforeUpdateCount(0) {}

		int GetUpdateCount() const { return m_updateCount; }
		int GetBeforeUpdateCount() const { return m_beforeUpdateCount; }
		const istd::IChangeable::ChangeSet& GetLastChangeSet() const { return m_lastChangeSet; }

		void Reset()
		{
			m_updateCount = 0;
			m_beforeUpdateCount = 0;
			m_lastChangeSet.Clear();
		}

		// Reimplemented from CSingleModelObserverBase
		virtual void BeforeUpdate(imod::IModel* modelPtr) override
		{
			CSingleModelObserverBase::BeforeUpdate(modelPtr);
			m_beforeUpdateCount++;
		}

		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override
		{
			m_updateCount++;
			m_lastChangeSet = changeSet;
		}

	private:
		int m_updateCount;
		int m_beforeUpdateCount;
		istd::IChangeable::ChangeSet m_lastChangeSet;
	};

	typedef imod::TModelWrap<TestDataModel> TestModel;

	TestModel* m_model;
	TestObserver* m_observer;
};
