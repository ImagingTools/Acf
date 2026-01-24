// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <i2d/ICalibration2d.h>
#include <iprm/IOptionsList.h>


namespace i2d
{


/**
	Common interface for an object, which delivers a list of 2D-calibration objects.
*/
class IMultiCalibrationProvider: virtual public istd::IChangeable
{
public:
	/**
		Get optional information about calibration objects in form of section contraints.
		It allows to get name and description of each calibration object.
	*/
	virtual const iprm::IOptionsList* GetCalibrationSelectionContraints() const = 0;

	/**
		Get number of available calibrations.
	*/
	virtual int GetCalibrationsCount() const = 0;

	/**
		Get access to transformation object with a given index, which transforms a local logical coordinate system to global one.
		\return		Pointer to transformation if it is accessible, or NULL.
	*/
	virtual const ICalibration2d* GetCalibration(int calibrationIndex) const = 0;
};


} // namespace i2d




