#include <iqtgui/CProgressGuiBridge.h>


namespace iqtgui
{


// public methods

CProgressGuiBridge::CProgressGuiBridge(QProgressBar* progressWidget, QAbstractButton* cancelButton)
	:m_progresBarPtr(progressWidget),
	m_cancelButtonPtr(cancelButton),
	m_isCanceled(false)
{
	if (m_cancelButtonPtr != NULL){
		connect(m_cancelButtonPtr, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));
	}
}


// reimplemented (ibase::IProgressManager)

int CProgressGuiBridge::BeginProgressSession(
			const QByteArray& /*progressId*/,
			const QString& /*description*/,
			bool /*isCancelable*/)
{
	return 0;
}


void CProgressGuiBridge::EndProgressSession(int /*sessionId*/)
{
	if (m_progresBarPtr != NULL){
		m_progresBarPtr->setValue(0);
	}
}


void CProgressGuiBridge::OnProgress(int /*sessionId*/, double currentProgress)
{
	if (m_progresBarPtr != NULL){
		m_progresBarPtr->setValue(currentProgress * 100);
	}
}


bool CProgressGuiBridge::IsCanceled(int /*sessionId*/) const
{
	return m_isCanceled;
}


// protected slots

void CProgressGuiBridge::OnCancelButtonClicked()
{
	m_isCanceled = true;
}


} // namespace iqtgui


