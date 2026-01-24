// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <istd/IPolymorphic.h>

#include <iview/IRuler.h>


namespace iview
{


/**
	Common interface for all view rulers provider.
	This interface allows you to get an access to left and top view ruler.
*/
class IViewRulersAccessor: virtual public istd::IPolymorphic
{
public:
	/**
		Get an access to left view ruler.
		\return	pointer to left ruler object, or NULL, when it is not accessible.
	*/
	virtual IRuler* GetLeftRulerPtr() = 0;
	
	/**
		Get an access to top view ruler.
		\return	pointer to top ruler object, or NULL, when it is not accessible.
	*/
	virtual IRuler* GetTopRulerPtr() = 0;
};


} // namespace iview




