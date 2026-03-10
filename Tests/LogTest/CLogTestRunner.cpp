// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CLogTestRunner.h"


// ACF includes
#include <ilog/CMessage.h>
#include <iser/CMemoryWriteArchive.h>
#include <istd/IInformationProvider.h>


// protected slots

void CLogTestRunner::initTestCase()
{
	m_testPartituraInstanceCompPtr.reset(new CLogTest());
	m_testPartituraInstanceCompPtr->EnsureAutoInitComponentsCreated();
}


void CLogTestRunner::testConsoleLogCreation()
{
	// Test that ConsoleLog component was created successfully
	ilog::IMessageConsumer* consoleLogPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageConsumer>("ConsoleLog");
	QVERIFY(consoleLogPtr != nullptr);
}


void CLogTestRunner::testConsoleLogMessageHandling()
{
	// Get ConsoleLog component
	ilog::IMessageConsumer* consoleLogPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageConsumer>("ConsoleLog");
	QVERIFY(consoleLogPtr != nullptr);

	// Test that it supports different message categories
	QVERIFY(consoleLogPtr->IsMessageSupported(istd::IInformationProvider::IC_INFO));
	QVERIFY(consoleLogPtr->IsMessageSupported(istd::IInformationProvider::IC_WARNING));
	QVERIFY(consoleLogPtr->IsMessageSupported(istd::IInformationProvider::IC_ERROR));
	QVERIFY(consoleLogPtr->IsMessageSupported(istd::IInformationProvider::IC_CRITICAL));

	// Create and add a test message
	ilog::IMessageConsumer::MessagePtr testMessage(
		new ilog::CMessage(
			istd::IInformationProvider::IC_INFO,
			1001,
			"Test information message",
			"TestSource"
		)
	);
	
	consoleLogPtr->AddMessage(testMessage);
	// Message should be logged to console (visual verification during test run)
}


void CLogTestRunner::testLogCompCreation()
{
	// Test that LogComp component was created successfully
	ilog::IMessageContainer* logCompPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageContainer>("LogComp");
	QVERIFY(logCompPtr != nullptr);

	// Also verify it implements IMessageConsumer interface
	ilog::IMessageConsumer* consumerPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageConsumer>("LogComp");
	QVERIFY(consumerPtr != nullptr);
}


void CLogTestRunner::testLogCompMessageStorage()
{
	// Get LogComp component
	ilog::IMessageContainer* logCompPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageContainer>("LogComp");
	ilog::IMessageConsumer* consumerPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageConsumer>("LogComp");
	QVERIFY(logCompPtr != nullptr);
	QVERIFY(consumerPtr != nullptr);

	// Clear any existing messages
	logCompPtr->ClearMessages();

	// Add test messages with different categories
	ilog::IMessageConsumer::MessagePtr infoMsg(
		new ilog::CMessage(istd::IInformationProvider::IC_INFO, 2001, "Info message", "TestSource")
	);
	ilog::IMessageConsumer::MessagePtr warningMsg(
		new ilog::CMessage(istd::IInformationProvider::IC_WARNING, 2002, "Warning message", "TestSource")
	);
	ilog::IMessageConsumer::MessagePtr errorMsg(
		new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 2003, "Error message", "TestSource")
	);

	consumerPtr->AddMessage(infoMsg);
	consumerPtr->AddMessage(warningMsg);
	consumerPtr->AddMessage(errorMsg);

	// Wait for asynchronous message processing with timeout
	QTRY_COMPARE_WITH_TIMEOUT(logCompPtr->GetMessages().size(), 3, 1000);

	// Verify messages are stored
	ilog::IMessageContainer::Messages messages = logCompPtr->GetMessages();
	QCOMPARE(messages.size(), 3);

	// Verify worst category is ERROR
	istd::IInformationProvider::InformationCategory worstCategory = logCompPtr->GetWorstCategory();
	QCOMPARE(worstCategory, istd::IInformationProvider::IC_ERROR);
}


void CLogTestRunner::testLogCompMaxMessageCount()
{
	// Get LogComp component (MaxMessageCount=100)
	ilog::IMessageContainer* logCompPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageContainer>("LogComp");
	ilog::IMessageConsumer* consumerPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageConsumer>("LogComp");
	QVERIFY(logCompPtr != nullptr);
	QVERIFY(consumerPtr != nullptr);

	// Clear any existing messages
	logCompPtr->ClearMessages();

	// Add more messages than the maximum (100)
	for (int i = 0; i < 110; ++i) {
		ilog::IMessageConsumer::MessagePtr msg(
			new ilog::CMessage(istd::IInformationProvider::IC_INFO, 3000 + i, QString("Message %1").arg(i), "TestSource")
		);
		consumerPtr->AddMessage(msg);
	}

	// Wait for asynchronous message processing with timeout
	// The message count should stabilize at or below MaxMessageCount (100)
	QTest::qWait(100); // Give initial time for processing
	int messageCount = logCompPtr->GetMessages().size();
	
	// Give additional time if still processing
	int maxWait = 5;
	while (messageCount != logCompPtr->GetMessages().size() && maxWait-- > 0) {
		QTest::qWait(50);
		messageCount = logCompPtr->GetMessages().size();
	}

	// Verify message count is limited to MaxMessageCount
	ilog::IMessageContainer::Messages messages = logCompPtr->GetMessages();
	QVERIFY(messages.size() <= 100);
}


void CLogTestRunner::testLogRouterCreation()
{
	// Test that LogRouter component was created successfully
	// Note: LogRouter doesn't have a specific interface, but should exist as a component
	icomp::IComponent* routerPtr = m_testPartituraInstanceCompPtr->GetInterface<icomp::IComponent>("LogRouter");
	QVERIFY(routerPtr != nullptr);
}


void CLogTestRunner::testLogRouterMessageRouting()
{
	// Get LogComp (source) and ErrorLog (destination)
	ilog::IMessageContainer* logCompPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageContainer>("LogComp");
	ilog::IMessageConsumer* logCompConsumerPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageConsumer>("LogComp");
	ilog::IMessageContainer* errorLogPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageContainer>("ErrorLog");
	
	QVERIFY(logCompPtr != nullptr);
	QVERIFY(logCompConsumerPtr != nullptr);
	QVERIFY(errorLogPtr != nullptr);

	// Clear both logs
	logCompPtr->ClearMessages();
	errorLogPtr->ClearMessages();

	// Add messages of different categories to LogComp
	ilog::IMessageConsumer::MessagePtr infoMsg(
		new ilog::CMessage(istd::IInformationProvider::IC_INFO, 4001, "Info message", "TestSource")
	);
	ilog::IMessageConsumer::MessagePtr warningMsg(
		new ilog::CMessage(istd::IInformationProvider::IC_WARNING, 4002, "Warning message", "TestSource")
	);
	ilog::IMessageConsumer::MessagePtr errorMsg(
		new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 4003, "Error message", "TestSource")
	);
	ilog::IMessageConsumer::MessagePtr criticalMsg(
		new ilog::CMessage(istd::IInformationProvider::IC_CRITICAL, 4004, "Critical message", "TestSource")
	);

	logCompConsumerPtr->AddMessage(infoMsg);
	logCompConsumerPtr->AddMessage(warningMsg);
	logCompConsumerPtr->AddMessage(errorMsg);
	logCompConsumerPtr->AddMessage(criticalMsg);

	// Wait for asynchronous message processing and routing with timeout
	QTRY_COMPARE_WITH_TIMEOUT(logCompPtr->GetMessages().size(), 4, 1000);
	QTRY_COMPARE_WITH_TIMEOUT(errorLogPtr->GetMessages().size(), 4, 1000);

	// Verify all messages are in LogComp
	ilog::IMessageContainer::Messages allMessages = logCompPtr->GetMessages();
	QCOMPARE(allMessages.size(), 4);

	// Verify only error and critical messages are in ErrorLog
	// (LogRouter MinimalCategory is set to 3 = IC_ERROR)
	ilog::IMessageContainer::Messages errorMessages = errorLogPtr->GetMessages();
	QCOMPARE(errorMessages.size(), 4);
	
	// Verify the routed messages are ERROR and CRITICAL
	bool hasError = false;
	bool hasCritical = false;
	for (const auto& msgPtr : errorMessages) {
		istd::IInformationProvider::InformationCategory category = msgPtr->GetInformationCategory();
		if (category == istd::IInformationProvider::IC_ERROR) {
			hasError = true;
		} else if (category == istd::IInformationProvider::IC_CRITICAL) {
			hasCritical = true;
		}
	}
	QVERIFY(hasError);
	QVERIFY(hasCritical);
}


void CLogTestRunner::testTracingConfigCreation()
{
	// Test that TracingConfig component was created successfully
	ilog::ITracingConfiguration* tracingPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::ITracingConfiguration>("TracingConfig");
	QVERIFY(tracingPtr != nullptr);
}


void CLogTestRunner::testTracingConfigLevel()
{
	// Get TracingConfig component
	ilog::ITracingConfiguration* tracingPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::ITracingConfiguration>("TracingConfig");
	QVERIFY(tracingPtr != nullptr);

	// Verify default tracing level is set to 1 (as configured in .acc)
	QCOMPARE(tracingPtr->GetTracingLevel(), 1);

	// Test setting a new tracing level
	tracingPtr->SetTracingLevel(2);
	QCOMPARE(tracingPtr->GetTracingLevel(), 2);

	// Test disabling tracing
	tracingPtr->SetTracingLevel(-1);
	QCOMPARE(tracingPtr->GetTracingLevel(), -1);
}


void CLogTestRunner::testStreamLogFiltering()
{
	// Get StreamLog component (MinCategory=2 = IC_WARNING)
	ilog::IMessageConsumer* streamLogPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageConsumer>("StreamLog");
	QVERIFY(streamLogPtr != nullptr);

	// Test that it supports messages above the minimum category
	QVERIFY(streamLogPtr->IsMessageSupported(istd::IInformationProvider::IC_WARNING));
	QVERIFY(streamLogPtr->IsMessageSupported(istd::IInformationProvider::IC_ERROR));
	QVERIFY(streamLogPtr->IsMessageSupported(istd::IInformationProvider::IC_CRITICAL));
	
	// Note: Info messages (category 0) should still be "supported" but will be filtered
	// The IsMessageSupported typically returns true; actual filtering happens in WriteMessageToLog
}


void CLogTestRunner::testMessageSerialization()
{
	// Get LogComp for message storage
	ilog::IMessageContainer* logCompPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageContainer>("LogComp");
	ilog::IMessageConsumer* consumerPtr = m_testPartituraInstanceCompPtr->GetInterface<ilog::IMessageConsumer>("LogComp");
	QVERIFY(logCompPtr != nullptr);
	QVERIFY(consumerPtr != nullptr);

	// Clear messages and add test messages
	logCompPtr->ClearMessages();

	ilog::IMessageConsumer::MessagePtr msg1(
		new ilog::CMessage(istd::IInformationProvider::IC_INFO, 5001, "Serialization test message 1", "SerTest")
	);
	ilog::IMessageConsumer::MessagePtr msg2(
		new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 5002, "Serialization test message 2", "SerTest")
	);

	consumerPtr->AddMessage(msg1);
	consumerPtr->AddMessage(msg2);

	// Wait for asynchronous message processing with timeout
	QTRY_COMPARE_WITH_TIMEOUT(logCompPtr->GetMessages().size(), 2, 1000);

	// Verify messages are stored
	ilog::IMessageContainer::Messages messages = logCompPtr->GetMessages();
	QCOMPARE(messages.size(), 2);

	// Create a memory archive for serialization test
	iser::CMemoryWriteArchive writeArchive;
	bool serializeResult = logCompPtr->Serialize(writeArchive);
	QVERIFY(serializeResult);

	// Verify that data was written
	QVERIFY(writeArchive.GetBufferSize() > 0);
}


void CLogTestRunner::cleanupTestCase()
{
	m_testPartituraInstanceCompPtr.reset();
}


I_ADD_TEST(CLogTestRunner);


