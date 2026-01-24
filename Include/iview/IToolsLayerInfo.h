// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace iview
{


/**
	Interface for accessing informations about the tool layer in the shape view.
*/
class IToolsLayerInfo: virtual public istd::IChangeable
{
public:
	/**
		Check if the distance measuring is active.
	*/
	virtual bool IsDistanceMeasureToolActive() const = 0;

	/**
		Check if the point measuring is active.
	*/
	virtual bool IsPointMeasureToolActive() const = 0;
};


} // namespace iview



