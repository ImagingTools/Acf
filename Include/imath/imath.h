#ifndef imath_included
#define imath_included


#include "iser/iser.h"


/**
	\ingroup Main

	Mathematical constant value PI.
*/
static const double I_PI = 3.1415926;


/**
	\ingroup Main

	Package with mathematical functions and algebraical primitives.
	This package is system independent.
	This package use following ACF packages:
	\li istd
	\li iser
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


#endif // !imath_included

