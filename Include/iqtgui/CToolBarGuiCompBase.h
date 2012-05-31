#ifndef iqtgui_CToolBarGuiCompBase_included
#define iqtgui_CToolBarGuiCompBase_included


// Qt includes
#include <QtCore/QMap>
#include <QtGui/QToolBar>

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
};


} // namespace iqtgui


#endif // !iqtgui_CToolBarGuiCompBase_included


