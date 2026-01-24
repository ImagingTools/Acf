// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION < 0x050000
#include <limits>
#define qInf std::numeric_limits<double>::infinity
inline bool qIsNaN(double x){ return x != x; }
#endif

// ACF includes
#include <iser/iser.h>

/**
	Mathematical constant value PI.
*/
static const double I_PI = 3.14159265358979323846;

/**
	Mathematical constant value 2 * PI.
*/
static const double I_2PI = 6.2831853071795864769;


/**
	Package with mathematical functions and algebraical primitives.
	This package is system independent.
	This package use following ACF packages:
	\li istd
	\li iser

	\ingroup NumericalBase
*/
namespace imath
{


inline double GetRadianFromDegree(double degree)
{
	return degree / 180.0 * I_PI;
}


inline double GetDegreeFromRadian(double radian)
{
	return radian * 180.0 / I_PI;
}


} // namespace imath


