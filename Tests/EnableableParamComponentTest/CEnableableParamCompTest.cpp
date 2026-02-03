// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CEnableableParamCompTest.h"


// ACF includes
#include <iprm/CEnableableParam.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


// protected slots

void CEnableableParamCompTest::initTestCase()
{
	m_testPartituraInstanceCompPtr.reset(new CEnableableParamComponentTest);

	m_enabledByDefaultPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::IEnableableParam>("EnabledByDefault");
	m_disabledByDefaultPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::IEnableableParam>("DisabledByDefault");

	QVERIFY(m_enabledByDefaultPtr != nullptr);
	QVERIFY(m_disabledByDefaultPtr != nullptr);
}


// Tests for enableable parameter with default enabled

void CEnableableParamCompTest::testCreationDefaultEnabled()
{
	// Verify the component was created successfully
	QVERIFY(m_enabledByDefaultPtr != nullptr);
	
	// Check that enabling is allowed
	QVERIFY(m_enabledByDefaultPtr->IsEnablingAllowed());
}


void CEnableableParamCompTest::testGetEnabledState()
{
	// Verify the parameter is enabled by default
	QVERIFY(m_enabledByDefaultPtr->IsEnabled());
}


void CEnableableParamCompTest::testSetEnabled()
{
	// Test disabling
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(false));
	QVERIFY(!m_enabledByDefaultPtr->IsEnabled());
	
	// Test enabling
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(true));
	QVERIFY(m_enabledByDefaultPtr->IsEnabled());
}


void CEnableableParamCompTest::testToggleEnabled()
{
	// Start with enabled state
	QVERIFY(m_enabledByDefaultPtr->IsEnabled());
	
	// Toggle to disabled
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(false));
	QVERIFY(!m_enabledByDefaultPtr->IsEnabled());
	
	// Toggle to enabled
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(true));
	QVERIFY(m_enabledByDefaultPtr->IsEnabled());
	
	// Multiple toggles
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(false));
	QVERIFY(!m_enabledByDefaultPtr->IsEnabled());
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(false));
	QVERIFY(!m_enabledByDefaultPtr->IsEnabled());
	
	// Restore enabled state
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(true));
}


void CEnableableParamCompTest::testSerializationEnabled()
{
	// Set to disabled
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(false));
	QVERIFY(!m_enabledByDefaultPtr->IsEnabled());
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_enabledByDefaultPtr->Serialize(writeArchive));
	
	// Change the state
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(true));
	QVERIFY(m_enabledByDefaultPtr->IsEnabled());
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_enabledByDefaultPtr->Serialize(readArchive));
	
	// Verify the state was restored to disabled
	QVERIFY(!m_enabledByDefaultPtr->IsEnabled());
	
	// Restore default state
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(true));
}


void CEnableableParamCompTest::testCopyEnabled()
{
	// Set to disabled
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(false));
	
	// Create a new enableable parameter and copy from the component
	iprm::CEnableableParam copyParam;
	QVERIFY(copyParam.CopyFrom(*m_enabledByDefaultPtr));
	
	// Verify the state was copied
	QCOMPARE(copyParam.IsEnabled(), m_enabledByDefaultPtr->IsEnabled());
	QVERIFY(!copyParam.IsEnabled());
	
	// Restore default state
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(true));
}


void CEnableableParamCompTest::testCloneEnabled()
{
	// Set to disabled
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(false));
	
	// Clone the parameter
	auto clonedPtr = m_enabledByDefaultPtr->CloneMe();
	QVERIFY(clonedPtr != nullptr);
	
	auto clonedEnableableParam = dynamic_cast<iprm::IEnableableParam*>(clonedPtr.get());
	QVERIFY(clonedEnableableParam != nullptr);
	
	// Verify the state was cloned
	QCOMPARE(clonedEnableableParam->IsEnabled(), m_enabledByDefaultPtr->IsEnabled());
	QVERIFY(!clonedEnableableParam->IsEnabled());
	
	// Modify the clone and verify original is unchanged
	QVERIFY(clonedEnableableParam->SetEnabled(true));
	QVERIFY(clonedEnableableParam->IsEnabled());
	QVERIFY(!m_enabledByDefaultPtr->IsEnabled());
	
	// Restore default state
	QVERIFY(m_enabledByDefaultPtr->SetEnabled(true));
}


// Tests for enableable parameter with default disabled

void CEnableableParamCompTest::testCreationDefaultDisabled()
{
	// Verify the component was created successfully
	QVERIFY(m_disabledByDefaultPtr != nullptr);
	
	// Check that enabling is allowed
	QVERIFY(m_disabledByDefaultPtr->IsEnablingAllowed());
}


void CEnableableParamCompTest::testDisabledState()
{
	// Verify the parameter is disabled by default
	QVERIFY(!m_disabledByDefaultPtr->IsEnabled());
	
	// Test enabling
	QVERIFY(m_disabledByDefaultPtr->SetEnabled(true));
	QVERIFY(m_disabledByDefaultPtr->IsEnabled());
	
	// Reset to disabled
	QVERIFY(m_disabledByDefaultPtr->SetEnabled(false));
	QVERIFY(!m_disabledByDefaultPtr->IsEnabled());
}


void CEnableableParamCompTest::testSerializationDisabled()
{
	// Set to enabled
	QVERIFY(m_disabledByDefaultPtr->SetEnabled(true));
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_disabledByDefaultPtr->Serialize(writeArchive));
	
	// Change the state
	QVERIFY(m_disabledByDefaultPtr->SetEnabled(false));
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_disabledByDefaultPtr->Serialize(readArchive));
	
	// Verify the state was restored to enabled
	QVERIFY(m_disabledByDefaultPtr->IsEnabled());
	
	// Reset to disabled
	QVERIFY(m_disabledByDefaultPtr->SetEnabled(false));
}


void CEnableableParamCompTest::cleanupTestCase()
{
	m_enabledByDefaultPtr = nullptr;
	m_disabledByDefaultPtr = nullptr;
	m_testPartituraInstanceCompPtr.reset();
}


I_ADD_TEST(CEnableableParamCompTest);
