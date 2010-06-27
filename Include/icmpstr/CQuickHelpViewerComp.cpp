#include "icmpstr/CQuickHelpViewerComp.h"


// Qt includes
#include <QProcess>


namespace icmpstr
{


// reimplemented (idoc::IHelpViewer)

void CQuickHelpViewerComp::ShowHelp(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr)
{
	if (!IsGuiCreated()){
		return;
	}

	bool hasDescrFile = false;
	if (m_descriptionFileProviderCompPtr.IsValid()){
		m_descrFilePath = m_descriptionFileProviderCompPtr->GetHelpFilePath(contextText, contextObjectPtr);

		hasDescrFile = (m_descriptionFileProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr) > 0);
		if (hasDescrFile){
			QUrl url = QUrl::fromLocalFile(iqt::GetQString(m_descrFilePath));

			DescriptionBrowser->setSource(url);
		}
	}

	bool isEditAvailable = m_docuEditorFileParamsCompPtr.IsValid() && !m_docuEditorFileParamsCompPtr->GetPath().IsEmpty();
	NewButton->setVisible(isEditAvailable);
	EditButton->setVisible(isEditAvailable);

	DescriptionFrame->setVisible(hasDescrFile);
	NoDescriptionFrame->setVisible(!hasDescrFile);

	if (	m_technicalFileProviderCompPtr.IsValid() &&
			(m_technicalFileProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr) > 0)){
		m_techFilePath = m_descriptionFileProviderCompPtr->GetHelpFilePath(contextText, contextObjectPtr);

		ShowTechButton->setVisible(true);
	}
	else{
		ShowTechButton->setVisible(true);
	}
}


// protected slots

void CQuickHelpViewerComp::on_EditButton_clicked()
{
	if (!m_docuEditorFileParamsCompPtr.IsValid()){
		return;
	}

	istd::CString editorPath = m_docuEditorFileParamsCompPtr->GetPath();
	
	if (editorPath.IsEmpty()){
		return;
	}

	QStringList parameters;
	parameters << iqt::GetQString(m_descrFilePath);

	QProcess::startDetached(iqt::GetQString(editorPath), parameters);
}


void CQuickHelpViewerComp::on_NewButton_clicked()
{
	on_EditButton_clicked();
}


void CQuickHelpViewerComp::on_ShowTechButton_clicked()
{
	if (!m_techDocuViewerFileParamsCompPtr.IsValid()){
		return;
	}

	istd::CString editorPath = m_techDocuViewerFileParamsCompPtr->GetPath();
	
	if (editorPath.IsEmpty()){
		return;
	}

	QStringList parameters;
	parameters << iqt::GetQString(m_techFilePath);

	QProcess::startDetached(iqt::GetQString(editorPath), parameters);
}


} // namespace icmpstr


