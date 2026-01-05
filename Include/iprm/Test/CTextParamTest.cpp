#include <iprm/Test/CTextParamTest.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <iprm/CIdParam.h>


void CTextParamTest::initTestCase()
{

}


void CTextParamTest::GetSetTest()
{
	QVERIFY(m_param.GetText().isEmpty());

	m_param.SetText(m_text);

	QVERIFY(m_param.GetText() == m_text);
}


void CTextParamTest::SetSameTextTest()
{
	// Verify that setting the same text value multiple times works correctly
	m_param.SetText(m_text);
	QVERIFY(m_param.GetText() == m_text);

	// Set the same value again - should remain unchanged
	m_param.SetText(m_text);
	QVERIFY(m_param.GetText() == m_text);
}


void CTextParamTest::SetEmptyTextTest()
{
	// Test setting empty text
	m_param.SetText(m_text);
	QVERIFY(m_param.GetText() == m_text);

	m_param.SetText(QString());
	QVERIFY(m_param.GetText().isEmpty());
}


void CTextParamTest::GetSupportedOperationsTest()
{
	// Test that all expected operations are supported
	int operations = m_param.GetSupportedOperations();
	QVERIFY(operations & istd::IChangeable::SO_CLONE);
	QVERIFY(operations & istd::IChangeable::SO_COPY);
	QVERIFY(operations & istd::IChangeable::SO_COMPARE);
	QVERIFY(operations & istd::IChangeable::SO_RESET);
}


void CTextParamTest::ResetTest()
{
	m_param.SetText(m_text);
	QVERIFY(m_param.GetText() == m_text);
	QVERIFY(m_param.ResetData());
	QVERIFY(m_param.GetText().isEmpty());
}


void CTextParamTest::ResetEmptyTest()
{
	// Test resetting when already empty - using a fresh instance to ensure clean state
	iprm::CTextParam param;
	QVERIFY(param.GetText().isEmpty());
	QVERIFY(param.ResetData());
	QVERIFY(param.GetText().isEmpty());
}


void CTextParamTest::SerializeTest()
{
	iser::CMemoryWriteArchive writeArchive(nullptr);

	m_param.SetText(m_text);

	QVERIFY(m_param.Serialize(writeArchive));

	QVERIFY(m_param.ResetData());
	QVERIFY(m_param.GetText().isEmpty());

	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_param.Serialize(readArchive));

	QVERIFY(m_param.GetText() == m_text);
}


void CTextParamTest::CopyFromTest()
{
	iprm::CTextParam sourceParam;
	sourceParam.SetText(m_text);
	
	iprm::CTextParam param;

	QVERIFY(sourceParam.GetText() == m_text);
	QVERIFY(param.GetText().isEmpty());

	QVERIFY(param.CopyFrom(sourceParam));

	QVERIFY(param.GetText() == m_text);
}


void CTextParamTest::CopyFromIncompatibleTest()
{
	// Test CopyFrom with incompatible type
	iprm::CIdParam idParam;
	idParam.SetId("TestId");

	iprm::CTextParam param;
	param.SetText(m_text);

	// CopyFrom should fail with incompatible type
	QVERIFY(!param.CopyFrom(idParam));
	// Text should remain unchanged
	QVERIFY(param.GetText() == m_text);
}


void CTextParamTest::CloneTest()
{
	QVERIFY(m_param.GetText() == m_text);

	istd::TUniqueInterfacePtr<iprm::ITextParam> clonePtr;
	QVERIFY(clonePtr.MoveCastedPtr(m_param.CloneMe()));
	QVERIFY(clonePtr.IsValid());

	QVERIFY(clonePtr->GetText() == m_text);
}


void CTextParamTest::CloneWithModeTest()
{
	// Test clone with explicit compatibility mode
	m_param.SetText(m_text);
	
	istd::TUniqueInterfacePtr<iprm::ITextParam> clonePtr;
	QVERIFY(clonePtr.MoveCastedPtr(m_param.CloneMe(istd::IChangeable::CM_WITHOUT_REFS)));
	QVERIFY(clonePtr.IsValid());
	QVERIFY(clonePtr->GetText() == m_text);

	// Test with CM_WITH_REFS mode
	istd::TUniqueInterfacePtr<iprm::ITextParam> clonePtr2;
	QVERIFY(clonePtr2.MoveCastedPtr(m_param.CloneMe(istd::IChangeable::CM_WITH_REFS)));
	QVERIFY(clonePtr2.IsValid());
	QVERIFY(clonePtr2->GetText() == m_text);
}


void CTextParamTest::IsEqualTest()
{
	iprm::CTextParam param;
	param.SetText(m_text);

	QVERIFY(m_param.IsEqual(param));
}


void CTextParamTest::IsEqualDifferentTextTest()
{
	// Test IsEqual with different text values
	iprm::CTextParam param1;
	param1.SetText(m_text);

	iprm::CTextParam param2;
	param2.SetText("Different Text");

	QVERIFY(!param1.IsEqual(param2));
	QVERIFY(!param2.IsEqual(param1));
}


void CTextParamTest::IsEqualIncompatibleTest()
{
	// Test IsEqual with incompatible type
	iprm::CIdParam idParam;
	idParam.SetId("TestId");

	iprm::CTextParam param;
	param.SetText(m_text);

	// IsEqual should return false for incompatible type
	QVERIFY(!param.IsEqual(idParam));
}


void CTextParamTest::IsReadOnlyTest()
{
	// Test that text parameter is not read-only by default
	QVERIFY(!m_param.IsReadOnly());
}


void CTextParamTest::cleanupTestCase()
{
}


I_ADD_TEST(CTextParamTest);


