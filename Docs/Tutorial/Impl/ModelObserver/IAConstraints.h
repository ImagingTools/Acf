// SPDX-License-Identifier: LGPL-2.1-only
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




