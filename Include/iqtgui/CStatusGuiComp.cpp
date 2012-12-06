#include "iqtgui/CStatusGuiComp.h"


// Qt includes
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QStatusBar>
#include <QtGui/QGroupBox>

// ACF includes
#include "i2d/CAffineTransformation2d.h"


namespace iqtgui
{


// reimplemented (iqtgui::TGuiObserverWrap)

void CStatusGuiComp::UpdateGui(int /*updateFlags*/)
{
	static QIcon stateUnknownIcon(":/Icons/StateUnknown.svg");
	static QIcon stateOkIcon(":/Icons/StateOk.svg");
	static QIcon stateWarningIcon(":/Icons/StateWarning.svg");
	static QIcon stateInvalidIcon(":/Icons/StateInvalid.svg");

	QIcon icon;

	istd::IInformationProvider* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		switch (objectPtr->GetInformationCategory()){
		case istd::IInformationProvider::IC_INFO:
			icon = stateOkIcon;
			break;

		case istd::IInformationProvider::IC_WARNING:
			icon = stateWarningIcon;
			break;

		case istd::IInformationProvider::IC_ERROR:
			icon = stateInvalidIcon;
			break;

		case istd::IInformationProvider::IC_CRITICAL:
			icon = stateInvalidIcon;
			break;

		default:
			icon = stateUnknownIcon;
			break;
		}
	}
	else{
		icon = stateUnknownIcon;
	}

	QLabel* labelPtr = GetQtWidget();
	Q_ASSERT(labelPtr != NULL);

	labelPtr->setPixmap(icon.pixmap(128));
}


// reimplemented (iqtgui::CGuiComponentBase)

void CStatusGuiComp::OnGuiCreated()
{
	QLabel* labelPtr = GetQtWidget();
	Q_ASSERT(labelPtr != NULL);

	labelPtr->setScaledContents(true);
}


} // namespace iqtgui


