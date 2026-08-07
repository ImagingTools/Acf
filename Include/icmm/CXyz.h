// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iser/IArchive.h>
#include <icmm/TComposedColor.h>


namespace icmm
{


/**
	Primitive for representation of CIE XYZ tristimulus values.

	XYZ is the device independent base of all tristimulus color spaces. Values are relative to an
	illuminant and observer, the Y component is typically normalized to 1.0 for the white point.

	\sa icmm::CLab, icmm::CXyzToCieLabTransformation, icmm::ITristimulusSpecification

	\ingroup Color
*/
class CXyz: public TComposedColor<3>
{
public:
	typedef TComposedColor<3> BaseClass;

	enum ComponentIndex
	{
		CI_X,
		CI_Y,
		CI_Z
	};

	CXyz(double x = 0.0, double y = 0.0, double z = 0.0);
	CXyz(const CXyz& color);

	// access to component values
	double GetX() const;
	void SetX(double value);
	double GetY() const;
	void SetY(double value);
	double GetZ() const;
	void SetZ(double value);

	virtual bool Serialize(iser::IArchive& archive) override;

	// operators
	CXyz operator+(const CXyz& color) const;
	CXyz operator-(const CXyz& color) const;
	CXyz operator*(const CXyz& color) const;
	CXyz operator/(const CXyz& color) const;

	CXyz operator*(double value) const;
	CXyz operator/(double value) const;

	CXyz& operator=(const CXyz& color);

	const CXyz& operator+=(const CXyz& color);
	const CXyz& operator-=(const CXyz& color);
	const CXyz& operator*=(const CXyz& color);
	const CXyz& operator/=(const CXyz& color);

	const CXyz& operator*=(double value);
	const CXyz& operator/=(double value);
};


// inline methods

inline CXyz::CXyz(double x, double y, double z)
{
	SetElement(CI_X, x);
	SetElement(CI_Y, y);
	SetElement(CI_Z, z);
}


inline CXyz::CXyz(const CXyz& color)
:	BaseClass(color)
{
}


// access to components

inline double CXyz::GetX() const
{
	return GetElement(CI_X);
}


inline void CXyz::SetX(double value)
{
	SetElement(CI_X, value);
}


inline double CXyz::GetY() const
{
	return GetElement(CI_Y);
}


inline void CXyz::SetY(double value)
{
	SetElement(CI_Y, value);
}


inline double CXyz::GetZ() const
{
	return GetElement(CI_Z);
}


inline void CXyz::SetZ(double value)
{
	SetElement(CI_Z, value);
}


// operators

inline CXyz CXyz::operator+(const CXyz& color) const
{
	CXyz retVal = *this;

	retVal += color;

	return retVal;
}


inline CXyz CXyz::operator-(const CXyz& color) const
{
	CXyz retVal = *this;

	retVal -= color;

	return retVal;
}


inline CXyz CXyz::operator*(const CXyz& color) const
{
	CXyz retVal = *this;

	retVal *= color;

	return retVal;
}


inline CXyz CXyz::operator/(const CXyz& color) const
{
	CXyz retVal = *this;

	retVal /= color;

	return retVal;
}


inline CXyz CXyz::operator*(double value) const
{
	CXyz retVal = *this;

	retVal *= value;

	return retVal;
}


inline CXyz CXyz::operator/(double value) const
{
	CXyz retVal = *this;

	retVal /= value;

	return retVal;
}


inline CXyz& CXyz::operator=(const CXyz& color)
{
	BaseClass::operator=(color);

	return *this;
}


inline const CXyz& CXyz::operator+=(const CXyz& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CXyz& CXyz::operator-=(const CXyz& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CXyz& CXyz::operator*=(const CXyz& color)
{
	BaseClass::operator*=(color);

	return *this;
}


inline const CXyz& CXyz::operator/=(const CXyz& color)
{
	BaseClass::operator/=(color);

	return *this;
}


inline const CXyz& CXyz::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CXyz& CXyz::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm

