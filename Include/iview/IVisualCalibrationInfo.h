// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <istd/IChangeable.h>


namespace iview
{


/**
	Common interface for all objects which allow access to calibration info.
*/
class IVisualCalibrationInfo: virtual public istd::IChangeable
{
public:
	/**
		Get minimal distance between grid lines.
	*/
	virtual double GetMinGridDistance() const = 0;
	
	/**
		Check if grid is visible.
	*/
	virtual bool IsGridVisible() const = 0;
	
	/**
		Check if grid should be shown in milimeter.
	*/
	virtual bool IsGridInMm() const = 0;
};


} // namespace iview




