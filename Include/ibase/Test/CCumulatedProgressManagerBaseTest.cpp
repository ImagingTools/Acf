// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/Test/CCumulatedProgressManagerBaseTest.h>


// ACF includes
#include <ibase/CCumulatedProgressManagerBase.h>


void CCumulatedProgressManagerBaseTest::DoOverflowTest1()
{
	ibase::CCumulatedProgressManagerBase progress;

	std::unique_ptr<ibase::IProgressLogger> loggerPtr = progress.StartProgressLogger();
	loggerPtr->OnProgress(1);

	std::unique_ptr<ibase::IProgressManager> subtaskPtr = progress.CreateSubtaskManager("subtask1", "", 0.5);
	std::unique_ptr<ibase::IProgressLogger> subtaskLoggerPtr = subtaskPtr->StartProgressLogger();
	subtaskLoggerPtr->OnProgress(1);

	double cumulatedProgress = progress.GetCumulatedProgress();

	QVERIFY(qFuzzyCompare(cumulatedProgress, 1.5));
}


void CCumulatedProgressManagerBaseTest::TestBasicProgressLogger()
{
	ibase::CCumulatedProgressManagerBase progress;

	// Test initial progress is 0
	QCOMPARE(progress.GetCumulatedProgress(), 0.0);

	// Start progress logger and report progress
	std::unique_ptr<ibase::IProgressLogger> loggerPtr = progress.StartProgressLogger();
	QCOMPARE(progress.GetCumulatedProgress(), 0.0);

	loggerPtr->OnProgress(0.5);
	QCOMPARE(progress.GetCumulatedProgress(), 0.5);

	loggerPtr->OnProgress(1.0);
	QCOMPARE(progress.GetCumulatedProgress(), 1.0);
}


void CCumulatedProgressManagerBaseTest::TestMultipleSubtasks()
{
	ibase::CCumulatedProgressManagerBase progress;

	// Create subtasks with different weights
	std::unique_ptr<ibase::IProgressManager> subtask1Ptr = progress.CreateSubtaskManager("task1", "Task 1", 1.0);
	std::unique_ptr<ibase::IProgressManager> subtask2Ptr = progress.CreateSubtaskManager("task2", "Task 2", 2.0);

	// Start loggers for subtasks
	std::unique_ptr<ibase::IProgressLogger> logger1Ptr = subtask1Ptr->StartProgressLogger();
	std::unique_ptr<ibase::IProgressLogger> logger2Ptr = subtask2Ptr->StartProgressLogger();

	// Report progress on first subtask (weight 1.0, progress 0.5)
	logger1Ptr->OnProgress(0.5);
	QCOMPARE(progress.GetCumulatedProgress(), 0.5);

	// Report progress on second subtask (weight 2.0, progress 0.5)
	logger2Ptr->OnProgress(0.5);
	QCOMPARE(progress.GetCumulatedProgress(), 1.5); // 0.5 + 1.0

	// Complete first subtask
	logger1Ptr->OnProgress(1.0);
	QCOMPARE(progress.GetCumulatedProgress(), 2.0); // 1.0 + 1.0

	// Complete second subtask
	logger2Ptr->OnProgress(1.0);
	QCOMPARE(progress.GetCumulatedProgress(), 3.0); // 1.0 + 2.0
}


void CCumulatedProgressManagerBaseTest::TestCancellation()
{
	ibase::CCumulatedProgressManagerBase progress;

	// Initially not cancelable and not canceled
	QVERIFY(!progress.IsCancelable());
	QVERIFY(!progress.IsCanceled());

	// Start a non-cancelable logger
	std::unique_ptr<ibase::IProgressLogger> loggerPtr = progress.StartProgressLogger(false);
	QVERIFY(!progress.IsCancelable());
	QVERIFY(!progress.IsCanceled());

	// Reset and test cancelable logger
	progress.ResetProgressManager();
	std::unique_ptr<ibase::IProgressLogger> cancelableLoggerPtr = progress.StartProgressLogger(true);
	QVERIFY(progress.IsCancelable());
	QVERIFY(!progress.IsCanceled());

	// Set canceled state
	progress.SetCanceled(true);
	QVERIFY(progress.IsCanceled());
	QVERIFY(cancelableLoggerPtr->IsCanceled());

	// Unset canceled state
	progress.SetCanceled(false);
	QVERIFY(!progress.IsCanceled());
	QVERIFY(!cancelableLoggerPtr->IsCanceled());
}


void CCumulatedProgressManagerBaseTest::TestGetProcessedTasksSorted()
{
	ibase::CCumulatedProgressManagerBase progress;

	// Create subtasks with different weights
	std::unique_ptr<ibase::IProgressManager> task1Ptr = progress.CreateSubtaskManager("task1", "Task 1", 3.0);
	std::unique_ptr<ibase::IProgressManager> task2Ptr = progress.CreateSubtaskManager("task2", "Task 2", 1.0);
	std::unique_ptr<ibase::IProgressManager> task3Ptr = progress.CreateSubtaskManager("task3", "Task 3", 2.0);

	// Start loggers
	std::unique_ptr<ibase::IProgressLogger> logger1Ptr = task1Ptr->StartProgressLogger();
	std::unique_ptr<ibase::IProgressLogger> logger2Ptr = task2Ptr->StartProgressLogger();
	std::unique_ptr<ibase::IProgressLogger> logger3Ptr = task3Ptr->StartProgressLogger();

	// Get unsorted tasks
	std::vector<ibase::CCumulatedProgressManagerBase::TaskProgressInfo> unsortedTasks = progress.GetProcessedTasks(false);
	QCOMPARE(unsortedTasks.size(), size_t(3));

	// Get sorted tasks (sorted by weight ascending)
	std::vector<ibase::CCumulatedProgressManagerBase::TaskProgressInfo> sortedTasks = progress.GetProcessedTasks(true);
	QCOMPARE(sortedTasks.size(), size_t(3));
	// Tasks should be sorted by weight: task2 (1.0), task3 (2.0), task1 (3.0)
	QCOMPARE(sortedTasks[0].id, QByteArray("task2"));
	QCOMPARE(sortedTasks[1].id, QByteArray("task3"));
	QCOMPARE(sortedTasks[2].id, QByteArray("task1"));
}


void CCumulatedProgressManagerBaseTest::TestGetProcessedTasksMaxCount()
{
	ibase::CCumulatedProgressManagerBase progress;

	// Create multiple subtasks
	std::unique_ptr<ibase::IProgressManager> task1Ptr = progress.CreateSubtaskManager("task1", "Task 1", 1.0);
	std::unique_ptr<ibase::IProgressManager> task2Ptr = progress.CreateSubtaskManager("task2", "Task 2", 2.0);
	std::unique_ptr<ibase::IProgressManager> task3Ptr = progress.CreateSubtaskManager("task3", "Task 3", 3.0);

	// Start loggers
	std::unique_ptr<ibase::IProgressLogger> logger1Ptr = task1Ptr->StartProgressLogger();
	std::unique_ptr<ibase::IProgressLogger> logger2Ptr = task2Ptr->StartProgressLogger();
	std::unique_ptr<ibase::IProgressLogger> logger3Ptr = task3Ptr->StartProgressLogger();

	// Get all tasks
	std::vector<ibase::CCumulatedProgressManagerBase::TaskProgressInfo> allTasks = progress.GetProcessedTasks(false, -1);
	QCOMPARE(allTasks.size(), size_t(3));

	// Get limited number of tasks
	std::vector<ibase::CCumulatedProgressManagerBase::TaskProgressInfo> limitedTasks = progress.GetProcessedTasks(false, 2);
	QCOMPARE(limitedTasks.size(), size_t(2));
}


void CCumulatedProgressManagerBaseTest::TestResetProgressManager()
{
	ibase::CCumulatedProgressManagerBase progress;

	// Create subtask and logger
	std::unique_ptr<ibase::IProgressManager> subtaskPtr = progress.CreateSubtaskManager("task1", "Task 1", 1.0);
	std::unique_ptr<ibase::IProgressLogger> loggerPtr = subtaskPtr->StartProgressLogger();
	loggerPtr->OnProgress(0.5);

	// Verify progress and tasks exist
	QCOMPARE(progress.GetCumulatedProgress(), 0.5);
	QCOMPARE(progress.GetProcessedTasks().size(), size_t(1));

	// Set canceled state
	std::unique_ptr<ibase::IProgressLogger> cancelableLoggerPtr = progress.StartProgressLogger(true);
	progress.SetCanceled(true);
	QVERIFY(progress.IsCanceled());
	QVERIFY(progress.IsCancelable());

	// Reset progress manager
	progress.ResetProgressManager();

	// Verify everything is reset
	QCOMPARE(progress.GetCumulatedProgress(), 0.0);
	QCOMPARE(progress.GetProcessedTasks().size(), size_t(0));
	QVERIFY(!progress.IsCanceled());
	QVERIFY(!progress.IsCancelable());
}


void CCumulatedProgressManagerBaseTest::TestTaskStatusTransitions()
{
	ibase::CCumulatedProgressManagerBase progress;

	// Create subtask
	std::unique_ptr<ibase::IProgressManager> subtaskPtr = progress.CreateSubtaskManager("task1", "Task 1", 1.0);

	// Check task is created but not started
	std::vector<ibase::CCumulatedProgressManagerBase::TaskProgressInfo> tasks = progress.GetProcessedTasks();
	QCOMPARE(tasks.size(), size_t(1));
	QCOMPARE(tasks[0].status, ibase::CCumulatedProgressManagerBase::TaskStatus::Created);
	QCOMPARE(tasks[0].progress, 0.0);

	// Start logger - task should be running
	std::unique_ptr<ibase::IProgressLogger> loggerPtr = subtaskPtr->StartProgressLogger();
	tasks = progress.GetProcessedTasks();
	QCOMPARE(tasks.size(), size_t(1));
	QCOMPARE(tasks[0].status, ibase::CCumulatedProgressManagerBase::TaskStatus::Running);
	QCOMPARE(tasks[0].progress, 0.0);

	// Report progress
	loggerPtr->OnProgress(0.5);
	tasks = progress.GetProcessedTasks();
	QCOMPARE(tasks.size(), size_t(1));
	QCOMPARE(tasks[0].status, ibase::CCumulatedProgressManagerBase::TaskStatus::Running);
	QCOMPARE(tasks[0].progress, 0.5);

	// Complete task
	loggerPtr->OnProgress(1.0);
	tasks = progress.GetProcessedTasks();
	QCOMPARE(tasks.size(), size_t(1));
	QCOMPARE(tasks[0].status, ibase::CCumulatedProgressManagerBase::TaskStatus::Running);
	QCOMPARE(tasks[0].progress, 1.0);

	// Close logger - task should be finished but remain in list until destroyed
	loggerPtr.reset();
	tasks = progress.GetProcessedTasks();
	QCOMPARE(tasks.size(), size_t(1));
	QCOMPARE(tasks[0].status, ibase::CCumulatedProgressManagerBase::TaskStatus::Finished);
	QCOMPARE(tasks[0].progress, 1.0);
}


void CCumulatedProgressManagerBaseTest::TestNestedSubtasks()
{
	ibase::CCumulatedProgressManagerBase progress;

	// Create parent subtask with weight 2.0
	std::unique_ptr<ibase::IProgressManager> parentTaskPtr = progress.CreateSubtaskManager("parent", "Parent Task", 2.0);

	// Create nested subtask under parent with weight 1.0
	std::unique_ptr<ibase::IProgressManager> childTaskPtr = parentTaskPtr->CreateSubtaskManager("child", "Child Task", 1.0);

	// Start logger for child task
	std::unique_ptr<ibase::IProgressLogger> childLoggerPtr = childTaskPtr->StartProgressLogger();
	childLoggerPtr->OnProgress(0.5);

	// The parent task should report 0.5 progress to the main progress manager
	// Main progress should be: 2.0 (parent weight) * 0.5 (parent progress) = 1.0
	QCOMPARE(progress.GetCumulatedProgress(), 1.0);

	// Complete child task
	childLoggerPtr->OnProgress(1.0);
	QCOMPARE(progress.GetCumulatedProgress(), 2.0);

	// Close child logger
	childLoggerPtr.reset();

	// Parent task should now be finished (no more subtasks)
	// Verify parent task weight is counted in closed tasks
	QCOMPARE(progress.GetCumulatedProgress(), 2.0);
}


void CCumulatedProgressManagerBaseTest::TestDefaultTaskInfo()
{
	ibase::CCumulatedProgressManagerBase::TaskInfo defaultTaskInfo{"default_id", "Default Description"};
	ibase::CCumulatedProgressManagerBase progress(defaultTaskInfo);

	// Start logger without description - should use default
	std::unique_ptr<ibase::IProgressLogger> loggerPtr = progress.StartProgressLogger();

	std::vector<ibase::CCumulatedProgressManagerBase::TaskProgressInfo> tasks = progress.GetProcessedTasks();
	QCOMPARE(tasks.size(), size_t(1));
	QCOMPARE(tasks[0].id, QByteArray("default_id"));
	QCOMPARE(tasks[0].description, QString("Default Description"));

	// Reset and change default task info
	progress.ResetProgressManager();
	ibase::CCumulatedProgressManagerBase::TaskInfo newTaskInfo{"new_id", "New Description"};
	progress.SetDefaultTaskInfo(newTaskInfo);

	// Start logger with custom description - should override default
	std::unique_ptr<ibase::IProgressLogger> loggerPtr2 = progress.StartProgressLogger(false, "Custom Description");

	tasks = progress.GetProcessedTasks();
	QCOMPARE(tasks.size(), size_t(1));
	QCOMPARE(tasks[0].id, QByteArray("new_id"));
	QCOMPARE(tasks[0].description, QString("Custom Description"));
}


I_ADD_TEST(CCumulatedProgressManagerBaseTest);


