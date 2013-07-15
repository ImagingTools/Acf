#ifndef iqtgui_CToolBarGuiCompBase_included
#define iqtgui_CToolBarGuiCompBase_included


// Qt includes
#include <QtCore/QMap>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QToolBar>
#else
#include <QtGui/QToolBar>
#endif

// ACF includes
#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


/**
	Basic implementation of a tool bar component.
*/
class CToolBarGuiCompBase:
			public iqtgui::TGuiComponentBase<QToolBar>,
			virtual public iqtgui::IMainWindowComponent
{
public:
	typedef iqtgui::TGuiComponentBase<QToolBar> BaseClass;

	I_BEGIN_COMPONENT(CToolBarGuiCompBase);
		I_REGISTER_INTERFACE(iqtgui::IMainWindowComponent);
		I_ASSIGN(m_dockAreaAttrPtr, "ToolBarArea", "Specify the area for this toolbar\n 0 - left\n 1 - right\n 2 - top\n 3 - bottom", false, 0);
		I_ASSIGN(m_titleAttrPtr, "Title", "Tool bar title", false, "");
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of the icon used in the tool bar", false, 32);
	I_END_COMPONENT;

	// reimplemented (iqtgui::IMainWindowComponent)
	virtual bool AddToMainWindow(QMainWindow& mainWindow);
	virtual bool RemoveFromMainWindow(QMainWindow& mainWindow);

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_ATTR(int, m_dockAreaAttrPtr);
	I_ATTR(QString, m_titleAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CToolBarGuiCompBase_included


