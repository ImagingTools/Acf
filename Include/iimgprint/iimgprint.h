#pragma once


// ACF includes
#include <icmm/icmm.h>
#include <iimg/iimg.h>


/**
	Contains printer color transformation classes for converting images between printer color spaces.
	
	This package provides functionality for color conversion between different printer systems,
	enabling consistent printing results across different printers. It builds upon the icmm
	(color management) and iimg (image) libraries to provide printer-specific color transformations.
	
	This package uses the following ACF packages:
	\li	istd
	\li	iser
	\li	imath
	\li	icmm
	\li	iimg

	\ingroup Color
	\ingroup ImageProcessing
*/
namespace iimgprint
{


Q_NAMESPACE


/**
	Printer color space types for color transformations.
*/
enum class PrinterColorSpace
{
	/**
		RGB color space (display/input).
	*/
	RGB,
	
	/**
		CMYK color space (basic 4-color printing).
	*/
	CMYK,
	
	/**
		Extended gamut with additional spot colors.
	*/
	ExtendedGamut,
	
	/**
		Device-specific color space.
	*/
	DeviceSpecific
};
Q_ENUM_NS(PrinterColorSpace)


/**
	Rendering intent for color transformations between printer profiles.
*/
enum class RenderingIntent
{
	/**
		Perceptual rendering intent - maintains visual relationship between colors.
	*/
	Perceptual,
	
	/**
		Relative colorimetric - maps colors to nearest match in gamut.
	*/
	RelativeColorimetric,
	
	/**
		Saturation rendering - maximizes color saturation.
	*/
	Saturation,
	
	/**
		Absolute colorimetric - maintains absolute color values.
	*/
	AbsoluteColorimetric
};
Q_ENUM_NS(RenderingIntent)


} // namespace iimgprint


