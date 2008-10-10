#ifndef iqtgui_IToolBarManager_included
#define iqtgui_IToolBarManager_included


#include "iqtgui/iqtgui.h"


#include "istd/IPolymorphic.h"


class QWidget;
class QToolBar;


namespace iqtgui
{


/**
	Common interface for a toolbar manager.
*/
class IToolBarManager: virtual public istd::IPolymorphic
{
public:
	/**
		Set visibility of all tool bars to \s isVisible
	*/
	virtual void SetToolBarsVisible(bool isVisible = true) = 0;

	/**
		Get number of toolbars.
	*/
	virtual int GetToolBarCount() const = 0;

	/**
		Add a toolbar.
	*/
	virtual	void AddToolBar(int flags, QToolBar* widgetPtr) = 0;

	/**
		Remove the given toolbar.
	*/
	virtual void RemoveToolBar(QToolBar* widgetPtr) = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IToolBarManager_included

