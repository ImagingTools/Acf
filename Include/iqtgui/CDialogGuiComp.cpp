#include "iqtgui/CDialogGuiComp.h"


// Qt includes
#include <QtGui/QIcon>
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#else
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#endif


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IDialog)

int CDialogGuiComp::ExecuteDialog(IGuiObject* parentPtr)
{
	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr(CreateComponentDialog(QDialogButtonBox::Close, parentPtr));
	if (dialogPtr.IsValid()){
		if (*m_isModalAttrPtr){
			return dialogPtr->exec();
		}
		else{
			dialogPtr->setModal(false);
			dialogPtr->setAttribute(Qt::WA_DeleteOnClose);

			dialogPtr->show();

			dialogPtr.PopPtr();

			return QDialog::Accepted;
		}
	}

	return QDialog::Rejected;
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CDialogGuiComp::GetCommands() const
{
	return &m_rootCommand;
}


// protected methods

iqtgui::CGuiComponentDialog* CDialogGuiComp::CreateComponentDialog(int buttons, IGuiObject* parentPtr) const
{
	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr;

	if (m_guiCompPtr.IsValid()){
		QWidget* parentWidgetPtr = (parentPtr != NULL)? parentPtr->GetWidget(): NULL;

		dialogPtr.SetPtr(
					new iqtgui::CGuiComponentDialog(
								m_guiCompPtr.GetPtr(),
								buttons,
								true,
								parentWidgetPtr));

		if (m_dialogTitleAttrPtr.IsValid()){
			dialogPtr->setWindowTitle((*m_dialogTitleAttrPtr));
		}
		else{
			dialogPtr->setWindowTitle(QCoreApplication::applicationName());
		}

		if (m_dialogIconPathAttrPtr.IsValid()){
			dialogPtr->setWindowIcon(QIcon(*m_dialogIconPathAttrPtr));
		}
		else{
			dialogPtr->setWindowIcon(QApplication::windowIcon());
		}

		dialogPtr->SetDialogGeometry(m_initialDialogSizeAttrPtr.IsValid() ? *m_initialDialogSizeAttrPtr : 0.0);
	}

	return dialogPtr.PopPtr();
}


// reimplemented (icomp::CComponentBase)

void CDialogGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_rootMenuCommand.SetName(*m_rootMenuNameAttrPtr);
	m_dialogCommand.SetVisuals(*m_menuNameAttrPtr, *m_menuNameAttrPtr, *m_menuDescriptionAttrPtr);

	m_rootMenuCommand.InsertChild(&m_dialogCommand);
	m_rootCommand.InsertChild(&m_rootMenuCommand);

	connect(&m_dialogCommand, SIGNAL(triggered()), this, SLOT(OnCommandActivated()));
}


// protected slots

void CDialogGuiComp::OnCommandActivated()
{
	ExecuteDialog(NULL);
}


} // namespace iqtgui


