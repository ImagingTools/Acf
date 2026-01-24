// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <i2d/IObject2d.h>


namespace i2d
{


/**
	Common interface for a provider, which delivers a 2D-object.
*/
class IObject2dProvider: virtual public istd::IChangeable
{
public:
	/**
		Get access to 2D-object.
	*/
	virtual const IObject2d* GetObject2d() const = 0;
};


} // namespace i2d




