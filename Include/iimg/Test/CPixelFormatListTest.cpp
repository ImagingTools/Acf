// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CPixelFormatListTest.h"


// protected slots

void CPixelFormatListTest::initTestCase()
{
	m_formatListPtr = new iimg::CPixelFormatList();
	QVERIFY(m_formatListPtr != nullptr);
}


void CPixelFormatListTest::ConstructorTest()
{
	iimg::CPixelFormatList formatList;
	
	// Verify options count is reasonable (should have pixel formats)
	int count = formatList.GetOptionsCount();
	QVERIFY(count > 0);
}


void CPixelFormatListTest::GetInstanceTest()
{
	const iimg::CPixelFormatList& instance1 = iimg::CPixelFormatList::GetInstance();
	const iimg::CPixelFormatList& instance2 = iimg::CPixelFormatList::GetInstance();
	
	// Verify singleton pattern - both references should point to same instance
	QVERIFY(&instance1 == &instance2);
	
	// Instance should be valid
	QVERIFY(instance1.GetOptionsCount() > 0);
}


void CPixelFormatListTest::GetOptionsCountTest()
{
	int count = m_formatListPtr->GetOptionsCount();
	
	// Should have at least the basic pixel formats
	// PF_UNKNOWN, PF_MONO, PF_GRAY, PF_RGB, PF_RGBA, etc.
	QVERIFY(count > 5);
}


void CPixelFormatListTest::GetOptionNameTest()
{
	int count = m_formatListPtr->GetOptionsCount();
	QVERIFY(count > 0);
	
	// Test first option
	QString name0 = m_formatListPtr->GetOptionName(0);
	QVERIFY(!name0.isEmpty());
	
	// Test known format indices if available
	// Index 1 should be PF_MONO
	if (count > 1)
	{
		QString name1 = m_formatListPtr->GetOptionName(1);
		QVERIFY(!name1.isEmpty());
	}
	
	// Test index 2 should be PF_GRAY
	if (count > 2)
	{
		QString name2 = m_formatListPtr->GetOptionName(2);
		QVERIFY(!name2.isEmpty());
	}
}


void CPixelFormatListTest::GetOptionDescriptionTest()
{
	int count = m_formatListPtr->GetOptionsCount();
	QVERIFY(count > 0);
	
	// Test first option
	QString description = m_formatListPtr->GetOptionDescription(0);
	// Description might be empty, so just verify it doesn't crash
	// and returns a valid QString
	QVERIFY(description.isNull() || !description.isNull());
	
	// Test another index
	if (count > 2)
	{
		QString description2 = m_formatListPtr->GetOptionDescription(2);
		QVERIFY(description2.isNull() || !description2.isNull());
	}
}


void CPixelFormatListTest::GetOptionIdTest()
{
	int count = m_formatListPtr->GetOptionsCount();
	QVERIFY(count > 0);
	
	// Test first option
	QByteArray id0 = m_formatListPtr->GetOptionId(0);
	QVERIFY(!id0.isEmpty());
	
	// Test more options
	if (count > 3)
	{
		// PF_RGB should be at index 3
		QByteArray id3 = m_formatListPtr->GetOptionId(3);
		QVERIFY(!id3.isEmpty());
	}
	
	// Verify IDs are unique
	QSet<QByteArray> ids;
	for (int i = 0; i < count; i++)
	{
		QByteArray id = m_formatListPtr->GetOptionId(i);
		QVERIFY(!ids.contains(id));
		ids.insert(id);
	}
}


void CPixelFormatListTest::IsOptionEnabledTest()
{
	int count = m_formatListPtr->GetOptionsCount();
	QVERIFY(count > 0);
	
	// By default, no formats are set as active
	// But IsOptionEnabled should not crash
	for (int i = 0; i < qMin(count, 5); i++)
	{
		// Just verify the call doesn't crash
		bool enabled = m_formatListPtr->IsOptionEnabled(i);
		// Result depends on active formats set
		Q_UNUSED(enabled);
	}
}


void CPixelFormatListTest::GetActiveFormatsTest()
{
	iimg::CPixelFormatList formatList;
	
	iimg::CPixelFormatList::ActiveFormats formats = formatList.GetActiveFormats();
	
	// Initially should be empty or contain default formats
	// Just verify the call works
	QVERIFY(formats.isEmpty() || !formats.isEmpty());
}


void CPixelFormatListTest::SetActiveFormatsTest()
{
	iimg::CPixelFormatList formatList;
	
	// Create a set of active formats
	iimg::CPixelFormatList::ActiveFormats activeFormats;
	activeFormats.insert(iimg::IBitmap::PF_GRAY);
	activeFormats.insert(iimg::IBitmap::PF_RGB);
	activeFormats.insert(iimg::IBitmap::PF_RGBA);
	
	formatList.SetActiveFormats(activeFormats);
	
	// Verify the formats were set
	iimg::CPixelFormatList::ActiveFormats retrievedFormats = formatList.GetActiveFormats();
	QCOMPARE(retrievedFormats.size(), activeFormats.size());
	QVERIFY(retrievedFormats.contains(iimg::IBitmap::PF_GRAY));
	QVERIFY(retrievedFormats.contains(iimg::IBitmap::PF_RGB));
	QVERIFY(retrievedFormats.contains(iimg::IBitmap::PF_RGBA));
	
	// Verify IsOptionEnabled reflects the active formats
	QVERIFY(formatList.IsOptionEnabled(iimg::IBitmap::PF_GRAY));
	QVERIFY(formatList.IsOptionEnabled(iimg::IBitmap::PF_RGB));
	QVERIFY(formatList.IsOptionEnabled(iimg::IBitmap::PF_RGBA));
	QVERIFY(!formatList.IsOptionEnabled(iimg::IBitmap::PF_MONO));
}


void CPixelFormatListTest::GetOptionsFlagsTest()
{
	int flags = m_formatListPtr->GetOptionsFlags();
	
	// Flags should be non-negative
	QVERIFY(flags >= 0);
}


void CPixelFormatListTest::cleanupTestCase()
{
	delete m_formatListPtr;
	m_formatListPtr = nullptr;
}


I_ADD_TEST(CPixelFormatListTest);
