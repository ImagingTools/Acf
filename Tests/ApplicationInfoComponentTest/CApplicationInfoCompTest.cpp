// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CApplicationInfoCompTest.h"


// ACF includes
#include <ibase/IApplicationInfo.h>
#include <iser/IVersionInfo.h>


// protected slots

void CApplicationInfoCompTest::initTestCase()
{
	m_testInstanceCompPtr.reset(new CApplicationInfoComponentTest);

	m_appInfoCompPtr = m_testInstanceCompPtr->GetInterface<ibase::IApplicationInfo>("ApplicationInfo");
	m_customAppInfoCompPtr = m_testInstanceCompPtr->GetInterface<ibase::IApplicationInfo>("CustomApplicationInfo");

	QVERIFY(m_appInfoCompPtr != nullptr);
	QVERIFY(m_customAppInfoCompPtr != nullptr);
}


void CApplicationInfoCompTest::cleanupTestCase()
{
	m_appInfoCompPtr = nullptr;
	m_customAppInfoCompPtr = nullptr;
	m_testInstanceCompPtr.reset();
}


// Application Info Component tests

void CApplicationInfoCompTest::testApplicationInfoComponentCreation()
{
	// Verify the components were created successfully
	QVERIFY(m_appInfoCompPtr != nullptr);
	QVERIFY(m_customAppInfoCompPtr != nullptr);
}


void CApplicationInfoCompTest::testGetApplicationAttribute()
{
	// Test that GetApplicationAttribute returns non-empty strings for valid attributes
	QString companyName = m_appInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME);
	QString productName = m_appInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_PRODUCT_NAME);
	QString appName = m_appInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME);
	
	QVERIFY(!companyName.isEmpty());
	QVERIFY(!productName.isEmpty());
	QVERIFY(!appName.isEmpty());
}


void CApplicationInfoCompTest::testGetCompanyName()
{
	// Test default company name
	QString companyName = m_appInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME);
	QCOMPARE(companyName, QString("<my company>"));
	
	// Test custom company name
	QString customCompanyName = m_customAppInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME);
	QCOMPARE(customCompanyName, QString("ImagingTools"));
}


void CApplicationInfoCompTest::testGetProductName()
{
	// Test default product name
	QString productName = m_appInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_PRODUCT_NAME);
	QCOMPARE(productName, QString("<my product>"));
	
	// Test custom product name
	QString customProductName = m_customAppInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_PRODUCT_NAME);
	QCOMPARE(customProductName, QString("ACF"));
}


void CApplicationInfoCompTest::testGetApplicationName()
{
	// Test default application name
	QString appName = m_appInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME);
	QCOMPARE(appName, QString("ACF Application"));
	
	// Test custom application name
	QString customAppName = m_customAppInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME);
	QCOMPARE(customAppName, QString("ApplicationInfoTest"));
}


void CApplicationInfoCompTest::testGetApplicationId()
{
	// Test default application ID
	QString appId = m_appInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_ID);
	QCOMPARE(appId, QString("ApplicationId"));
	
	// Test custom application ID
	QString customAppId = m_customAppInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_ID);
	QCOMPARE(customAppId, QString("AppInfoTestApp"));
}


void CApplicationInfoCompTest::testGetApplicationType()
{
	// Test default application type
	QString appType = m_appInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_TYPE);
	QCOMPARE(appType, QString("Server"));
	
	// Test custom application type
	QString customAppType = m_customAppInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_TYPE);
	QCOMPARE(customAppType, QString("Test"));
}


void CApplicationInfoCompTest::testGetLegalCopyright()
{
	// Test that legal copyright is not empty
	QString copyright = m_appInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_LEGAL_COPYRIGHT);
	QVERIFY(!copyright.isEmpty());
	
	// Test custom copyright
	QString customCopyright = m_customAppInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_LEGAL_COPYRIGHT);
	QCOMPARE(customCopyright, QString("© 2024 ImagingTools"));
}


void CApplicationInfoCompTest::testGetMainVersionId()
{
	// Test default main version ID (should be 0)
	int mainVersionId = m_appInfoCompPtr->GetMainVersionId();
	QCOMPARE(mainVersionId, 0);
	
	// Test custom main version ID
	int customMainVersionId = m_customAppInfoCompPtr->GetMainVersionId();
	QCOMPARE(customMainVersionId, 42);
}


void CApplicationInfoCompTest::testGetVersionInfo()
{
	// Get version info reference
	const iser::IVersionInfo& versionInfo = m_appInfoCompPtr->GetVersionInfo();
	
	// Version info should be accessible (returns empty version info if not configured)
	Q_UNUSED(versionInfo);
	
	// Test custom version info with actual version
	const iser::IVersionInfo& customVersionInfo = m_customAppInfoCompPtr->GetVersionInfo();
	
	// Get version number for main version ID
	quint32 versionNumber;
	bool hasVersion = customVersionInfo.GetVersionNumber(42, versionNumber);
	QVERIFY(hasVersion);
	QCOMPARE(versionNumber, quint32(12345));
}


void CApplicationInfoCompTest::testCustomValues()
{
	// Test product edition (optional attribute)
	QString productEdition = m_customAppInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_PRODUCT_EDITION);
	QCOMPARE(productEdition, QString("Professional"));
	
	// Test application subname (optional attribute)
	QString appSubname = m_customAppInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_SUBNAME);
	QCOMPARE(appSubname, QString("Unit Tests"));
	
	// Test application path (should always return ".")
	QString appPath = m_customAppInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_PATH);
	QCOMPARE(appPath, QString("."));
}
