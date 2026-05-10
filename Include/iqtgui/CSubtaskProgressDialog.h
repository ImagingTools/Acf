// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtWidgets/QDialog>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtCore/QMap>

// ACF includes
#include <ibase/CCumulatedProgressManagerBase.h>


namespace iqtgui
{


/**
	Progress dialog that displays progress of subtasks.

	This dialog extends QDialog and implements ibase::CCumulatedProgressManagerBase
	to show both overall progress and individual subtask progress with status indicators.

	Features:
	- Overall progress bar showing cumulative progress
	- Dynamic list of subtasks with individual progress bars
	- Status indicators: completed (checkmark), running (arrow), pending (circle)
	- Cancel button functionality
	- Thread-safe signal/slot communication for GUI updates

	Usage example:
	\code
	CSubtaskProgressDialog dialog("Processing Data", "Initializing...", parentWidget);
	dialog.show();

	// Create subtask managers
	auto subtask1 = dialog.CreateSubtaskManager("load", "Loading files", 0.3);
	auto subtask2 = dialog.CreateSubtaskManager("process", "Processing images", 0.5);
	auto subtask3 = dialog.CreateSubtaskManager("save", "Saving results", 0.2);

	// Start logging progress for each subtask
	auto logger1 = subtask1->StartProgressLogger(true);
	logger1->OnProgress(0.5); // 50% complete
	// ...
	\endcode
*/
class CSubtaskProgressDialog: public QDialog, public ibase::CCumulatedProgressManagerBase
{
	Q_OBJECT

public:
	typedef QDialog BaseClass;

	/**
		Create a subtask progress dialog.
		\param title Dialog window title
		\param defaultText Default text shown when no tasks are active
		\param parentWidget Parent widget (optional)
	*/
	CSubtaskProgressDialog(
		const QString& title,
		const QString& defaultText,
		QWidget* parentWidget = nullptr);

	virtual ~CSubtaskProgressDialog();

protected:
	// reimplemented (ibase::CCumulatedProgressManagerBase)
	virtual void OnProgressChanged(double cumulatedValue) override;
	virtual void OnTasksChanged() override;

Q_SIGNALS:
	/**
		Signal emitted when overall progress changes.
		Used for thread-safe GUI updates via queued connection.
	*/
	void OverallProgressChanged(int progress);

	/**
		Signal emitted when subtask list or progress changes.
		Used for thread-safe GUI updates via queued connection.
	*/
	void SubtasksUpdated();

private Q_SLOTS:
	void OnCancelButtonClicked();
	void OnOverallProgressChanged(int progress);
	void OnSubtasksUpdated();

private:
	struct SubtaskWidget
	{
		QLabel* statusLabelPtr;
		QLabel* descriptionLabelPtr;
		QProgressBar* progressBarPtr;
		QLabel* percentLabelPtr;
		QWidget* containerPtr;
	};

	void CreateSubtaskWidget(const QByteArray& taskId, const QString& description);
	void UpdateSubtaskWidget(const QByteArray& taskId, double progress, TaskStatus status);
	void RemoveSubtaskWidget(const QByteArray& taskId);
	QString GetStatusIcon(TaskStatus status) const;
	QString GetStatusStyleSheet(TaskStatus status) const;

	QLabel* m_overallLabelPtr;
	QProgressBar* m_overallProgressBarPtr;
	QLabel* m_subtasksLabelPtr;
	QWidget* m_subtasksContainerPtr;
	QVBoxLayout* m_subtasksLayoutPtr;
	QPushButton* m_cancelButtonPtr;

	QMap<QByteArray, SubtaskWidget> m_subtaskWidgets;

	QString m_defaultText;
	int m_lastProgressValue;
};


} // namespace iqtgui


