// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <istd/IPolymorphic.h>
#include <iqtgui/IGuiObject.h>


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
		\return Status of the dialog execution. \sa QDialog
	*/
	virtual int ExecuteDialog(IGuiObject* parentPtr) = 0;
};


} // namespace iqtgui




