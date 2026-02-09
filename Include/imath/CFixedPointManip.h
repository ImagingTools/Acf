// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <imath/CDoubleManip.h>


namespace imath
{


/**
	Implementation of double value manipulator with fixed-point arithmetic and rounding.
	
	\section FixedPointPurpose Purpose
	CFixedPointManip provides manipulation of double precision values with fixed decimal
	precision, simulating fixed-point arithmetic. It rounds values to a specified number
	of decimal places and ensures consistent behavior for financial calculations, user
	interfaces, and scenarios where exact decimal representation is required. Extends
	CDoubleManip with configurable precision and rounding modes.
	
	\section FixedPointFeatures Key Features
	- **Fixed Precision**: Rounds to specified decimal places (1 = 0.1, 2 = 0.01, etc.)
	- **Rounding Modes**: Normal (nearest), floor (down), ceil (up)
	- **Consistent Arithmetic**: All operations respect the fixed precision
	- **String Formatting**: Shows exactly the configured precision
	- **Comparison**: Compares rounded values (tolerance implicit in precision)
	- **Increment/Decrement**: Steps by the minimum precision unit
	
	\section FixedPointRoundingModes Rounding Modes
	
	**RT_NORMAL** - Mathematical rounding (nearest value, 0.5 rounds up):
	- 1.234 → 1.23 (precision=2)
	- 1.235 → 1.24 (precision=2)
	
	**RT_FLOOR** - Always round toward negative infinity:
	- 1.239 → 1.23 (precision=2)
	- -1.231 → -1.24 (precision=2)
	
	**RT_CEIL** - Always round toward positive infinity:
	- 1.231 → 1.24 (precision=2)
	- -1.239 → -1.23 (precision=2)
	
	\section FixedPointUsageExamples Usage Examples
	\code
	// Create manipulator for currency (2 decimal places)
	imath::CFixedPointManip moneyManip(2, imath::CFixedPointManip::RT_NORMAL);
	
	// Rounding
	double rounded = moneyManip.GetRounded(3.14159);  // 3.14
	
	// String formatting (shows exact precision)
	QString str = moneyManip.GetString(5.1);  // "5.10" (always 2 decimals)
	
	// Comparison (rounds before comparing)
	bool equal = moneyManip.AreValuesEqual(1.234, 1.235);  // true (both → 1.24)
	bool less = moneyManip.IsSmaller(1.231, 1.235);  // true (1.23 < 1.24)
	
	// Increment/decrement by precision
	double bigger = moneyManip.GetBiggerValue(5.10);  // 5.11 (step = 0.01)
	double smaller = moneyManip.GetSmallerValue(5.10);  // 5.09
	
	// Parse with rounding
	double value;
	moneyManip.GetParsed("3.14159", value);  // value = 3.14 (rounded)
	
	// Different precisions
	imath::CFixedPointManip manip0(0);  // Integer (precision = 1)
	imath::CFixedPointManip manip1(1);  // One decimal (precision = 0.1)
	imath::CFixedPointManip manip3(3);  // Three decimals (precision = 0.001)
	
	manip0.GetRounded(3.7);  // 4.0
	manip1.GetRounded(3.74);  // 3.7
	manip3.GetRounded(3.1416);  // 3.142
	
	// Rounding mode comparison
	imath::CFixedPointManip normal(2, imath::CFixedPointManip::RT_NORMAL);
	imath::CFixedPointManip floor(2, imath::CFixedPointManip::RT_FLOOR);
	imath::CFixedPointManip ceil(2, imath::CFixedPointManip::RT_CEIL);
	
	normal.GetRounded(1.235);  // 1.24
	floor.GetRounded(1.235);   // 1.23
	ceil.GetRounded(1.235);    // 1.24
	
	normal.GetRounded(-1.235); // -1.24
	floor.GetRounded(-1.235);  // -1.24 (toward -∞)
	ceil.GetRounded(-1.235);   // -1.23 (toward +∞)
	\endcode
	
	\section FixedPointUseCases Common Use Cases
	
	**Financial/Currency Applications**:
	- Always use precision=2 (cents/pence)
	- Prevents floating-point rounding errors
	- Ensures consistent display ($1.50, not $1.5)
	
	**User Interface Controls**:
	- Spinboxes with fixed step size
	- Sliders with discrete positions
	- Display fields with consistent formatting
	
	**Fixed-Resolution Measurements**:
	- Instruments with limited resolution (0.1mm, 0.01V)
	- Quantized values from hardware
	
	**Scientific Data with Fixed Precision**:
	- Laboratory measurements with known precision
	- Sensor data with fixed decimal resolution
	
	\section FixedPointVsDouble CFixedPointManip vs CDoubleManip
	- **CFixedPointManip**: Rounds to fixed precision, predictable for UI/financial
	- **CDoubleManip**: Full precision, no rounding, for scientific calculations
	
	\section FixedPointPrecisionNote Precision Parameter
	The precision parameter represents the number of decimal places:
	- precision=0: Integer values (1, 2, 3, ...)
	- precision=1: One decimal place (1.0, 1.1, 1.2, ...)
	- precision=2: Two decimal places (1.00, 1.01, 1.02, ...)
	- precision=n: n decimal places
	
	\sa imath::CDoubleManip, imath::IDoubleManip, imath::CDouble, imath::TIValueManip
	
	\ingroup NumericalTypes
*/
class CFixedPointManip: public CDoubleManip
{
public:
	typedef CDoubleManip BaseClass;

	enum RoundingType
	{
		/**
			Normal mathematical rounding to the nearest value.
		*/
		RT_NORMAL,
		/**
			Rounding down.
		*/
		RT_FLOOR,
		/**
			Rounding up.
		*/
		RT_CEIL,
		RT_LAST = RT_CEIL + 1
	};

	CFixedPointManip(int precision = 1, RoundingType roundingType = RT_NORMAL);

	/**
		Set fixed point precision.
	 */
	void SetPrecision(int precision);

	/**	Get type of rounding.
	 */
	RoundingType GetRoundingType() const;

	/**	Set type of rounding.
	 */
	void SetRoundingType(RoundingType roundingType);

	// reimplemented (imath::IDoubleManip)
	virtual int GetPrecision() const override;

	// reimplemented (imath::TIValueManip)
	virtual double GetRounded(const double& value) const override;
	virtual bool AreValuesEqual(const double& value1, const double& value2) const override;
	virtual bool IsSmaller(const double& value1, const double& value2) const override;
	virtual bool IsSmallerEqual(const double& value1, const double& value2) const override;
	virtual double GetSmallerValue(const double& value) const override;
	virtual double GetBiggerValue(const double& value) const override;
	virtual QString GetString(const double& value) const override;
	virtual bool GetParsed(const QString& text, double& result) const override;

protected:
	typedef int (*RoundingFuntionPtr)(qreal value);

	virtual qint32 GetInternalValue(double value) const;
	virtual double GetNormalValue(qint32 intValue) const;

	QString GetString(const double& value, int precision) const;

private:
	int m_precision;
	RoundingType m_roundingType;

	static RoundingFuntionPtr m_roundingFuntionsPtr[RT_LAST + 1];

	double m_scaleToIntFactor;
};


// inline methods

inline CFixedPointManip::RoundingType CFixedPointManip::GetRoundingType() const
{
	return m_roundingType;
}


// reimplemented (imath::TIValueManip)

inline double CFixedPointManip::GetRounded(const double& value) const
{
	return GetNormalValue(GetInternalValue(value));
}


inline bool CFixedPointManip::AreValuesEqual(const double& value1, const double& value2) const
{
	return GetInternalValue(value1) == GetInternalValue(value2);
}


inline bool CFixedPointManip::IsSmaller(const double& value1, const double& value2) const
{
	return GetInternalValue(value1) < GetInternalValue(value2);
}


inline bool CFixedPointManip::IsSmallerEqual(const double& value1, const double& value2) const
{
	return GetInternalValue(value1) <= GetInternalValue(value2);
}


inline double CFixedPointManip::GetSmallerValue(const double& value) const
{
	return GetNormalValue(GetInternalValue(value) - 1);
}


inline double CFixedPointManip::GetBiggerValue(const double& value) const
{
	return GetNormalValue(GetInternalValue(value) + 1);
}


// protected methods

inline qint32 CFixedPointManip::GetInternalValue(double value) const
{
	RoundingFuntionPtr function = m_roundingFuntionsPtr[m_roundingType];

	return qint32(function(value * m_scaleToIntFactor));
}


inline double CFixedPointManip::GetNormalValue(qint32 intValue) const
{
	return intValue / m_scaleToIntFactor;
}


} // namespace imath


