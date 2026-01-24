// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <istd/TRange.h>


/**
	Interface for contraints of A.
*/
class IAConstraints: virtual public istd::IChangeable
{
public:
	/**
		Get the possible value range for A.
	*/
	virtual const istd::CRange& GetARange() const = 0;
};




