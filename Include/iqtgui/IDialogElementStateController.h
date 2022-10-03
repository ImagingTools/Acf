#ifndef iqtgui_IDialogStateController_included
#define iqtgui_IDialogStateController_included


// Qt includes
#include <QtWidgets/QDialogButtonBox>

// ACF includes
#include <istd/IPolymorphic.h>


namespace iqtgui
{


/**
	Interface for managing the state of dialog elements.
*/
class IDialogElementStateController: virtual public istd::IPolymorphic
{
public:
	/**
		Get available dialog standard buttons
	*/
	virtual int GetAvailableButtons() const = 0;

	virtual bool IsButtonEnabled(QDialogButtonBox::StandardButton button) const = 0;
	virtual void EnableButton(QDialogButtonBox::StandardButton button, bool enable) = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IDialogStateController_included


