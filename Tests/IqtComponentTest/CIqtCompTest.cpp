// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CIqtCompTest.h"


// Qt includes
#include <QtCore/QSettings>

// ACF includes
#include <iprm/ISelectionParam.h>
#include <iprm/IOptionsList.h>


// protected slots

void CIqtCompTest::initTestCase()
{
	m_testInstanceCompPtr.reset(new CIqtComponentTest);

	m_applicationSettingsProviderPtr = m_testInstanceCompPtr->GetInterface<iqt::ISettingsProvider>("ApplicationSettingsProvider");
	m_iniSettingsProviderPtr = m_testInstanceCompPtr->GetInterface<iqt::ISettingsProvider>("IniSettingsProvider");
	m_clipboardSerializerPtr = m_testInstanceCompPtr->GetInterface<ifile::IFilePersistence>("ClipboardSerializer");
	m_settingsSerializerPtr = m_testInstanceCompPtr->GetInterface<ifile::IFilePersistence>("SettingsSerializer");
	m_translationManagerPtr = m_testInstanceCompPtr->GetInterface<iqt::ITranslationManager>("TranslationManager");

	QVERIFY(m_applicationSettingsProviderPtr != nullptr);
	QVERIFY(m_iniSettingsProviderPtr != nullptr);
	QVERIFY(m_clipboardSerializerPtr != nullptr);
	QVERIFY(m_settingsSerializerPtr != nullptr);
	QVERIFY(m_translationManagerPtr != nullptr);
}


// Tests for CApplicationSettingsProviderComp

void CIqtCompTest::testApplicationSettingsProviderCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_applicationSettingsProviderPtr != nullptr);
	
	// Verify we can get the settings object
	QSettings& settings = m_applicationSettingsProviderPtr->GetSettings();
	QVERIFY(&settings != nullptr);
}


void CIqtCompTest::testApplicationSettingsProviderGetSettings()
{
	// Get the settings object
	QSettings& settings = m_applicationSettingsProviderPtr->GetSettings();
	
	// Verify the settings have the correct organization and application names
	QCOMPARE(settings.organizationName(), QString("ImagingTools"));
	QCOMPARE(settings.applicationName(), QString("IqtTestApp"));
}


void CIqtCompTest::testApplicationSettingsProviderReadWrite()
{
	// Get the settings object
	QSettings& settings = m_applicationSettingsProviderPtr->GetSettings();
	
	// Write a test value
	settings.setValue("TestKey", "TestValue");
	settings.sync();
	
	// Read it back
	QString value = settings.value("TestKey").toString();
	QCOMPARE(value, QString("TestValue"));
	
	// Clean up
	settings.remove("TestKey");
	settings.sync();
}


// Tests for CIniSettingsProviderComp

void CIqtCompTest::testIniSettingsProviderCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_iniSettingsProviderPtr != nullptr);
	
	// Verify we can get the settings object
	QSettings& settings = m_iniSettingsProviderPtr->GetSettings();
	QVERIFY(&settings != nullptr);
}


void CIqtCompTest::testIniSettingsProviderGetSettings()
{
	// Get the settings object
	QSettings& settings = m_iniSettingsProviderPtr->GetSettings();
	
	// Verify the settings are in INI format
	QCOMPARE(settings.format(), QSettings::IniFormat);
}


void CIqtCompTest::testIniSettingsProviderReadWrite()
{
	// Get the settings object
	QSettings& settings = m_iniSettingsProviderPtr->GetSettings();
	
	// Write a test value
	settings.setValue("IniTestKey", "IniTestValue");
	settings.sync();
	
	// Read it back
	QString value = settings.value("IniTestKey").toString();
	QCOMPARE(value, QString("IniTestValue"));
	
	// Clean up
	settings.remove("IniTestKey");
	settings.sync();
}


// Tests for CClipboardSerializerComp

void CIqtCompTest::testClipboardSerializerCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_clipboardSerializerPtr != nullptr);
}


void CIqtCompTest::testClipboardSerializerInterfaces()
{
	// Verify the component implements IFilePersistence
	QVERIFY(m_clipboardSerializerPtr != nullptr);
	
	// Get test data object
	iprm::ISelectionParam* testDataPtr = m_testInstanceCompPtr->GetInterface<iprm::ISelectionParam>("TestDataObject");
	QVERIFY(testDataPtr != nullptr);
	
	istd::IChangeable* changeablePtr = dynamic_cast<istd::IChangeable*>(testDataPtr);
	QVERIFY(changeablePtr != nullptr);
	
	// Check if operation is supported (should be true for clipboard)
	bool isSupported = m_clipboardSerializerPtr->IsOperationSupported(changeablePtr);
	QVERIFY(isSupported);
}


void CIqtCompTest::testClipboardSerializerFileExtensions()
{
	// Get file extensions
	QStringList extensions;
	bool result = m_clipboardSerializerPtr->GetFileExtensions(extensions);
	
	// Clipboard serializer typically doesn't have file extensions
	// but the method should succeed
	QVERIFY(result);
}


// Tests for CSettingsSerializerComp

void CIqtCompTest::testSettingsSerializerCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_settingsSerializerPtr != nullptr);
}


void CIqtCompTest::testSettingsSerializerInterfaces()
{
	// Verify the component implements IFilePersistence
	QVERIFY(m_settingsSerializerPtr != nullptr);
	
	// Get test data object
	iprm::ISelectionParam* testDataPtr = m_testInstanceCompPtr->GetInterface<iprm::ISelectionParam>("TestDataObject");
	QVERIFY(testDataPtr != nullptr);
	
	istd::IChangeable* changeablePtr = dynamic_cast<istd::IChangeable*>(testDataPtr);
	QVERIFY(changeablePtr != nullptr);
	
	// Check if operation is supported
	bool isSupported = m_settingsSerializerPtr->IsOperationSupported(changeablePtr);
	QVERIFY(isSupported);
}


void CIqtCompTest::testSettingsSerializerFileExtensions()
{
	// Get file extensions
	QStringList extensions;
	bool result = m_settingsSerializerPtr->GetFileExtensions(extensions);
	
	// Settings serializer typically doesn't have file extensions
	// but the method should succeed
	QVERIFY(result);
}


// Tests for CTranslationManagerComp

void CIqtCompTest::testTranslationManagerCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_translationManagerPtr != nullptr);
}


void CIqtCompTest::testTranslationManagerLanguagesInfo()
{
	// Get languages info
	const iprm::IOptionsList& languagesList = m_translationManagerPtr->GetLanguagesInfo();
	
	// Verify we have the expected number of languages (3: en, de, fr)
	QCOMPARE(languagesList.GetOptionsCount(), 3);
	
	// Verify language names
	QCOMPARE(languagesList.GetOptionName(0), QString("English"));
	QCOMPARE(languagesList.GetOptionName(1), QString("German"));
	QCOMPARE(languagesList.GetOptionName(2), QString("French"));
	
	// Verify language IDs
	QCOMPARE(languagesList.GetOptionId(0), QByteArray("en"));
	QCOMPARE(languagesList.GetOptionId(1), QByteArray("de"));
	QCOMPARE(languagesList.GetOptionId(2), QByteArray("fr"));
}


void CIqtCompTest::testTranslationManagerSwitchLanguage()
{
	// Get initial language
	int initialLanguage = m_translationManagerPtr->GetCurrentLanguageIndex();
	
	// Switch to different language
	int newLanguage = (initialLanguage + 1) % 3;
	m_translationManagerPtr->SwitchLanguage(newLanguage);
	
	// Verify language was switched
	QCOMPARE(m_translationManagerPtr->GetCurrentLanguageIndex(), newLanguage);
	
	// Switch back
	m_translationManagerPtr->SwitchLanguage(initialLanguage);
}


void CIqtCompTest::testTranslationManagerCurrentLanguage()
{
	// Get current language index
	int currentLanguage = m_translationManagerPtr->GetCurrentLanguageIndex();
	
	// Verify it's a valid index
	// Note: -1 is a valid value indicating no language is currently selected
	QVERIFY(currentLanguage >= -1);
	QVERIFY(currentLanguage < 3);
}


void CIqtCompTest::cleanupTestCase()
{
	m_testInstanceCompPtr.reset();
}


I_ADD_TEST(CIqtCompTest);
