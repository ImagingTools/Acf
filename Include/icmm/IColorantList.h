// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <icmm/icmm.h>


namespace icmm
{


/**
	Common interface for a colorant list used in a substractive color model.
*/
class IColorantList: virtual public istd::IChangeable
{
public:
	typedef QString ColorantId;
	typedef QStringList ColorantIds;

	/**
		Get the list of colorant-IDs.
	*/
	virtual ColorantIds GetColorantIds() const = 0;

	/**
		Get type of the colorant with the given ID. 
	*/
	virtual ColorantUsage GetColorantUsage(const ColorantId& colorantId) const = 0;
};


} // namespace icmm


