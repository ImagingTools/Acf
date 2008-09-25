#ifndef iqtgui_IDockManager_included
#define iqtgui_IDockManager_included


#include "iqtgui/iqtgui.h"


#include "istd/IPolymorphic.h"


class QDockWidget;


namespace iqtgui
{


/**
	A common interface for docking window manager.
*/
class IDockManager: virtual public istd::IPolymorphic
{
public:
	virtual	void AddDockWidget(int flags, QDockWidget* widget) = 0;
	virtual void RemoveDockWidget(QDockWidget* widget) = 0;
};


} // namespace iqtgui


#endif // iqtgui_IDockManager_included

