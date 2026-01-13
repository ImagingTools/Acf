#include <iprm/Test/COptionsListTest.h>


// ACF includes
#include <istd/TDelPtr.h>


// Mock implementation of IOptionsList for testing
class CMockOptionsList: public iprm::IOptionsList
{
public:
	CMockOptionsList(bool empty = false)
	{
		if (!empty)
		{
			// Add some test options
			m_options.append(Option{"Option1", "First option", "opt1"});
			m_options.append(Option{"Option2", "Second option", "opt2"});
			m_options.append(Option{"Option3", "Third option", "opt3"});
		}
	}

	// IOptionsList interface
	virtual int GetOptionsFlags() const override
	{
		return SCF_SUPPORT_UNIQUE_ID;
	}

	virtual int GetOptionsCount() const override
	{
		return m_options.count();
	}

	virtual QString GetOptionName(int index) const override
	{
		if (index >= 0 && index < m_options.count())
			return m_options[index].name;
		return QString();
	}

	virtual QString GetOptionDescription(int index) const override
	{
		if (index >= 0 && index < m_options.count())
			return m_options[index].description;
		return QString();
	}

	virtual QByteArray GetOptionId(int index) const override
	{
		if (index >= 0 && index < m_options.count())
			return m_options[index].id;
		return QByteArray();
	}

	virtual bool IsOptionEnabled(int index) const override
	{
		return (index >= 0 && index < m_options.count());
	}

	// IChangeable interface (minimal implementation)
	virtual int GetSupportedOperations() const override
	{
		return 0;
	}

	virtual bool CopyFrom(const IChangeable& /*object*/, CompatibilityMode /*mode*/ = CM_WITHOUT_REFS) override
	{
		return false;
	}

	virtual bool IsEqual(const IChangeable& /*object*/) const override
	{
		return false;
	}

	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode /*mode*/ = CM_WITHOUT_REFS) const override
	{
		return istd::TUniqueInterfacePtr<istd::IChangeable>();
	}

	virtual bool ResetData(CompatibilityMode /*mode*/ = CM_WITHOUT_REFS) override
	{
		return false;
	}

private:
	struct Option
	{
		QString name;
		QString description;
		QByteArray id;
	};
	QList<Option> m_options;
};


void COptionsListTest::initTestCase()
{

}


void COptionsListTest::FindOptionIndexById_ValidId_Test()
{
	CMockOptionsList list;
	
	// Test finding the first option
	int index = iprm::FindOptionIndexById("opt1", list);
	QCOMPARE(index, 0);

	// Test finding the second option
	index = iprm::FindOptionIndexById("opt2", list);
	QCOMPARE(index, 1);

	// Test finding the third option
	index = iprm::FindOptionIndexById("opt3", list);
	QCOMPARE(index, 2);
}


void COptionsListTest::FindOptionIndexById_InvalidId_Test()
{
	CMockOptionsList list;
	
	// Test with non-existent ID
	int index = iprm::FindOptionIndexById("invalid_id", list);
	QCOMPARE(index, -1);
}


void COptionsListTest::FindOptionIndexById_EmptyId_Test()
{
	CMockOptionsList list;
	
	// Test with empty ID - should return -1
	int index = iprm::FindOptionIndexById(QByteArray(), list);
	QCOMPARE(index, -1);

	// Test with empty string
	index = iprm::FindOptionIndexById("", list);
	QCOMPARE(index, -1);
}


void COptionsListTest::FindOptionIndexById_EmptyList_Test()
{
	// Create an empty mock list using constructor parameter
	CMockOptionsList emptyList(true);
	
	// Test with empty list - should return -1
	int index = iprm::FindOptionIndexById("any_id", emptyList);
	QCOMPARE(index, -1);
}


void COptionsListTest::FindOptionIndexById_MultipleOptions_Test()
{
	CMockOptionsList list;
	
	// Verify we have the expected number of options
	QCOMPARE(list.GetOptionsCount(), 3);

	// Test finding each option and verify we get the correct index
	for (int i = 0; i < list.GetOptionsCount(); ++i)
	{
		QByteArray optionId = list.GetOptionId(i);
		int foundIndex = iprm::FindOptionIndexById(optionId, list);
		QCOMPARE(foundIndex, i);
	}
}


void COptionsListTest::cleanupTestCase()
{
}


I_ADD_TEST(COptionsListTest);


