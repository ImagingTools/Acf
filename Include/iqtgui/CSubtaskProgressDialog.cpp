// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtgui/CSubtaskProgressDialog.h>


// Qt includes
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtCore/QCoreApplication>


namespace iqtgui
{


// public methods

CSubtaskProgressDialog::CSubtaskProgressDialog(
	const QString& title,
	const QString& defaultText,
	QWidget* parentWidget)
:	BaseClass(parentWidget),
	m_overallLabelPtr(nullptr),
	m_overallProgressBarPtr(nullptr),
	m_subtasksLabelPtr(nullptr),
	m_subtasksContainerPtr(nullptr),
	m_subtasksLayoutPtr(nullptr),
	m_cancelButtonPtr(nullptr),
	m_defaultText(defaultText),
	m_lastProgressValue(0)
{
	setWindowTitle(title);
	setWindowModality(Qt::WindowModal);
	setMinimumWidth(400);
	setMinimumHeight(200);

	// Main layout
	QVBoxLayout* mainLayoutPtr = new QVBoxLayout(this);
	mainLayoutPtr->setSpacing(10);
	mainLayoutPtr->setContentsMargins(15, 15, 15, 15);

	// Overall progress section
	m_overallLabelPtr = new QLabel(tr("Overall Progress:"), this);
	QFont boldFont = m_overallLabelPtr->font();
	boldFont.setBold(true);
	m_overallLabelPtr->setFont(boldFont);
	mainLayoutPtr->addWidget(m_overallLabelPtr);

	// Overall progress bar with percentage
	QHBoxLayout* overallBarLayoutPtr = new QHBoxLayout();
	m_overallProgressBarPtr = new QProgressBar(this);
	m_overallProgressBarPtr->setMinimum(0);
	m_overallProgressBarPtr->setMaximum(100);
	m_overallProgressBarPtr->setValue(0);
	m_overallProgressBarPtr->setTextVisible(true);
	m_overallProgressBarPtr->setFormat("%p%");
	overallBarLayoutPtr->addWidget(m_overallProgressBarPtr);
	mainLayoutPtr->addLayout(overallBarLayoutPtr);

	// Subtasks section (hidden when there is only one task)
	m_subtasksLabelPtr = new QLabel(tr("Subtasks:"), this);
	m_subtasksLabelPtr->setFont(boldFont);
	m_subtasksLabelPtr->setVisible(false);
	mainLayoutPtr->addWidget(m_subtasksLabelPtr);

	// Subtasks container with scroll capability
	m_subtasksContainerPtr = new QWidget(this);
	m_subtasksLayoutPtr = new QVBoxLayout(m_subtasksContainerPtr);
	m_subtasksLayoutPtr->setSpacing(8);
	m_subtasksLayoutPtr->setContentsMargins(10, 0, 0, 0);
	m_subtasksContainerPtr->setVisible(false);
	mainLayoutPtr->addWidget(m_subtasksContainerPtr);

	// Add stretch to push cancel button to bottom
	mainLayoutPtr->addStretch();

	// Cancel button
	QHBoxLayout* buttonLayoutPtr = new QHBoxLayout();
	buttonLayoutPtr->addStretch();
	m_cancelButtonPtr = new QPushButton(tr("Cancel"), this);
	m_cancelButtonPtr->setMinimumWidth(80);
	m_cancelButtonPtr->setEnabled(false);
	buttonLayoutPtr->addWidget(m_cancelButtonPtr);
	buttonLayoutPtr->addStretch();
	mainLayoutPtr->addLayout(buttonLayoutPtr);

	// Connect signals for thread-safe GUI updates
	connect(this, &CSubtaskProgressDialog::OverallProgressChanged,
			this, &CSubtaskProgressDialog::OnOverallProgressChanged,
			Qt::QueuedConnection);
	connect(this, &CSubtaskProgressDialog::SubtasksUpdated,
			this, &CSubtaskProgressDialog::OnSubtasksUpdated,
			Qt::QueuedConnection);
	connect(m_cancelButtonPtr, &QPushButton::clicked,
			this, &CSubtaskProgressDialog::OnCancelButtonClicked);
}


CSubtaskProgressDialog::~CSubtaskProgressDialog()
{
	// Widgets are automatically deleted by Qt parent-child mechanism
}


// protected methods

// reimplemented (ibase::CCumulatedProgressManagerBase)

void CSubtaskProgressDialog::OnProgressChanged(double cumulatedValue)
{
	int progressValue = static_cast<int>(cumulatedValue * 100);
	if (progressValue != m_lastProgressValue){
		m_lastProgressValue = progressValue;
		Q_EMIT OverallProgressChanged(progressValue);
	}
}


void CSubtaskProgressDialog::OnTasksChanged()
{
	Q_EMIT SubtasksUpdated();
}


// private slots

void CSubtaskProgressDialog::OnCancelButtonClicked()
{
	SetCanceled();
	m_cancelButtonPtr->setEnabled(false);
	m_cancelButtonPtr->setText(tr("Canceling..."));
}


void CSubtaskProgressDialog::OnOverallProgressChanged(int progress)
{
	m_overallProgressBarPtr->setValue(progress);
}


void CSubtaskProgressDialog::OnSubtasksUpdated()
{
	// Get current tasks
	auto tasks = GetProcessedTasks(false);

	// Track which tasks we've seen
	QSet<QByteArray> currentTaskIds;

	for (const auto& task : tasks){
		currentTaskIds.insert(task.id);

		if (!m_subtaskWidgets.contains(task.id)){
			// Create new widget for this task
			CreateSubtaskWidget(task.id, task.description);
		}

		// Update the widget
		UpdateSubtaskWidget(task.id, task.progress, task.status);
	}

	// Remove widgets for tasks that no longer exist
	QList<QByteArray> toRemove;
	for (auto it = m_subtaskWidgets.begin(); it != m_subtaskWidgets.end(); ++it){
		if (!currentTaskIds.contains(it.key())){
			toRemove.append(it.key());
		}
	}
	for (const auto& taskId : toRemove){
		RemoveSubtaskWidget(taskId);
	}

	// Show subtasks section only if there are multiple tasks
	const bool showSubtasks = tasks.size() > 1;
	m_subtasksLabelPtr->setVisible(showSubtasks);
	m_subtasksContainerPtr->setVisible(showSubtasks);

	// Update cancel button state
	m_cancelButtonPtr->setEnabled(IsCancelable() && !IsCanceled());

	// Adjust dialog size if needed
	adjustSize();
}


// private methods

void CSubtaskProgressDialog::CreateSubtaskWidget(const QByteArray& taskId, const QString& description)
{
	SubtaskWidget subtask;

	// Container for this subtask
	subtask.containerPtr = new QWidget(m_subtasksContainerPtr);
	QVBoxLayout* containerLayoutPtr = new QVBoxLayout(subtask.containerPtr);
	containerLayoutPtr->setSpacing(2);
	containerLayoutPtr->setContentsMargins(0, 0, 0, 0);

	// First row: status icon and description
	QHBoxLayout* labelLayoutPtr = new QHBoxLayout();
	labelLayoutPtr->setSpacing(5);

	subtask.statusLabelPtr = new QLabel(subtask.containerPtr);
	subtask.statusLabelPtr->setFixedWidth(16);
	labelLayoutPtr->addWidget(subtask.statusLabelPtr);

	subtask.descriptionLabelPtr = new QLabel(description, subtask.containerPtr);
	labelLayoutPtr->addWidget(subtask.descriptionLabelPtr);
	labelLayoutPtr->addStretch();

	containerLayoutPtr->addLayout(labelLayoutPtr);

	// Second row: progress bar and percentage
	QHBoxLayout* progressLayoutPtr = new QHBoxLayout();
	progressLayoutPtr->setContentsMargins(21, 0, 0, 0); // Indent to align with text

	subtask.progressBarPtr = new QProgressBar(subtask.containerPtr);
	subtask.progressBarPtr->setMinimum(0);
	subtask.progressBarPtr->setMaximum(100);
	subtask.progressBarPtr->setValue(0);
	subtask.progressBarPtr->setTextVisible(false);
	subtask.progressBarPtr->setMaximumHeight(12);
	progressLayoutPtr->addWidget(subtask.progressBarPtr);

	subtask.percentLabelPtr = new QLabel("0%", subtask.containerPtr);
	subtask.percentLabelPtr->setFixedWidth(40);
	subtask.percentLabelPtr->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	progressLayoutPtr->addWidget(subtask.percentLabelPtr);

	containerLayoutPtr->addLayout(progressLayoutPtr);

	// Add to layout
	m_subtasksLayoutPtr->addWidget(subtask.containerPtr);

	// Store widget
	m_subtaskWidgets[taskId] = subtask;
}


void CSubtaskProgressDialog::UpdateSubtaskWidget(const QByteArray& taskId, double progress, TaskStatus status)
{
	if (!m_subtaskWidgets.contains(taskId)){
		return;
	}

	SubtaskWidget& subtask = m_subtaskWidgets[taskId];

	// Update status icon
	subtask.statusLabelPtr->setText(GetStatusIcon(status));
	subtask.descriptionLabelPtr->setStyleSheet(GetStatusStyleSheet(status));

	// Update progress
	int progressPercent = static_cast<int>(progress * 100);
	subtask.progressBarPtr->setValue(progressPercent);
	subtask.percentLabelPtr->setText(QString("%1%").arg(progressPercent));

	// Update progress bar color based on status
	QString barStyle;
	switch (status){
		case TaskStatus::Finished:
			barStyle = "QProgressBar::chunk { background-color: #4CAF50; }"; // Green
			break;
		case TaskStatus::Running:
			barStyle = "QProgressBar::chunk { background-color: #4285F4; }"; // Blue
			break;
		default:
			barStyle = "QProgressBar::chunk { background-color: #BDBDBD; }"; // Gray
			break;
	}
	subtask.progressBarPtr->setStyleSheet(barStyle);
}


void CSubtaskProgressDialog::RemoveSubtaskWidget(const QByteArray& taskId)
{
	if (!m_subtaskWidgets.contains(taskId)){
		return;
	}

	SubtaskWidget& subtask = m_subtaskWidgets[taskId];

	// Remove from layout and delete
	m_subtasksLayoutPtr->removeWidget(subtask.containerPtr);
	delete subtask.containerPtr;

	m_subtaskWidgets.remove(taskId);
}


QString CSubtaskProgressDialog::GetStatusIcon(TaskStatus status) const
{
	switch (status){
		case TaskStatus::Finished:
			return QString::fromUtf8("\u2713"); // Checkmark ✓
		case TaskStatus::Running:
			return QString::fromUtf8("\u25B6"); // Play arrow ►
		default:
			return QString::fromUtf8("\u25CB"); // Circle ○
	}
}


QString CSubtaskProgressDialog::GetStatusStyleSheet(TaskStatus status) const
{
	switch (status){
		case TaskStatus::Finished:
			return "color: #4CAF50;"; // Green
		case TaskStatus::Running:
			return "color: #4285F4;"; // Blue
		default:
			return "color: #9E9E9E;"; // Gray
	}
}


} // namespace iqtgui


