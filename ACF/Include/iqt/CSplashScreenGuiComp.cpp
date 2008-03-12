#include "iqt/CSplashScreenGuiComp.h"


namespace iqt
{


// reimplemented (iqt::CGuiComponentBase)

void CSplashScreenGuiComp::OnGuiShown()
{
	istd::CString acfVersionText;
	istd::CString productVersionText;

	if (m_applicationInfoCompPtr.IsValid()){
		if (m_showAcfVersionAttrPtr.IsValid() && m_showAcfVersionAttrPtr->GetValue()){
			I_DWORD acfVersion = m_applicationInfoCompPtr->GetVersion(iser::IVersionInfo::FrameworkVersionId);
			acfVersionText = m_applicationInfoCompPtr->EncodeVersionName(acfVersion, iser::IVersionInfo::FrameworkVersionId);
		}
		if (m_showProductVersionAttrPtr.IsValid()){
			I_DWORD acfVersion = m_applicationInfoCompPtr->GetVersion(iser::IVersionInfo::UserVersionId);
			productVersionText = m_applicationInfoCompPtr->EncodeVersionName(acfVersion, iser::IVersionInfo::UserVersionId);
		}
	}

	if (!acfVersionText.empty()){
		AcfVersionLabel->setText(iqt::GetQString(acfVersionText));
	}
	AcfVersionDescriptionLabel->setVisible(!acfVersionText.empty());
	AcfVersionLabel->setVisible(!acfVersionText.empty());

	if (!productVersionText.empty()){
		VersionLabel->setText(iqt::GetQString(productVersionText));
	}
	VersionDescriptionLabel->setVisible(!productVersionText.empty());
	VersionLabel->setVisible(!productVersionText	.empty());

	if (m_productTypeAttrPtr.IsValid()){
		ProductTypeLabel->setText(iqt::GetQString(m_productTypeAttrPtr->GetValue()));
	}
	ProductTypeLabel->setVisible(m_productTypeAttrPtr.IsValid());

	if (m_copyrightTextAttrPtr.IsValid()){
		CopyrightLabel->setText(iqt::GetQString(m_copyrightTextAttrPtr->GetValue()));
	}
	CopyrightLabel->setVisible(m_copyrightTextAttrPtr.IsValid());

	QSplashScreen* splashScreenPtr = dynamic_cast<QSplashScreen*>(GetWidget());
	if (m_imagePathAttrPtr.IsValid() && (splashScreenPtr != NULL)){
		QPixmap image(GetQString(m_imagePathAttrPtr->GetValue()));
		splashScreenPtr->setPixmap(image);
	}
}


} // namespace iqt
