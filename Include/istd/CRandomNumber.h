// SPDX-License-Identifier: LGPL-2.1-only
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




