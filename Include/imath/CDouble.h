// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QtMath>

// ACF includes
#include <iser/IArchive.h>
#include <imath/TVector.h>


namespace imath
{


/**
	Wrapper class for double precision floating-point values with rounding utilities.
	
	\section DoublePurpose Purpose
	CDouble provides a convenient wrapper around double precision floating-point values,
	offering arithmetic operators and specialized rounding/comparison methods. Built on
	top of TVector<1>, it combines the benefits of vector operations with double-specific
	functionality for numerical computations requiring controlled precision.
	
	\section DoubleFeatures Key Features
	- **Arithmetic Operations**: Full set of operators (+, -, *, /, +=, -=, *=, /=)
	- **Comparison**: Standard comparison operators (==, !=, <, >, <=, >=)
	- **Rounding**: Multiple rounding modes (round, floor, ceil) with precision control
	- **Tolerance Comparison**: IsSimiliar() for floating-point equality within tolerance
	- **Type Conversion**: Implicit conversion to/from double
	- **Static Methods**: Utility functions for working with raw double values
	
	\section DoubleUsageExamples Usage Examples
	\code
	// Basic construction and arithmetic
	imath::CDouble value1(3.14159);
	imath::CDouble value2(2.71828);
	imath::CDouble sum = value1 + value2;  // 5.85987
	
	// Rounding to specified precision
	imath::CDouble pi(3.14159265359);
	imath::CDouble rounded = pi.GetRounded(2);  // 3.14
	imath::CDouble roundedDown = pi.GetRoundedDown(3);  // 3.141
	imath::CDouble roundedUp = pi.GetRoundedUp(3);  // 3.142
	
	// Comparison with rounding
	imath::CDouble val1(3.14159);
	imath::CDouble val2(3.14151);
	if (val1.IsRoundedEqual(val2, 2)) {
		// true - both round to 3.14
	}
	
	// Tolerance-based comparison (for floating-point equality)
	imath::CDouble a(1.0000001);
	imath::CDouble b(1.0000002);
	if (a.IsSimiliar(b, 1e-6)) {
		// true - difference is within tolerance
	}
	
	// Using static methods
	double d1 = 2.718281828;
	double rounded = imath::CDouble::GetRounded(d1, 4);  // 2.7183
	bool similar = imath::CDouble::IsSimiliar(2.0, 2.0000001, 1e-6);  // true
	
	// Implicit conversion
	imath::CDouble value(5.5);
	double d = value;  // Implicit conversion to double
	value = 7.2;  // Implicit conversion from double
	\endcode
	
	\section DoubleWhenToUse When to Use
	Use CDouble when:
	- You need controlled rounding and precision management
	- Working with numerical algorithms requiring tolerance comparisons
	- Interfacing with code that uses TVector but need scalar operations
	- You want type safety and explicit semantics for double values
	
	Use raw double when:
	- Simple scalar calculations without special rounding needs
	- Performance is absolutely critical (minimal overhead)
	- Interfacing with C APIs or external libraries expecting double*
	
	\section DoublePrecision Precision Notes
	- **precision parameter**: Number of decimal places (e.g., precision=2 means 0.01 resolution)
	- **GetRounded()**: Rounds to nearest (0.5 rounds up)
	- **GetRoundedDown()**: Always rounds toward negative infinity (floor)
	- **GetRoundedUp()**: Always rounds toward positive infinity (ceil)
	- **IsSimiliar()**: Default tolerance is I_BIG_EPSILON (typically 1e-6)
	
	\sa imath::TVector, imath::CDoubleManip, imath::CFixedPointManip
	
	\ingroup NumericalTypes
*/
class CDouble: public TVector<1>
{
public:
	typedef TVector<1> BaseClass;

	CDouble(double value = 0.0);
	CDouble(const BaseClass& value);

	/**
		Get rounded value.
	*/
	CDouble GetRounded(int precision = 2) const;

	/**
		Check if two values are equal after rounding.
	*/
	bool IsRoundedEqual(const CDouble& value, int precision = 2) const;

	/**
		Get rounded down value with specified precision.
	*/
	CDouble GetRoundedDown(int precision = 2) const;

	/**
		Check if two values are equal after rounding down.
	*/
	bool IsRoundedDownEqual(const CDouble& value, int precision = 2) const;

	/**
		Get rounded down value with specified precision.
	*/
	CDouble GetRoundedUp(int precision = 2) const;

	/**
		Check if two values are equal after rounding down.
	*/
	bool IsRoundedUpEqual(const CDouble& value, int precision = 2) const;

	/**
		Check if two values are similiar with specified tolerance.
	*/
	bool IsSimiliar(const CDouble& value, double tolerance = I_BIG_EPSILON) const;

	// operators
	operator double() const;

	bool operator==(const CDouble& value) const;
	bool operator!=(const CDouble& value) const;
	bool operator<(const CDouble& value) const;
	bool operator>(const CDouble& value) const;
	bool operator<=(const CDouble& value) const;
	bool operator>=(const CDouble& value) const;

	CDouble operator+(const CDouble& value) const;
	CDouble operator-(const CDouble& value) const;
	CDouble operator*(const CDouble& value) const;
	CDouble operator/(const CDouble& value) const;

	const CDouble& operator=(const CDouble& value);

	const CDouble& operator+=(const CDouble& value);
	const CDouble& operator-=(const CDouble& value);
	const CDouble& operator*=(const CDouble& value);
	const CDouble& operator/=(const CDouble& value);

	// static methods
	static double GetRounded(double value, int precision = 2);
	static bool IsRoundedEqual(double value1, double value2, int precision = 2);
	static double GetRoundedDown(double value, int precision = 2);
	static bool IsRoundedDownEqual(double value1, double value2, int precision = 2);
	static double GetRoundedUp(double value, int precision = 2);
	static bool IsRoundedUpEqual(double value1, double value2, int precision = 2);
	static bool IsSimiliar(double value1, double value2, double tolerance = I_BIG_EPSILON);
};


// inline methods
	
inline CDouble::CDouble(double value)
{
	SetElement(0, value);
}


inline CDouble::CDouble(const TVector<1>& value)
:	BaseClass(value)
{
}


inline CDouble::operator double() const
{
	return GetElement(0);
}


inline CDouble CDouble::GetRounded(int precision) const
{
	return GetRounded(GetElement(0), precision);
}


inline bool CDouble::IsRoundedEqual(const CDouble& value, int precision) const
{
	return IsRoundedEqual(*this, value, precision);
}


inline CDouble CDouble::GetRoundedDown(int precision) const
{
	return GetRoundedDown(*this, precision);
}


inline bool CDouble::IsRoundedDownEqual(const CDouble& value, int precision) const
{
	return IsRoundedDownEqual(*this, value, precision);
}


inline CDouble CDouble::GetRoundedUp(int precision) const
{
	return GetRoundedUp(*this, precision);
}


inline bool CDouble::IsRoundedUpEqual(const CDouble& value, int precision) const
{
	return IsRoundedUpEqual(*this, value, precision);
}


inline bool CDouble::IsSimiliar(const CDouble& value, double tolerance) const
{
	return IsSimiliar(*this, value, tolerance);
}


inline bool CDouble::operator==(const CDouble& value) const
{
	return GetElement(0) == value.GetElement(0);
}


inline bool CDouble::operator!=(const CDouble& value) const
{
	return GetElement(0) != value.GetElement(0);
}


inline bool CDouble::operator<(const CDouble& value) const
{
	return GetElement(0) < value.GetElement(0);
}


inline bool CDouble::operator>(const CDouble& value) const
{
	return GetElement(0) > value.GetElement(0);
}


inline bool CDouble::operator<=(const CDouble& value) const
{
	return GetElement(0) <= value.GetElement(0);
}


inline bool CDouble::operator>=(const CDouble& value) const
{
	return GetElement(0) >= value.GetElement(0);
}


inline CDouble CDouble::operator+(const CDouble& value) const
{
	return GetElement(0) + value.GetElement(0);
}


inline CDouble CDouble::operator-(const CDouble& value) const
{
	return GetElement(0) - value.GetElement(0);
}


inline CDouble CDouble::operator*(const CDouble& value) const
{
	return GetElement(0) * value.GetElement(0);
}


inline CDouble CDouble::operator/(const CDouble& value) const
{
	return GetElement(0) / value.GetElement(0);
}


inline const CDouble& CDouble::operator=(const CDouble& value)
{
	operator[](0) = value.GetElement(0);

	return *this;
}


inline const CDouble& CDouble::operator+=(const CDouble& value)
{
	operator[](0) += value.GetElement(0);

	return *this;
}


inline const CDouble& CDouble::operator-=(const CDouble& value)
{
	operator[](0) -= value.GetElement(0);

	return *this;
}


inline const CDouble& CDouble::operator*=(const CDouble& value)
{
	operator[](0) *= value.GetElement(0);

	return *this;
}


inline const CDouble& CDouble::operator/=(const CDouble& value)
{
	operator[](0) /= value.GetElement(0);

	return *this;
}


// static methods

inline double CDouble::GetRounded(double value, int precision)
{
	double scale = qPow(10.0, precision);

	return qFloor(value * scale + 0.5) / scale;
}


inline bool CDouble::IsRoundedEqual(double value1, double value2, int precision)
{
	double scale = qPow(10.0, precision);

	return qFloor(value1 * scale + 0.5) == qFloor(value2 * scale + 0.5);
}


inline double CDouble::GetRoundedDown(double value, int precision)
{
	double scale = qPow(10.0, precision);

	return qFloor(value * scale) / scale;
}


inline bool CDouble::IsRoundedDownEqual(double value1, double value2, int precision)
{
	double scale = qPow(10.0, precision);

	return qFloor(value1 * scale) == qFloor(value2 * scale);
}


inline double CDouble::GetRoundedUp(double value, int precision)
{
	double scale = qPow(10.0, precision);

	return qCeil(value * scale) / scale;
}


inline bool CDouble::IsRoundedUpEqual(double value1, double value2, int precision)
{
	double scale = qPow(10.0, precision);

	return qCeil(value1 * scale) == qCeil(value2 * scale);
}


inline bool CDouble::IsSimiliar(double value1, double value2, double tolerance)
{
	return qAbs(value1 - value2) <= tolerance;
}


} // namespace imath


