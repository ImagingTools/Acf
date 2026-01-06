#pragma once


// ACF includes
#include <iser/IArchive.h>
#include <icmm/TComposedColor.h>


namespace icmm
{


/**
	Primitive for representation of CMYK (Cyan, Magenta, Yellow, Black) color values.
	
	\section CmykPurpose Purpose
	CCmyk represents colors in the CMYK subtractive color model, which is the standard
	for color printing and physical media. Unlike RGB (additive), CMYK works by
	subtracting colors from white light, as inks absorb certain wavelengths. The K
	(Key/Black) component is added for practical printing reasons - combining CMY
	inks doesn't produce true black and uses excessive ink.

	\section CmykColorModel CMYK Color Model
	CMYK is a subtractive color model where colors are created by absorbing light:
	- **Cyan (C)**: Absorbs red light, reflects blue and green
	- **Magenta (M)**: Absorbs green light, reflects red and blue
	- **Yellow (Y)**: Absorbs blue light, reflects red and green
	- **Black (K)**: Key plate, provides true black and reduces ink usage

	**Color mixing:**
	- Cyan + Magenta = Blue
	- Cyan + Yellow = Green
	- Magenta + Yellow = Red
	- Cyan + Magenta + Yellow = Dark Brown (not true black)
	- Full Black component (K=1.0) = True Black

	\section CmykUsage Usage Examples
	\code
	// Example 1: Creating CMYK colors
	icmm::CCmyk white(0.0, 0.0, 0.0, 0.0);    // No ink
	icmm::CCmyk black(0.0, 0.0, 0.0, 1.0);    // Full black ink
	icmm::CCmyk cyan(1.0, 0.0, 0.0, 0.0);     // Pure cyan
	icmm::CCmyk magenta(0.0, 1.0, 0.0, 0.0);  // Pure magenta
	icmm::CCmyk yellow(0.0, 0.0, 1.0, 0.0);   // Pure yellow
	
	// Mixed colors
	icmm::CCmyk blue(1.0, 1.0, 0.0, 0.0);     // Cyan + Magenta
	icmm::CCmyk green(1.0, 0.0, 1.0, 0.0);    // Cyan + Yellow
	icmm::CCmyk red(0.0, 1.0, 1.0, 0.0);      // Magenta + Yellow

	// Example 2: Component access
	icmm::CCmyk color;
	color.SetC(0.2);  // 20% cyan
	color.SetM(0.5);  // 50% magenta
	color.SetY(0.8);  // 80% yellow
	color.SetK(0.1);  // 10% black
	
	double cyan = color.GetC();
	double magenta = color.GetM();
	double yellow = color.GetY();
	double black = color.GetK();

	// Example 3: RGB to CMYK conversion (naive approach)
	icmm::CCmyk RgbToCmykNaive(const icmm::CRgb& rgb)
	{
		// Simple CMY conversion
		double c = 1.0 - rgb.GetRed();
		double m = 1.0 - rgb.GetGreen();
		double y = 1.0 - rgb.GetBlue();
		
		// Extract black component (UCR - Under Color Removal)
		double k = qMin(c, qMin(m, y));
		
		// Reduce CMY by black amount
		if (k < 1.0) {
			c = (c - k) / (1.0 - k);
			m = (m - k) / (1.0 - k);
			y = (y - k) / (1.0 - k);
		} else {
			c = m = y = 0.0;
		}
		
		return icmm::CCmyk(c, m, y, k);
	}

	// Example 4: Estimating ink coverage
	double GetInkCoverage(const icmm::CCmyk& color)
	{
		// Total ink as percentage (400% max for CMYK)
		return (color.GetC() + color.GetM() + 
		        color.GetY() + color.GetK()) * 100.0;
	}
	
	icmm::CCmyk richBlack(0.6, 0.4, 0.4, 1.0);  // Rich black
	double coverage = GetInkCoverage(richBlack);  // 240%
	
	// Example 5: Checking ink limits
	bool IsWithinInkLimit(const icmm::CCmyk& color, double maxCoverage = 3.0)
	{
		double total = color.GetC() + color.GetM() + 
		              color.GetY() + color.GetK();
		return total <= maxCoverage;
	}

	// Example 6: Creating rich black
	icmm::CCmyk CreateRichBlack(double blackAmount = 1.0,
	                            double cmy = 0.4)
	{
		// Rich black uses CMY + K for deeper black
		return icmm::CCmyk(cmy, cmy, cmy, blackAmount);
	}
	
	icmm::CCmyk standardBlack = CreateRichBlack(1.0, 0.0);   // Pure K
	icmm::CCmyk richBlack = CreateRichBlack(1.0, 0.4);       // K + CMY

	// Example 7: Gray component replacement (GCR)
	icmm::CCmyk ApplyGCR(const icmm::CCmyk& color, double gcrAmount)
	{
		// Find minimum CMY value
		double minCMY = qMin(color.GetC(), qMin(color.GetM(), color.GetY()));
		
		// Replace with black
		double addBlack = minCMY * gcrAmount;
		
		return icmm::CCmyk(
			color.GetC() - addBlack,
			color.GetM() - addBlack,
			color.GetY() - addBlack,
			color.GetK() + addBlack
		);
	}

	// Example 8: Spot color separation
	struct SpotColorData {
		QString name;
		icmm::CCmyk processEquivalent;
		double angle;  // Screen angle for printing
	};
	
	SpotColorData CreateSpotColor(const QString& name,
	                              const icmm::CCmyk& cmyk)
	{
		SpotColorData spot;
		spot.name = name;
		spot.processEquivalent = cmyk;
		spot.angle = 45.0;  // Default angle
		return spot;
	}
	\endcode

	\section CmykPrinting CMYK and Printing
	**Ink Coverage:**
	- Total ink coverage (TAC) is typically limited to 240-340%
	- High coverage causes drying problems and paper saturation
	- GCR (Gray Component Replacement) reduces CMY, increases K
	- UCR (Under Color Removal) removes CMY where K exists

	**Black Generation:**
	- **Process Black**: K=1.0, C=M=Y=0 (standard black)
	- **Rich Black**: K=1.0 with some CMY (deeper, richer black)
	- **Super Black**: Maximum K with CMY support (photography)

	**Screen Angles:**
	- Cyan: 105°, Magenta: 75°, Yellow: 90°, Black: 45°
	- Prevents moiré patterns in halftone printing

	\section CmykBestPractices Best Practices
	- Monitor ink coverage - keep TAC below printer limits
	- Use rich black for large dark areas
	- Apply GCR/UCR for better print quality and ink economy
	- Convert from RGB using proper ICC profiles, not naive algorithms
	- Test colors on actual print media - screen won't match exactly
	- Consider paper type - coated vs. uncoated affects color
	- Use CMYK color model class with proper transformations for accuracy

	\section CmykLimitations Limitations
	- Smaller color gamut than RGB (can't reproduce all screen colors)
	- Device-dependent (varies by printer, ink, paper)
	- Not suitable for screen display - convert to RGB
	- Requires careful color management for accuracy

	\note Component values range from 0.0 (no ink) to 1.0 (full ink coverage)

	\sa icmm::TComposedColor, icmm::CCmykColorModel, icmm::CRgb,
	    icmm::CRgbToCmykTransformation, icmm::CCmykToRgbTransformation

	\ingroup Color
*/
class CCmyk: public TComposedColor<4>
{
public:
	typedef TComposedColor<4> BaseClass;

	enum ComponentIndex
	{
		CI_CYAN,
		CI_MAGENTA,
		CI_YELLOW,
		CI_BLACK,
		CI_LAST = CI_BLACK
	};

	CCmyk(double c = 0.0, double m = 0.0, double y = 0.0, double k = 0.0);
	CCmyk(const CCmyk& color);

	// access to component values
	double GetC() const;
	void SetC(double value);
	double GetM() const;
	void SetM(double value);
	double GetY() const;
	void SetY(double value);
	double GetK() const;
	void SetK(double value);

	virtual bool Serialize(iser::IArchive& archive) override;

	// operators
	CCmyk operator+(const CCmyk& color) const;
	CCmyk operator-(const CCmyk& color) const;
	CCmyk operator*(const CCmyk& color) const;
	CCmyk operator/(const CCmyk& color) const;

	CCmyk operator*(double value) const;
	CCmyk operator/(double value) const;

	CCmyk& operator=(const CCmyk& color);

	const CCmyk& operator+=(const CCmyk& color);
	const CCmyk& operator-=(const CCmyk& color);
	const CCmyk& operator*=(const CCmyk& color);
	const CCmyk& operator/=(const CCmyk& color);

	const CCmyk& operator*=(double value);
	const CCmyk& operator/=(double value);
};


// inline methods

inline CCmyk::CCmyk(double c, double m, double y, double k)
{
	SetElement(CI_CYAN, c);
	SetElement(CI_MAGENTA, m);
	SetElement(CI_YELLOW, y);
	SetElement(CI_BLACK, k);
}


inline CCmyk::CCmyk(const CCmyk& color)
:	BaseClass(color)
{
}


// access to components

inline double CCmyk::GetC() const
{
	return GetElement(CI_CYAN);
}


inline void CCmyk::SetC(double value)
{
	SetElement(CI_CYAN, value);
}


inline double CCmyk::GetM() const
{
	return GetElement(CI_MAGENTA);
}


inline void CCmyk::SetM(double value)
{
	SetElement(CI_MAGENTA, value);
}


inline double CCmyk::GetY() const
{
	return GetElement(CI_YELLOW);
}


inline void CCmyk::SetY(double value)
{
	SetElement(CI_YELLOW, value);
}


inline double CCmyk::GetK() const
{
	return GetElement(CI_BLACK);
}


inline void CCmyk::SetK(double value)
{
	SetElement(CI_BLACK, value);
}


// operators

inline CCmyk CCmyk::operator+(const CCmyk& color) const
{
	CCmyk retVal = *this;

	retVal += color;

	return retVal;
}


inline CCmyk CCmyk::operator-(const CCmyk& color) const
{
	CCmyk retVal = *this;

	retVal -= color;

	return retVal;
}


inline CCmyk CCmyk::operator*(const CCmyk& color) const
{
	CCmyk retVal = *this;

	retVal *= color;

	return retVal;
}


inline CCmyk CCmyk::operator/(const CCmyk& color) const
{
	CCmyk retVal = *this;

	retVal /= color;

	return retVal;
}


inline CCmyk CCmyk::operator*(double value) const
{
	CCmyk retVal = *this;

	retVal *= value;

	return retVal;
}


inline CCmyk CCmyk::operator/(double value) const
{
	CCmyk retVal = *this;

	retVal /= value;

	return retVal;
}


inline CCmyk& CCmyk::operator=(const CCmyk& color)
{
	BaseClass::operator=(color);

	return *this;
}


inline const CCmyk& CCmyk::operator+=(const CCmyk& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CCmyk& CCmyk::operator-=(const CCmyk& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CCmyk& CCmyk::operator*=(const CCmyk& color)
{
	BaseClass::operator*=(color);

	return *this;
}


inline const CCmyk& CCmyk::operator/=(const CCmyk& color)
{
	BaseClass::operator/=(color);

	return *this;
}


inline const CCmyk& CCmyk::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CCmyk& CCmyk::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


