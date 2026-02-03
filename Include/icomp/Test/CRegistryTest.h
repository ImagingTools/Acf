// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


class CRegistryTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	void cleanupTestCase();

	// Registry creation and basic operations
	void testRegistryCreation();
	void testElementInsertion();
	void testElementRetrieval();
	void testElementRemoval();
	void testElementRename();

	// Embedded registries
	void testEmbeddedRegistryInsertion();
	void testEmbeddedRegistryRetrieval();
	void testEmbeddedRegistryRemoval();

	// Exported interfaces
	void testExportedInterfaces();

	// Registry serialization
	void testRegistrySerialization();

	// Thread safety (basic tests)
	void testConcurrentAccess();
};
