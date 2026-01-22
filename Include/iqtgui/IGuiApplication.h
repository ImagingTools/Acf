// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


#include <ibase/IApplication.h>

#include <iqtgui/IGuiObject.h>


namespace iqtgui
{		


/**
	Interface for Qt based application with GUI.
*/
class IGuiApplication: virtual public ibase::IApplication
{
public:
	/**
		Get the main UI object of the application.
	*/
	virtual const iqtgui::IGuiObject* GetApplicationGui() const = 0;
};


} // namespace iqtgui




