// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <imath/TISampledFunction.h>


namespace imath
{


/**
	Interface for a 1D-interpolator based on a sampled function.
*/
class ISampledFunctionInterpolator: virtual public IDoubleFunction
{
public:
	/**
		Initialize interpolator based on a sample function.
	*/
	virtual bool InitFromFunction(const ISampledFunction& function) = 0;
};


} // namespace imath


