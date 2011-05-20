#ifndef iqtgui_IDialog_included
#define iqtgui_IDialog_included


// ACF includes
#include "istd/IPolymorphic.h"

#include "iqtgui/IGuiObject.h"


namespace iqtgui
{


/**
	Interface for a dialog widget.
*/
class IDialog: virtual public istd::IPolymorphic
{
public:
	/**
		Execute dialog instance.
	*/
	virtual void ExecuteDialog(IGuiObject* parentPtr) = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IDialog_included


