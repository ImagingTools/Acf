// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imod/TModelWrap.h>
#include <imod/CSingleModelObserverBase.h>
#include <iser/CSerializableBase.h>


/**
	Test class for TModelWrap functionality.
	Tests the template wrapper that converts IChangeable objects into observable models.
	Tests include:
	- Model wrapping and base object access
	- Observer attachment and notification
	- Change notification propagation
	- Cloning and copying
	- Serialization
*/
class TModelWrapTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();

	// Basic functionality tests
	void testModelWrapCreation();
	void testSetBaseObject();
	void testGetSupportedOperations();
	void testCloneMe();

	// Observer tests
	void testObserverAttachment();
	void testChangeNotification();
	void testBeginEndChanges();
	void testBeginEndChangeGroup();

	// Data modification tests
	void testModelDataAccess();
	void testCopyFrom();
	void testResetData();

private:
	// Helper class: Simple data class to wrap
	class SimpleData: public iser::CSerializableBase
	{
	public:
		enum ChangeFlags
		{
			CF_VALUE_CHANGED = 0x1,
			CF_TEXT_CHANGED = 0x2
		};

		SimpleData() : m_value(0) {}

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

		void SetText(const QString& text)
		{
			if (m_text != text)
			{
				istd::IChangeable::ChangeSet changeSet;
				changeSet.Set(CF_TEXT_CHANGED);
				BeginChanges(changeSet);
				m_text = text;
				EndChanges(changeSet);
			}
		}

		QString GetText() const { return m_text; }

		// IChangeable interface
		virtual int GetSupportedOperations() const override
		{
			return istd::IChangeable::SO_CLONE | istd::IChangeable::SO_COPY | istd::IChangeable::SO_RESET;
		}

		virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override
		{
			SimpleData* clone = new SimpleData();
			clone->m_value = m_value;
			clone->m_text = m_text;
			return istd::TUniqueInterfacePtr<istd::IChangeable>(clone);
		}

		virtual bool CopyFrom(const istd::IChangeable& object,
			istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			const SimpleData* other = dynamic_cast<const SimpleData*>(&object);
			if (other)
			{
				m_value = other->m_value;
				m_text = other->m_text;
				return true;
			}
			return false;
		}

		virtual bool ResetData(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) override
		{
			m_value = 0;
			m_text.clear();
			return true;
		}

		virtual bool Serialize(iser::IArchive& archive) override
		{
			archive.SerializeValue("value", m_value);
			archive.SerializeValue("text", m_text);
			return true;
		}

	private:
		int m_value;
		QString m_text;
	};

	// Helper class: Observer for testing notifications
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

	typedef imod::TModelWrap<SimpleData> WrappedModel;

	WrappedModel* m_model;
	TestObserver* m_observer;
};
