#include "iqtproc/CProgressManagerGuiComp.h"


#include <QCoreApplication>


namespace iqtproc
{


CProgressManagerGuiComp::CProgressManagerGuiComp()
:	m_nextSessionId(0),
	m_progressSum(0),
	m_isCanceled(false),
	m_cancelableSessionsCount(0)
{
}


// reimplemented (iproc::IProgressManager)

int CProgressManagerGuiComp::BeginProgressSession(const iser::CArchiveTag& /*progressTag*/, bool isCancelable)
{
	if (m_isCanceled){
		return -1;
	}

	int id = m_nextSessionId++;

	ProgressInfo& info = m_progressMap[id];
	info.progress = 0;
	info.isCancelable = isCancelable;

	if (isCancelable){
		++m_cancelableSessionsCount;
	}

	if (IsGuiCreated()){
		UpdateVisibleComponents();

		QCoreApplication::processEvents();
	}

	return id;
}


void CProgressManagerGuiComp::EndProgressSession(int sessionId)
{
	ProgressMap::iterator iter = m_progressMap.find(sessionId);
	I_ASSERT(iter != m_progressMap.end());

	const ProgressInfo& info = iter->second;
	m_progressSum -= info.progress;

	if (info.isCancelable){
		--m_cancelableSessionsCount;
	}

	m_progressMap.erase(iter);

	if (m_progressMap.empty()){
		m_progressSum = 0;
		m_isCanceled = false;
	}

	if (IsGuiCreated()){
		UpdateVisibleComponents();

		QCoreApplication::processEvents();
	}
}


void CProgressManagerGuiComp::OnProgress(int sessionId, double currentProgress)
{
	ProgressMap::iterator iter = m_progressMap.find(sessionId);
	I_ASSERT(iter != m_progressMap.end());

	m_progressSum += currentProgress - iter->second.progress;
	iter->second.progress = currentProgress;

	if (IsGuiCreated()){
		UpdateProgressBar();

		QCoreApplication::processEvents();
	}
}


bool CProgressManagerGuiComp::IsCanceled(int /*sessionId*/) const
{
	return m_isCanceled;
}


// protected methods

void CProgressManagerGuiComp::UpdateVisibleComponents()
{
	QWidget* widgetPtr = GetWidget();
	if (widgetPtr != NULL){
		if (*m_automaticHideAttrPtr){
			widgetPtr->setVisible(!m_progressMap.empty());
		}

		CancelButton->setVisible(m_cancelableSessionsCount > 0);
		CancelButton->setEnabled(!m_isCanceled);

		UpdateProgressBar();
	}
}


void CProgressManagerGuiComp::UpdateProgressBar()
{
	if (m_progressMap.empty()){
		ProgressBar->setValue(0);
		ProgressBar->setEnabled(false);
	}
	else{
		ProgressBar->setValue(1000 * m_progressSum / m_progressMap.size());
		ProgressBar->setEnabled(true);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CProgressManagerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_descriptionAttrPtr.IsValid()){
		DescriptionLabel->setText(tr((*m_descriptionAttrPtr).ToString().c_str()));
		DescriptionLabel->setVisible(true);
	}
	else{
		DescriptionLabel->setVisible(false);
	}

	UpdateVisibleComponents();
}


// protected slots

void CProgressManagerGuiComp::on_CancelButton_clicked()
{
	UpdateVisibleComponents();

	m_isCanceled = true;
}


} // namespace iqtproc


