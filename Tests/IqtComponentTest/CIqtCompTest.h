// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iqt/ISettingsProvider.h>
#include <iqt/ITranslationManager.h>
#include <ifile/IFilePersistence.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/IqtComponentTest/CIqtComponentTest.h>

class CIqtCompTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Tests for CApplicationSettingsProviderComp
	void testApplicationSettingsProviderCreation();
	void testApplicationSettingsProviderGetSettings();
	void testApplicationSettingsProviderReadWrite();

	// Tests for CIniSettingsProviderComp
	void testIniSettingsProviderCreation();
	void testIniSettingsProviderGetSettings();
	void testIniSettingsProviderReadWrite();

	// Tests for CClipboardSerializerComp
	void testClipboardSerializerCreation();
	void testClipboardSerializerInterfaces();
	void testClipboardSerializerFileExtensions();

	// Tests for CSettingsSerializerComp
	void testSettingsSerializerCreation();
	void testSettingsSerializerInterfaces();
	void testSettingsSerializerFileExtensions();

	// Tests for CTranslationManagerComp
	void testTranslationManagerCreation();
	void testTranslationManagerLanguagesInfo();
	void testTranslationManagerSwitchLanguage();
	void testTranslationManagerCurrentLanguage();

	void cleanupTestCase();

private:
	std::shared_ptr<CIqtComponentTest> m_testInstanceCompPtr;

	iqt::ISettingsProvider* m_applicationSettingsProviderPtr = nullptr;
	iqt::ISettingsProvider* m_iniSettingsProviderPtr = nullptr;
	ifile::IFilePersistence* m_clipboardSerializerPtr = nullptr;
	ifile::IFilePersistence* m_settingsSerializerPtr = nullptr;
	iqt::ITranslationManager* m_translationManagerPtr = nullptr;
};
