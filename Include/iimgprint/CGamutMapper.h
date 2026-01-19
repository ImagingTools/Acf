#pragma once


// ACF includes
#include <iimgprint/IGamutMapper.h>
#include <iimgprint/CPrinterProfile.h>


namespace iimgprint
{


/**
	Gamut mapper that uses clipping to handle out-of-gamut colors.
	
	\section ClippingApproach Clipping Approach
	This mapper uses the simplest strategy: colors outside the target gamut
	are clipped to the nearest point on the gamut boundary. While fast and
	preserving in-gamut colors perfectly, it can cause loss of detail in
	highly saturated areas and may produce visible posterization.
	
	\section ClippingUseCases When to Use
	- Fast processing is required
	- Source and target gamuts are similar
	- Content has few out-of-gamut colors
	- Absolute color accuracy is critical for in-gamut colors
	
	\section ClippingLimitations Limitations
	- Loss of detail in saturated areas
	- Can cause posterization effects
	- Abrupt transitions at gamut boundary
	- Not suitable for photographs with wide gamut
	
	\code
	// Example: Use clipping for logo with solid colors
	iimgprint::CPrinterProfile targetProfile;
	targetProfile.LoadFromFile("office_printer.xml");
	
	iimgprint::CClippingGamutMapper clipper(targetProfile);
	
	// Map a bright red that might be out-of-gamut
	icmm::CVarColor brightRed(3);
	brightRed.SetElement(0, 1.0);  // R
	brightRed.SetElement(1, 0.0);  // G
	brightRed.SetElement(2, 0.0);  // B
	
	icmm::CVarColor clippedRed;
	if (!clipper.IsInGamut(brightRed)) {
		clipper.MapToGamut(brightRed, clippedRed);
		// Use clippedRed for printing
	}
	\endcode
	
	\sa iimgprint::IGamutMapper, iimgprint::CPerceptualGamutMapper,
	    iimgprint::CPrinterProfile
	
	\ingroup Color
	\ingroup ImageProcessing
*/
class CClippingGamutMapper : public IGamutMapper
{
public:
	/**
		Constructs a clipping gamut mapper for the given printer profile.
		
		\param targetProfile Printer profile defining the target gamut
	*/
	explicit CClippingGamutMapper(const CPrinterProfile& targetProfile);
	
	/**
		Destructor.
	*/
	virtual ~CClippingGamutMapper();
	
	// IGamutMapper interface
	virtual bool IsInGamut(const icmm::CVarColor& color) const override;
	virtual bool MapToGamut(const icmm::CVarColor& sourceColor,
	                        icmm::CVarColor& mappedColor) const override;
	virtual double GetGamutVolume() const override;
	virtual double GetGamutCoverage() const override;
	virtual double GetDistanceToGamutBoundary(const icmm::CVarColor& color) const override;

private:
	class Impl;
	Impl* m_impl;
};


/**
	Gamut mapper that uses perceptual compression for out-of-gamut colors.
	
	\section PerceptualApproach Perceptual Approach
	This mapper preserves the visual appearance and relationships between colors.
	It smoothly compresses colors near and outside the gamut boundary while leaving
	colors deep within the gamut unchanged. This produces the most visually pleasing
	results for photographic content.
	
	\section PerceptualAlgorithm Algorithm
	Uses a sigmoidal compression function that:
	1. Leaves in-gamut colors largely unchanged
	2. Smoothly compresses colors approaching gamut boundary
	3. Maps all out-of-gamut colors into gamut
	4. Preserves hue angles where possible
	5. Maintains perceptual color differences
	
	\section PerceptualUseCases When to Use
	- Photographs and natural images
	- Wide-gamut source (display/camera) to narrow-gamut target
	- Visual quality is more important than colorimetric accuracy
	- Preserving smooth gradients is critical
	
	\code
	// Example: Use perceptual mapping for photograph
	iimgprint::CPrinterProfile sourceProfile, targetProfile;
	sourceProfile.LoadFromFile("wide_gamut_camera.xml");
	targetProfile.LoadFromFile("office_printer.xml");
	
	iimgprint::CPerceptualGamutMapper perceptual(targetProfile);
	
	// Process image pixels
	for (const auto& pixel : imagePixels) {
		icmm::CVarColor mappedPixel;
		perceptual.MapToGamut(pixel, mappedPixel);
		// Use mappedPixel for printing
	}
	\endcode
	
	\sa iimgprint::IGamutMapper, iimgprint::CClippingGamutMapper,
	    iimgprint::CPrinterProfile
	
	\ingroup Color
	\ingroup ImageProcessing
*/
class CPerceptualGamutMapper : public IGamutMapper
{
public:
	/**
		Constructs a perceptual gamut mapper for the given printer profile.
		
		\param targetProfile Printer profile defining the target gamut
		\param compressionKnee Point where compression begins (0.0-1.0, default 0.8)
	*/
	explicit CPerceptualGamutMapper(const CPrinterProfile& targetProfile,
	                                double compressionKnee = 0.8);
	
	/**
		Destructor.
	*/
	virtual ~CPerceptualGamutMapper();
	
	/**
		Sets the compression knee point.
		
		The knee point determines where gamut compression begins.
		0.8 means compression starts at 80% of gamut radius.
		
		\param knee Knee point (0.0 to 1.0)
	*/
	void SetCompressionKnee(double knee);
	
	/**
		Gets the current compression knee point.
		
		\return Knee point value
	*/
	double GetCompressionKnee() const;
	
	// IGamutMapper interface
	virtual bool IsInGamut(const icmm::CVarColor& color) const override;
	virtual bool MapToGamut(const icmm::CVarColor& sourceColor,
	                        icmm::CVarColor& mappedColor) const override;
	virtual double GetGamutVolume() const override;
	virtual double GetGamutCoverage() const override;
	virtual double GetDistanceToGamutBoundary(const icmm::CVarColor& color) const override;

private:
	class Impl;
	Impl* m_impl;
};


} // namespace iimgprint


