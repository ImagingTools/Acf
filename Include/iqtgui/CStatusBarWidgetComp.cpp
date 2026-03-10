// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtgui/CStatusBarWidgetComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IMainWindowComponent)

bool CStatusBarWidgetComp::AddToMainWindow(QMainWindow& mainWindow)
{
	if (m_statusBarWidgetsCompPtr.IsValid()){
		QStatusBar* statusBar = mainWindow.statusBar();
		Q_ASSERT(statusBar != NULL);

		statusBar->setSizeGripEnabled(*m_isSizeGripEnabledAttrPtr);

		if (*m_suppressWidgetFrameAttrPtr){
			statusBar->setStyleSheet("QStatusBar::item { border: 0px solid transparent }");
		}

		int widgetsCount = m_statusBarWidgetsCompPtr.GetCount();

		for (int widgetIndex = 0; widgetIndex < widgetsCount; widgetIndex++){
			iqtgui::IGuiObject* guiPtr = m_statusBarWidgetsCompPtr[widgetIndex];
			if (guiPtr != NULL){
				if (!guiPtr->IsGuiCreated()){
					if (guiPtr->CreateGui(statusBar)){
						statusBar->addPermanentWidget(guiPtr->GetWidget());
					}
					else{
						return false;
					}
				}
				else{
					I_CRITICAL();

					return false;
				}
			}
			else{
				I_CRITICAL();

				return false;
			}
		}
	}

	return false;
}


bool CStatusBarWidgetComp::RemoveFromMainWindow(QMainWindow& /*mainWindow*/)
{
	int widgetsCount = m_statusBarWidgetsCompPtr.GetCount();

	bool retVal = true;

	if (m_statusBarWidgetsCompPtr.IsValid()){
		for (int widgetIndex = 0; widgetIndex < widgetsCount; widgetIndex++){
			iqtgui::IGuiObject* guiPtr = m_statusBarWidgetsCompPtr[widgetIndex];
			if (guiPtr != NULL && guiPtr->IsGuiCreated()){
				retVal = guiPtr->DestroyGui() && retVal;
			}
		}
	}
	else{
		return false;
	}

	return retVal;
}


QString CStatusBarWidgetComp::GetTitle() const
{
	static QString emptyTitle;

	return emptyTitle;
}


int CStatusBarWidgetComp::GetFlags() const
{
	return WCF_NONE;
}


} // namespace iqtgui


