// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <ilog/IMessageConsumer.h>
#include <ilog/IMessageContainer.h>
#include <ilog/ITracingConfiguration.h>
#include <ilog/CMessage.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/IlogComponentTest/CIlogComponentTest.h>

class CIlogComponentTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void testConsoleLogCreation();
	void testConsoleLogMessageHandling();
	void testLogCompCreation();
	void testLogCompMessageStorage();
	void testLogCompMaxMessageCount();
	void testLogRouterCreation();
	void testLogRouterMessageRouting();
	void testTracingConfigCreation();
	void testTracingConfigLevel();
	void testStreamLogFiltering();
	void testMessageSerialization();

	void cleanupTestCase();

private:
	std::shared_ptr<CIlogComponentTest> m_testPartituraInstanceCompPtr;
};


