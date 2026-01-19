#pragma once


// ACF includes
#include <iimgprint/iimgprint.h>
#include <icmm/CVarColor.h>


namespace iimgprint
{


/**
	Interface for gamut mapping strategies.
	
	\section GamutMappingPurpose Purpose
	IGamutMapper handles colors that fall outside a printer's reproducible color gamut.
	When converting between printers with different gamuts, some source colors may not
	be physically reproducible on the target printer. The gamut mapper determines how
	to render these out-of-gamut colors in a visually pleasing or accurate way.
	
	\section GamutMappingStrategies Common Strategies
	
	**Clipping:**
	- Simplest approach - colors outside gamut are clipped to gamut boundary
	- Can cause loss of detail in saturated areas
	- Fast but can produce posterization
	
	**Compression:**
	- Smoothly compresses entire color range to fit target gamut
	- Preserves relationships between colors
	- May reduce saturation of all colors, even in-gamut ones
	
	**Perceptual:**
	- Maintains visual appearance and color relationships
	- Compresses only out-of-gamut colors
	- Good for photographs and natural images
	
	**Colorimetric:**
	- Preserves exact color values for in-gamut colors
	- Clips out-of-gamut colors to nearest gamut boundary
	- Good for logos and solid colors
	
	**Saturation Preservation:**
	- Maximizes color saturation in target gamut
	- May sacrifice hue accuracy
	- Good for business graphics and charts
	
	\section GamutMappingUsage Usage Examples
	\code
	// Example 1: Apply gamut mapping
	void MapColorToGamut(const icmm::CVarColor& sourceColor,
	                     const IGamutMapper* mapper)
	{
		// Check if color is in gamut
		if (!mapper->IsInGamut(sourceColor)) {
			qDebug() << "Color is out of gamut";
			
			// Map to gamut boundary
			icmm::CVarColor mappedColor;
			if (mapper->MapToGamut(sourceColor, mappedColor)) {
				qDebug() << "Mapped color:" 
				         << mappedColor.GetElement(0) << ","
				         << mappedColor.GetElement(1) << ","
				         << mappedColor.GetElement(2);
			}
		}
	}
	
	// Example 2: Different mapping strategies
	void CompareGamutMappings(const icmm::CVarColor& outOfGamutColor,
	                          const CPrinterProfile& targetProfile)
	{
		// Create different gamut mappers
		CClippingGamutMapper clipper(targetProfile);
		CPerceptualGamutMapper perceptual(targetProfile);
		CCompressionGamutMapper compression(targetProfile);
		
		// Compare results
		icmm::CVarColor clipped, perceptualMapped, compressed;
		clipper.MapToGamut(outOfGamutColor, clipped);
		perceptual.MapToGamut(outOfGamutColor, perceptualMapped);
		compression.MapToGamut(outOfGamutColor, compressed);
		
		// Evaluate which produces best visual result
	}
	
	// Example 3: Get gamut boundary information
	void AnalyzeGamut(const IGamutMapper* mapper)
	{
		// Get gamut volume
		double volume = mapper->GetGamutVolume();
		qDebug() << "Gamut volume:" << volume;
		
		// Check coverage
		double coverage = mapper->GetGamutCoverage();
		qDebug() << "Coverage relative to reference:" << coverage;
	}
	\endcode
	
	\section GamutMappingChallenges Key Challenges
	- **Preserving Detail**: Avoid losing shadow/highlight detail in saturated areas
	- **Color Relationships**: Maintain relative differences between adjacent colors
	- **Smoothness**: Avoid visible transitions at gamut boundary
	- **Speed**: Must be fast enough for real-time or batch processing
	- **Perceptual Quality**: Mathematical accuracy vs. visual pleasing results
	
	\sa iimgprint::CPrinterProfile, iimgprint::CPrinterColorTransformation,
	    iimgprint::RenderingIntent
	
	\ingroup Color
	\ingroup ImageProcessing
*/
class IGamutMapper
{
public:
	/**
		Virtual destructor.
	*/
	virtual ~IGamutMapper() {}
	
	/**
		Checks if a color is within the printer's reproducible gamut.
		
		\param color Color to check
		\return True if color can be reproduced, false if out-of-gamut
	*/
	virtual bool IsInGamut(const icmm::CVarColor& color) const = 0;
	
	/**
		Maps an out-of-gamut color to the nearest in-gamut color.
		
		\param sourceColor Original color (may be out-of-gamut)
		\param mappedColor Resulting in-gamut color
		\return True if mapping succeeded
	*/
	virtual bool MapToGamut(const icmm::CVarColor& sourceColor,
	                        icmm::CVarColor& mappedColor) const = 0;
	
	/**
		Gets the volume of the printer's color gamut.
		
		Returns a relative measure of gamut size, typically normalized
		to a reference color space (e.g., sRGB = 1.0).
		
		\return Gamut volume (relative to reference)
	*/
	virtual double GetGamutVolume() const = 0;
	
	/**
		Gets the gamut coverage relative to a reference color space.
		
		\return Coverage ratio (0.0 to 1.0+)
	*/
	virtual double GetGamutCoverage() const = 0;
	
	/**
		Calculates the distance from a color to the gamut boundary.
		
		Positive values indicate in-gamut, negative indicate out-of-gamut.
		The magnitude indicates distance.
		
		\param color Color to measure
		\return Distance to gamut boundary (positive = in-gamut, negative = out-of-gamut)
	*/
	virtual double GetDistanceToGamutBoundary(const icmm::CVarColor& color) const = 0;
};


} // namespace iimgprint


