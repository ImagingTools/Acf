// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imod/CModelUpdateBridge.h>
#include <imod/TModelWrap.h>
#include <iser/CSerializableBase.h>


/**
	Test class for CModelUpdateBridge functionality.
	Tests bridging model changes to IChangeable objects including:
	- Multiple model observation through bridge
	- Change notification delegation
	- Update flags (UF_DELEGATED, UF_SOURCE)
	- Model detachment from bridge
*/
class CModelUpdateBridgeTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();

	// Basic functionality tests
	void testBridgeCreation();
	void testAttachModelToBridge();
	void testDetachModelFromBridge();
	void testEnsureModelsDetached();
	void testGetModelCount();
	void testGetObservedModel();

	// Update delegation tests
	void testDelegatedUpdateFlag();
	void testSourceUpdateFlag();
	void testMultipleModelsUpdate();

	// Edge cases
	void testBridgeWithNullChangeable();
	void testMultipleAttachDetachCycles();

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

	// Helper class: Target object that receives bridged changes
	class TargetChangeable: public iser::CSerializableBase
	{
	public:
		TargetChangeable() : m_changeCount(0) {}

		int GetChangeCount() const { return m_changeCount; }
		const istd::IChangeable::ChangeSet& GetLastChangeSet() const { return m_lastChangeSet; }

		void Reset()
		{
			m_changeCount = 0;
			m_lastChangeSet.Clear();
		}

		// Override BeginChanges to track notifications
		virtual void BeginChanges(const istd::IChangeable::ChangeSet& changeSet) override
		{
			CSerializableBase::BeginChanges(changeSet);
			m_changeCount++;
			m_lastChangeSet = changeSet;
		}

		// IChangeable interface
		virtual int GetSupportedOperations() const override
		{
			return istd::IChangeable::SO_CLONE | istd::IChangeable::SO_COPY | istd::IChangeable::SO_RESET;
		}

		virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override
		{
			return istd::TUniqueInterfacePtr<istd::IChangeable>(new TargetChangeable());
		}

		virtual bool CopyFrom(const istd::IChangeable& object,
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			return true;
		}

		virtual bool ResetData(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			return true;
		}

		virtual bool Serialize(iser::IArchive& archive) override
		{
			return true;
		}

	private:
		int m_changeCount;
		istd::IChangeable::ChangeSet m_lastChangeSet;
	};

	typedef imod::TModelWrap<TestDataModel> TestModel;

	TargetChangeable* m_targetChangeable;
	imod::CModelUpdateBridge* m_bridge;
	TestModel* m_model1;
	TestModel* m_model2;
};
