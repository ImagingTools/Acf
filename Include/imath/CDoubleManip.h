// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <imath/IDoubleManip.h>


namespace imath
{


/**
	Simple implementation of double value manipulator without rounding or quantization.
	
	\section DoubleManipPurpose Purpose
	CDoubleManip provides basic manipulation and formatting of double precision values
	without any rounding or quantization. It implements the IDoubleManip interface,
	offering string conversion, comparison, and formatting operations with configurable
	decimal point character. This class is the base for more specialized manipulators
	like CFixedPointManip.
	
	\section DoubleManipFeatures Features
	- **No Rounding**: Values pass through unchanged (GetRounded returns original value)
	- **Exact Comparison**: Uses standard == operator (no tolerance)
	- **String Formatting**: Converts to/from QString with configurable point character
	- **Precision Control**: Default precision of 6 decimal places for display
	- **Increment/Decrement**: Provides smallest step values for UI controls
	
	\section DoubleManipUsageExamples Usage Examples
	\code
	// Create manipulator with default settings
	imath::CDoubleManip manip;
	
	// String conversion with default point character '.'
	QString str = manip.GetString(3.14159);  // "3.14159" (6 decimals)
	
	// Parse string to double
	double value;
	if (manip.GetParsed("2.718", value)) {
		// value = 2.718
	}
	
	// Use European decimal comma
	manip.SetPointChar(',');
	QString str2 = manip.GetString(1.5);  // "1,5"
	manip.GetParsed("3,14", value);  // Parses "3,14" as 3.14
	
	// Comparison (exact, no tolerance)
	if (manip.AreValuesEqual(1.0, 1.0)) {
		// true - exact equality
	}
	
	if (manip.IsSmaller(1.0, 2.0)) {
		// true
	}
	
	// Get increment/decrement values for UI spinners
	double smaller = manip.GetSmallerValue(5.0);  // Returns previous representable value
	double bigger = manip.GetBiggerValue(5.0);    // Returns next representable value
	\endcode
	
	\section DoubleManipVsFixed CDoubleManip vs CFixedPointManip
	- **CDoubleManip**: No rounding, exact comparisons, full double precision
	- **CFixedPointManip**: Rounds to fixed decimal places, useful for financial or UI
	
	Use CDoubleManip when:
	- You need full floating-point precision
	- Working with scientific/engineering calculations
	- Rounding would introduce unacceptable errors
	- Values span many orders of magnitude
	
	Use CFixedPointManip when:
	- Fixed decimal precision is required (e.g., currency)
	- Consistent rounding behavior is needed
	- UI displays need specific decimal places
	
	\section DoubleManipInterfaces Purpose in Framework
	This class is typically used by:
	- Parameter systems for formatting/parsing numeric values
	- UI controls (spinboxes, sliders) for value manipulation
	- Serialization systems for text-based numeric representation
	
	\sa imath::IDoubleManip, imath::CFixedPointManip, imath::TIValueManip, imath::CDouble
	
	\ingroup NumericalTypes
*/
class CDoubleManip: virtual public IDoubleManip
{
public:
	CDoubleManip();

	/**
		Get point character.
	 */
	char GetPointChar() const;

	/**
		Set point character.
	 */
	void SetPointChar(char pointChar);

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
	enum{
		PRECISION = 6
	};

private:
	char m_pointChar;
};


// inline methods

inline char CDoubleManip::GetPointChar() const
{
	return m_pointChar;
}


inline void CDoubleManip::SetPointChar(char pointChar)
{
	m_pointChar = pointChar;
}


// reimplemented (imath::TIValueManip)

inline double CDoubleManip::GetRounded(const double& value) const
{
	return value;
}


inline bool CDoubleManip::AreValuesEqual(const double& value1, const double& value2) const
{
	return value1 == value2;
}


inline bool CDoubleManip::IsSmaller(const double& value1, const double& value2) const
{
	return value1 < value2;
}


inline bool CDoubleManip::IsSmallerEqual(const double& value1, const double& value2) const
{
	return value1 <= value2;
}


inline double CDoubleManip::GetSmallerValue(const double& value) const
{
	return value - 0.001;
}


inline double CDoubleManip::GetBiggerValue(const double& value) const
{
	return value + 0.001;
}


} // namespace imath


