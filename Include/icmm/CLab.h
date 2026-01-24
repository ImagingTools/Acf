// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iser/IArchive.h>
#include <icmm/TComposedColor.h>


namespace icmm
{


/**
	Primitive for representation of CIE Lab color values.
	
	\section LabPurpose Purpose
	CLab represents colors in the CIE L*a*b* color space, a device-independent,
	perceptually uniform color model designed to approximate human vision. Unlike
	RGB or CMYK, Lab is designed so that the numerical difference between two
	colors corresponds to the perceptual difference seen by humans. This makes it
	ideal for color comparison, color correction, and gamut mapping.

	\section LabColorSpace CIE Lab Color Space
	Lab consists of three coordinates:
	- **L*** (Lightness): 0 (black) to 100 (white)
	- **a***: Green (-128) to Red (+127)
	- **b***: Blue (-128) to Yellow (+127)

	The a* and b* axes represent opponent color dimensions:
	- **a* < 0**: Green tones
	- **a* > 0**: Red/magenta tones
	- **b* < 0**: Blue tones
	- **b* > 0**: Yellow tones
	- **a* = b* = 0**: Neutral gray

	\section LabAdvantages Lab Advantages
	**Perceptual Uniformity:**
	- Equal distances in Lab space ≈ equal perceived color differences
	- Makes it ideal for ΔE (Delta E) color difference calculations
	
	**Device Independence:**
	- Not tied to any specific device (monitor, printer)
	- Provides absolute color definition
	
	**Gamut Mapping:**
	- Easy to determine if colors are out of gamut
	- Natural for color space conversions

	\section LabUsage Usage Examples
	\code
	// Example 1: Creating Lab colors
	icmm::CLab white(100.0, 0.0, 0.0);        // L=100, neutral
	icmm::CLab black(0.0, 0.0, 0.0);          // L=0, neutral
	icmm::CLab midGray(50.0, 0.0, 0.0);       // L=50, neutral
	
	icmm::CLab red(53.0, 80.0, 67.0);         // Typical red
	icmm::CLab green(88.0, -86.0, 83.0);      // Typical green
	icmm::CLab blue(32.0, 79.0, -108.0);      // Typical blue

	// Example 2: Component access
	icmm::CLab color;
	color.SetL(75.0);    // Lightness
	color.SetA(25.0);    // Red direction
	color.SetB(-30.0);   // Blue direction
	
	double lightness = color.GetL();
	double a = color.GetA();
	double b = color.GetB();

	// Example 3: Calculating color difference (ΔE)
	double CompareColors(const icmm::CLab& color1, const icmm::CLab& color2)
	{
		// ΔE76 formula (CIE 1976)
		double deltaE = color1.GetDeltaE(color2);
		
		// Interpretation of ΔE:
		// < 1.0: Imperceptible difference
		// 1-2: Perceptible by close observation
		// 2-10: Perceptible at a glance
		// 11-49: Colors are more similar than opposite
		// 100: Colors are exact opposites
		
		return deltaE;
	}
	
	icmm::CLab lab1(50.0, 10.0, 20.0);
	icmm::CLab lab2(52.0, 12.0, 18.0);
	double difference = CompareColors(lab1, lab2);  // Small difference

	// Example 4: Finding closest color in palette
	icmm::CLab FindClosestColor(const icmm::CLab& target,
	                            const QList<icmm::CLab>& palette)
	{
		if (palette.isEmpty()) return icmm::CLab();
		
		icmm::CLab closest = palette[0];
		double minDeltaE = target.GetDeltaE(closest);
		
		for (const icmm::CLab& color : palette) {
			double deltaE = target.GetDeltaE(color);
			if (deltaE < minDeltaE) {
				minDeltaE = deltaE;
				closest = color;
			}
		}
		
		return closest;
	}

	// Example 5: Adjusting lightness
	icmm::CLab AdjustLightness(const icmm::CLab& color, double factor)
	{
		icmm::CLab result = color;
		double newL = color.GetL() * factor;
		result.SetL(qBound(0.0, newL, 100.0));  // Clamp to valid range
		return result;
	}
	
	icmm::CLab dark = AdjustLightness(original, 0.7);    // Darker
	icmm::CLab light = AdjustLightness(original, 1.3);   // Lighter

	// Example 6: Checking if color is neutral (gray)
	bool IsNeutral(const icmm::CLab& color, double tolerance = 5.0)
	{
		// Neutral colors have a* and b* near zero
		return (qAbs(color.GetA()) < tolerance && 
		        qAbs(color.GetB()) < tolerance);
	}

	// Example 7: Color quality control
	struct ColorTolerance {
		double maxDeltaE;
		bool isAcceptable;
	};
	
	ColorTolerance CheckColorAccuracy(const icmm::CLab& reference,
	                                  const icmm::CLab& sample,
	                                  double tolerance = 2.0)
	{
		ColorTolerance result;
		result.maxDeltaE = reference.GetDeltaE(sample);
		result.isAcceptable = (result.maxDeltaE <= tolerance);
		return result;
	}

	// Example 8: Gamut mapping - checking if color is displayable
	bool IsInSRgbGamut(const icmm::CLab& labColor)
	{
		// Convert to RGB (via transformation)
		// Check if RGB components are in [0, 1]
		// This is simplified - actual implementation uses transformations
		icmm::CRgb rgb = ConvertLabToRgb(labColor);
		
		return (rgb.GetRed() >= 0.0 && rgb.GetRed() <= 1.0 &&
		        rgb.GetGreen() >= 0.0 && rgb.GetGreen() <= 1.0 &&
		        rgb.GetBlue() >= 0.0 && rgb.GetBlue() <= 1.0);
	}

	// Example 9: Perceptual color blending
	icmm::CLab BlendLabColors(const icmm::CLab& a,
	                          const icmm::CLab& b,
	                          double t)  // 0 to 1
	{
		// Linear interpolation in Lab space gives perceptual blending
		return icmm::CLab(
			a.GetL() * (1.0 - t) + b.GetL() * t,
			a.GetA() * (1.0 - t) + b.GetA() * t,
			a.GetB() * (1.0 - t) + b.GetB() * t
		);
	}
	\endcode

	\section LabDeltaE Delta E (ΔE) Color Difference
	The ΔE value quantifies the perceptual difference between two colors:
	
	**ΔE76 (CIE 1976):**
	- ΔE = √((L1-L2)² + (a1-a2)² + (b1-b2)²)
	- Simple Euclidean distance in Lab space
	- Available via GetDeltaE() method
	
	**Interpretation:**
	- **0-1**: Not perceptible by human eye
	- **1-2**: Perceptible through close observation
	- **2-10**: Perceptible at a glance
	- **10-50**: Colors are more similar than opposite
	- **>50**: Colors are significantly different

	\section LabApplications Common Applications
	- **Color Quality Control**: Compare production colors to standards
	- **Color Matching**: Find closest colors in a palette
	- **Gamut Mapping**: Convert between color spaces while preserving appearance
	- **Color Correction**: Adjust colors perceptually
	- **Image Analysis**: Segment images by perceptual color
	- **Fashion/Design**: Specify colors independently of device

	\section LabBestPractices Best Practices
	- Use Lab for perceptual color operations (blending, matching)
	- Use Lab for color difference calculations (quality control)
	- Convert to Lab from any color space via XYZ as intermediate
	- Be aware of illuminant - Lab is relative to a white point (usually D65)
	- Use ΔE for objective color difference measurements
	- Consider ΔE2000 for more accurate perceptual differences (not in this class)
	- Lab values are typically normalized to [0,1] range in ACF but represent actual Lab ranges

	\note In ACF, component values are normalized to [0, 1] range internally, but
	      represent the actual Lab ranges (L: 0-100, a: -128 to +127, b: -128 to +127)

	\sa icmm::TComposedColor, icmm::CCieLabColorModel, icmm::CXyzToCieLabTransformation,
	    icmm::CCieLabColor

	\ingroup Color
*/
class CLab: public TComposedColor<3>
{
public:
	typedef TComposedColor<3> BaseClass;

	enum ComponentIndex
	{
		CI_L,
		CI_A,
		CI_B
	};

	CLab(double l = 0.0, double a = 0.0, double b = 0.0);
	CLab(const CLab& color);

	// access to component values
	double GetL() const;
	void SetL(double value);
	double GetA() const;
	void SetA(double value);
	double GetB() const;
	void SetB(double value);

	virtual bool Serialize(iser::IArchive& archive) override;

	double GetDeltaE(const CLab& lab) const;

	// operators
	CLab operator+(const CLab& color) const;
	CLab operator-(const CLab& color) const;
	CLab operator*(const CLab& color) const;
	CLab operator/(const CLab& color) const;

	CLab operator*(double value) const;
	CLab operator/(double value) const;

	CLab& operator=(const CLab& color);

	const CLab& operator+=(const CLab& color);
	const CLab& operator-=(const CLab& color);
	const CLab& operator*=(const CLab& color);
	const CLab& operator/=(const CLab& color);

	const CLab& operator*=(double value);
	const CLab& operator/=(double value);
};


// inline methods

inline CLab::CLab(double l, double a, double b)
{
	SetElement(CI_L, l);
	SetElement(CI_A, a);
	SetElement(CI_B, b);
}


inline CLab::CLab(const CLab& color)
:	BaseClass(color)
{
}


// access to components

inline double CLab::GetL() const
{
	return GetElement(CI_L);
}


inline void CLab::SetL(double value)
{
	SetElement(CI_L, value);
}


inline double CLab::GetA() const
{
	return GetElement(CI_A);
}


inline void CLab::SetA(double value)
{
	SetElement(CI_A, value);
}


inline double CLab::GetB() const
{
	return GetElement(CI_B);
}


inline void CLab::SetB(double value)
{
	SetElement(CI_B, value);
}


// operators

inline CLab CLab::operator+(const CLab& color) const
{
	CLab retVal = *this;

	retVal += color;

	return retVal;
}


inline CLab CLab::operator-(const CLab& color) const
{
	CLab retVal = *this;

	retVal -= color;

	return retVal;
}


inline CLab CLab::operator*(const CLab& color) const
{
	CLab retVal = *this;

	retVal *= color;

	return retVal;
}


inline CLab CLab::operator/(const CLab& color) const
{
	CLab retVal = *this;

	retVal /= color;

	return retVal;
}


inline CLab CLab::operator*(double value) const
{
	CLab retVal = *this;

	retVal *= value;

	return retVal;
}


inline CLab CLab::operator/(double value) const
{
	CLab retVal = *this;

	retVal /= value;

	return retVal;
}


inline CLab& CLab::operator=(const CLab& color)
{
	BaseClass::operator=(color);

	return *this;
}


inline const CLab& CLab::operator+=(const CLab& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CLab& CLab::operator-=(const CLab& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CLab& CLab::operator*=(const CLab& color)
{
	BaseClass::operator*=(color);

	return *this;
}


inline const CLab& CLab::operator/=(const CLab& color)
{
	BaseClass::operator/=(color);

	return *this;
}


inline const CLab& CLab::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CLab& CLab::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


