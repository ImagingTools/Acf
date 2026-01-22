// SPDX-License-Identifier: LGPL-2.1-only
#include <iprm/Test/CEnableableParamTest.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <iprm/CIdParam.h>


void CEnableableParamTest::initTestCase()
{

}


void CEnableableParamTest::DefaultConstructorTest()
{
	// Test default constructor - should be enabled by default
	iprm::CEnableableParam param;
	QVERIFY(param.IsEnabled());
}


void CEnableableParamTest::ConstructorWithEnabledTest()
{
	// Test constructor with explicit enabled state
	iprm::CEnableableParam param(true);
	QVERIFY(param.IsEnabled());
}


void CEnableableParamTest::ConstructorWithDisabledTest()
{
	// Test constructor with disabled state
	iprm::CEnableableParam param(false);
	QVERIFY(!param.IsEnabled());
}


void CEnableableParamTest::IsEnabledTest()
{
	iprm::CEnableableParam param(true);
	QVERIFY(param.IsEnabled());

	iprm::CEnableableParam param2(false);
	QVERIFY(!param2.IsEnabled());
}


void CEnableableParamTest::SetEnabledTest()
{
	iprm::CEnableableParam param(false);
	QVERIFY(!param.IsEnabled());

	QVERIFY(param.SetEnabled(true));
	QVERIFY(param.IsEnabled());
}


void CEnableableParamTest::SetDisabledTest()
{
	iprm::CEnableableParam param(true);
	QVERIFY(param.IsEnabled());

	QVERIFY(param.SetEnabled(false));
	QVERIFY(!param.IsEnabled());
}


void CEnableableParamTest::SetSameStateTest()
{
	// Test setting the same state multiple times
	iprm::CEnableableParam param(true);
	QVERIFY(param.IsEnabled());

	// Setting same state should succeed
	QVERIFY(param.SetEnabled(true));
	QVERIFY(param.IsEnabled());

	QVERIFY(param.SetEnabled(false));
	QVERIFY(!param.IsEnabled());

	QVERIFY(param.SetEnabled(false));
	QVERIFY(!param.IsEnabled());
}


void CEnableableParamTest::IsEnablingAllowedTest()
{
	// Test that enabling is allowed by default
	iprm::CEnableableParam param;
	QVERIFY(param.IsEnablingAllowed());
}


void CEnableableParamTest::SerializeEnabledTest()
{
	iser::CMemoryWriteArchive writeArchive(nullptr);

	iprm::CEnableableParam param(true);
	QVERIFY(param.IsEnabled());

	QVERIFY(param.Serialize(writeArchive));

	iprm::CEnableableParam param2(false);
	QVERIFY(!param2.IsEnabled());

	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(param2.Serialize(readArchive));

	QVERIFY(param2.IsEnabled());
}


void CEnableableParamTest::SerializeDisabledTest()
{
	iser::CMemoryWriteArchive writeArchive(nullptr);

	iprm::CEnableableParam param(false);
	QVERIFY(!param.IsEnabled());

	QVERIFY(param.Serialize(writeArchive));

	iprm::CEnableableParam param2(true);
	QVERIFY(param2.IsEnabled());

	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(param2.Serialize(readArchive));

	QVERIFY(!param2.IsEnabled());
}


void CEnableableParamTest::CopyFromTest()
{
	iprm::CEnableableParam param1(true);
	iprm::CEnableableParam param2(false);

	QVERIFY(param1.IsEnabled());
	QVERIFY(!param2.IsEnabled());

	QVERIFY(param2.CopyFrom(param1));

	QVERIFY(param2.IsEnabled());
}


void CEnableableParamTest::CopyFromIncompatibleTest()
{
	// Test CopyFrom with incompatible type
	iprm::CIdParam idParam;
	idParam.SetId("TestId");

	iprm::CEnableableParam param(true);

	// CopyFrom should fail with incompatible type
	QVERIFY(!param.CopyFrom(idParam));
	// State should remain unchanged
	QVERIFY(param.IsEnabled());
}


void CEnableableParamTest::CloneTest()
{
	iprm::CEnableableParam param(true);
	QVERIFY(param.IsEnabled());

	istd::TUniqueInterfacePtr<iprm::IEnableableParam> clonePtr;
	QVERIFY(clonePtr.MoveCastedPtr(param.CloneMe()));
	QVERIFY(clonePtr.IsValid());

	QVERIFY(clonePtr->IsEnabled());

	// Test cloning disabled state
	iprm::CEnableableParam param2(false);
	QVERIFY(!param2.IsEnabled());

	istd::TUniqueInterfacePtr<iprm::IEnableableParam> clonePtr2;
	QVERIFY(clonePtr2.MoveCastedPtr(param2.CloneMe()));
	QVERIFY(clonePtr2.IsValid());

	QVERIFY(!clonePtr2->IsEnabled());
}


void CEnableableParamTest::CloneWithModeTest()
{
	// Test clone with explicit compatibility mode
	iprm::CEnableableParam param(true);
	
	istd::TUniqueInterfacePtr<iprm::IEnableableParam> clonePtr;
	QVERIFY(clonePtr.MoveCastedPtr(param.CloneMe(istd::IChangeable::CM_WITHOUT_REFS)));
	QVERIFY(clonePtr.IsValid());
	QVERIFY(clonePtr->IsEnabled());

	// Test with CM_WITH_REFS mode
	istd::TUniqueInterfacePtr<iprm::IEnableableParam> clonePtr2;
	QVERIFY(clonePtr2.MoveCastedPtr(param.CloneMe(istd::IChangeable::CM_WITH_REFS)));
	QVERIFY(clonePtr2.IsValid());
	QVERIFY(clonePtr2->IsEnabled());
}


void CEnableableParamTest::cleanupTestCase()
{
}


I_ADD_TEST(CEnableableParamTest);


