#pragma once


// ACF includes
#include <imath/imath.h>


/**
	Contains color management classes.
	This package is system undependent.
	This package use following ACF packages:
	\li	istd
	\li	iser
	\li	imath

	\ingroup Color
*/
namespace icmm
{


/**
	Usage of a colorant.
*/
enum ColorantUsage
{
	/**
		No special usage of the colorant.
	*/
	CU_NONE,
	CU_CYAN,
	CU_DARK_CYAN,
	CU_LIGHT_CYAN,
	CU_MAGENTA,
	CU_DARK_MAGENTA,
	CU_LIGHT_MAGENTA,
	CU_YELLOW,
	CU_DARK_YELLOW,
	CU_LIGHT_YELLOW,
	CU_BLACK,
	CU_LIGHT_BLACK,
	CU_LIGHT_LIGHT_BLACK,

	/**
		Colorant is used for expanding the gamut of the printing device.
	*/
	CU_ECG,

	/**
		Colorant is used as a spot color.
	*/
	CU_SPOT
};

I_DECLARE_ENUM(
			ColorantUsage,
			CU_NONE,
			CU_CYAN,
			CU_DARK_CYAN,
			CU_LIGHT_CYAN,
			CU_MAGENTA,
			CU_DARK_MAGENTA,
			CU_LIGHT_MAGENTA,
			CU_YELLOW,
			CU_DARK_YELLOW,
			CU_LIGHT_YELLOW,
			CU_BLACK,
			CU_LIGHT_BLACK,
			CU_LIGHT_LIGHT_BLACK,
			CU_ECG,
			CU_SPOT);



} // namespace icmm


