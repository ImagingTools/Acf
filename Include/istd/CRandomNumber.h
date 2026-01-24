// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


namespace istd
{


/**
	Helper class for calculation of a random number.
*/
class CRandomNumber
{
public:
	CRandomNumber();

	/**
		Get new random integer value in the given range.
	*/
	int GetNextInt(int minValue, int maxValue) const;

	/**
		Get new random real value in the given range.
	*/
	double GetNextDouble(double minValue, double maxValue) const;

protected:
	double GetNextUnitary() const;
};


} // namespace istd




