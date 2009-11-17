#include "iqtgui/CAboutWidgetGuiComp.h"


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CAboutWidgetGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
	
	if (m_logoPathAttrPtr.IsValid()){
		LogoLabel->setPixmap(QPixmap(iqt::GetQString(*m_logoPathAttrPtr)));
		LogoLabel->setVisible(true);
	}
	else{
		LogoLabel->setVisible(false);
	}

	if (m_descriptionTextAttrPtr.IsValid()){
		DescriptionLabel->setText(iqt::GetQString(*m_descriptionTextAttrPtr));
		DescriptionFrame->setVisible(true);
	}
	else{
		DescriptionFrame->setVisible(false);
	}

	if (m_applicationInfoCompPtr.IsValid()){
		ApplicationNameLabel->setText(iqt::GetQString(m_applicationInfoCompPtr->GetApplicationName()));

		QGridLayout* layoutPtr = dynamic_cast<QGridLayout*>(VersionsFrame->layout());
		if (layoutPtr != NULL){
			if (m_versionIdsAttrPtr.IsValid()){
				int versionsCount = m_versionIdsAttrPtr.GetCount();

				for (int i = 0; i < versionsCount; ++i){
					int versionId = m_versionIdsAttrPtr[i];

					I_DWORD version;
					if (m_applicationInfoCompPtr->GetVersionNumber(versionId, version)){
						istd::CString description = (i < m_versionNamesAttrPtr.GetCount())?
									m_versionNamesAttrPtr[i]:
									m_applicationInfoCompPtr->GetVersionIdDescription(versionId);
						istd::CString versionText = m_applicationInfoCompPtr->GetEncodedVersionName(versionId, version);

						QLabel* descriptionLabelPtr = new QLabel(iqt::GetQString(description), VersionsFrame);
						layoutPtr->addWidget(descriptionLabelPtr, i, 0);

						QLabel* versionLabelPtr = new QLabel(iqt::GetQString(versionText), VersionsFrame);
						layoutPtr->addWidget(versionLabelPtr, i, 1);
					}
				}
			}
			else{
				int rowCount = 0;
				
				iser::IVersionInfo::VersionIds ids = m_applicationInfoCompPtr->GetVersionIds();
				for (		iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
							iter != ids.end();
							++iter){
					int versionId = *iter;

					I_DWORD version;
					if (m_applicationInfoCompPtr->GetVersionNumber(versionId, version)){
						istd::CString description = m_applicationInfoCompPtr->GetVersionIdDescription(versionId);;
						istd::CString versionText = m_applicationInfoCompPtr->GetEncodedVersionName(versionId, version);

						QLabel* descriptionLabelPtr = new QLabel(iqt::GetQString(description), VersionsFrame);
						layoutPtr->addWidget(descriptionLabelPtr, rowCount, 0);

						QLabel* versionLabelPtr = new QLabel(iqt::GetQString(versionText), VersionsFrame);
						layoutPtr->addWidget(versionLabelPtr, rowCount++, 1);
					}
				}
			}
		}
	}
}


void CAboutWidgetGuiComp::OnRetranslate()
{
}


} // namespace iqtgui


