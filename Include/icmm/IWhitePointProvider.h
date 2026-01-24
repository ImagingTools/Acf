// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <icmm/CVarColor.h>


namespace icmm
{


/**
	Interface providing white point information for color spaces.
	
	\section WhitePointPurpose Purpose
	IWhitePointProvider defines an interface for objects that supply white point
	information. The white point represents the color that should appear as "white"
	in a color space, and is essential for chromatic adaptation and color space
	conversions. Different viewing conditions and illuminants produce different
	white points (D65, D50, A, etc.).

	\section WhitePointConcept White Point Concept
	A white point defines:
	- **Reference White**: The color considered as "neutral" or "white"
	- **Illuminant**: The light source characteristics (daylight, tungsten, etc.)
	- **Adaptation**: How colors appear under different lighting conditions
	- **XYZ Coordinates**: Typically expressed as XYZ tristimulus values

	\section WhitePointStandards Standard Illuminants
	Common standard white points:
	- **D65**: Daylight at 6500K - standard for sRGB, most displays
	- **D50**: Daylight at 5000K - standard for printing (ICC profiles)
	- **A**: Tungsten lamp at 2856K - incandescent lighting
	- **F2**: Cool white fluorescent - office lighting
	- **E**: Equal energy - theoretical reference

	\section WhitePointUsage Usage Examples
	\code
	// Example 1: Getting white point from provider
	void InspectWhitePoint(const icmm::IWhitePointProvider* provider)
	{
		icmm::CVarColor whitePoint = provider->GetWhitePoint();
		
		// White point typically in XYZ coordinates
		if (whitePoint.GetElementsCount() >= 3) {
			double X = whitePoint.GetElement(0);
			double Y = whitePoint.GetElement(1);
			double Z = whitePoint.GetElement(2);
			
			qDebug() << "White Point XYZ:" << X << Y << Z;
		}
	}

	// Example 2: Comparing white points
	bool SameWhitePoint(const icmm::IWhitePointProvider* provider1,
	                   const icmm::IWhitePointProvider* provider2,
	                   double tolerance = 0.001)
	{
		icmm::CVarColor wp1 = provider1->GetWhitePoint();
		icmm::CVarColor wp2 = provider2->GetWhitePoint();
		
		return wp1.IsSimilar(wp2, tolerance);
	}

	// Example 3: Chromatic adaptation
	// When converting between color spaces with different white points,
	// chromatic adaptation is needed
	icmm::CVarColor AdaptColor(const icmm::CVarColor& color,
	                          const icmm::IWhitePointProvider* sourceWP,
	                          const icmm::IWhitePointProvider* destWP)
	{
		icmm::CVarColor sourceWhite = sourceWP->GetWhitePoint();
		icmm::CVarColor destWhite = destWP->GetWhitePoint();
		
		// Apply chromatic adaptation transform (simplified)
		// Real implementation would use Bradford, von Kries, etc.
		// ...
		
		return color;  // Adapted color
	}

	// Example 4: Checking if adaptation is needed
	bool NeedsAdaptation(const icmm::IWhitePointProvider* source,
	                    const icmm::IWhitePointProvider* dest)
	{
		return !SameWhitePoint(source, dest);
	}
	\endcode

	\section WhitePointApplication Applications
	White points are used for:
	- **Color space conversion**: Ensuring colors appear consistent
	- **Chromatic adaptation**: Simulating how colors look under different lights
	- **Display calibration**: Matching screen white to desired standard
	- **Print preparation**: Converting from D65 (screen) to D50 (print)
	- **Color appearance modeling**: Advanced perceptual color models

	\sa icmm::IIlluminant, icmm::IColorSpecification, icmm::ITristimulusSpecification,
	    icmm::CXyzToCieLabTransformation

	\ingroup Color
*/
class IWhitePointProvider: virtual public istd::IChangeable
{
public:
	/**
		Gets the white point as a color value.
		
		Returns the white point typically as XYZ tristimulus values in a CVarColor.
		The white point defines what color should be considered as "white" or
		"neutral" in the color space.
		
		\return CVarColor containing the white point, typically XYZ coordinates
		
		\note White point is usually normalized so Y component = 1.0
		\note Common white points: D65 (X=0.95047, Y=1.0, Z=1.08883)
		
		\sa IIlluminant::GetWhitePoint()
	*/
	virtual icmm::CVarColor GetWhitePoint() const = 0;
};


} // namespace icmm


