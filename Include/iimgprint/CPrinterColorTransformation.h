#pragma once


// ACF includes
#include <iimgprint/IPrinterColorTransformation.h>
#include <iimgprint/CPrinterProfile.h>


namespace iimgprint
{


/**
	Implementation of printer-to-printer color transformation using spectral measurements.
	
	\section TransformationPurpose Purpose
	CPrinterColorTransformation converts colors from one printer's color space to another,
	using spectral measurements from test charts printed on both printers. This ensures
	that a document printed on the target printer will visually match the appearance
	it would have on the source printer.
	
	\section TransformationMethod Method
	The transformation uses:
	1. **Source Profile**: Spectral data showing how source printer reproduces colors
	2. **Target Profile**: Spectral data showing how target printer reproduces colors
	3. **Multi-dimensional Interpolation**: Uses imath::TMultidimensionalPolynomial to
	   interpolate between measured test chart points for smooth color transitions
	4. **Intermediate Lab Color Space**: For CMYK-to-CMYK conversion, uses device-independent
	   Lab as intermediate: Source CMYK → Lab → Target CMYK
	5. **Color Matching**: Mathematical mapping that finds the target printer color
	   that produces the closest spectral match to the source printer's output
	
	\section CMYKtoCMYK CMYK-to-CMYK Conversion Algorithm
	The primary use case for printer color transformation is CMYK-to-CMYK conversion.
	This uses a three-step process:
	
	1. **Device to Lab**: Convert source CMYK to Lab using source printer's profile
	   - Uses spectral measurements to determine Lab values for given CMYK inputs
	   - Multi-dimensional interpolation over test chart data points
	
	2. **Lab (intermediate)**: Device-independent color representation
	   - Perceptually uniform color space
	   - Enables accurate gamut mapping and color matching
	
	3. **Lab to Device**: Convert Lab to target CMYK using target printer's profile
	   - Inverse lookup: find CMYK values that produce desired Lab color
	   - Applies gamut mapping for out-of-gamut colors
	
	The transformation can handle:
	- Different color spaces (RGB to CMYK, CMYK to ExtendedGamut, etc.)
	- Different gamuts (compression/expansion as needed via gamut mapping)
	- Different rendering intents (perceptual, colorimetric, etc.)
	- Out-of-gamut colors (using appropriate gamut mapping strategy)
	
	\section TransformationUsage Usage Examples
	\code
	// Example 1: Create transformation between two printers
	void CreatePrinterTransformation()
	{
		// Load or create printer profiles
		iimgprint::CPrinterProfile sourceProfile;
		sourceProfile.LoadFromFile("printer1_profile.xml");
		
		iimgprint::CPrinterProfile targetProfile;
		targetProfile.LoadFromFile("printer2_profile.xml");
		
		// Create transformation with perceptual rendering intent
		iimgprint::CPrinterColorTransformation transform(
			sourceProfile,
			targetProfile,
			iimgprint::RenderingIntent::Perceptual
		);
		
		// Use transformation
		icmm::CVarColor sourceColor(4);  // CMYK color
		sourceColor.SetElement(0, 0.5);  // Cyan
		sourceColor.SetElement(1, 0.3);  // Magenta
		sourceColor.SetElement(2, 0.7);  // Yellow
		sourceColor.SetElement(3, 0.1);  // Black
		
		icmm::CVarColor targetColor;
		if (transform.Calculate(sourceColor, targetColor)) {
			qDebug() << "Transformed CMYK color for printer 2";
			// Internally uses: Source CMYK → Lab → Target CMYK
		}
	}
	
	// Example 2: CMYK-to-CMYK conversion with spectral profiles
	void ConvertCmykBetweenPrinters()
	{
		// Load printer profiles with spectral test chart data
		iimgprint::CPrinterProfile canonProfile, epsonProfile;
		canonProfile.LoadFromFile("canon_cmyk_profile.xml");
		epsonProfile.LoadFromFile("epson_cmyk_profile.xml");
		
		// Create CMYK-to-CMYK transformation
		iimgprint::CPrinterColorTransformation transform(
			canonProfile,
			epsonProfile,
			iimgprint::RenderingIntent::Perceptual
		);
		
		// Convert CMYK color
		icmm::CVarColor canonCmyk(4);
		canonCmyk.SetElement(0, 0.6);  // C
		canonCmyk.SetElement(1, 0.4);  // M
		canonCmyk.SetElement(2, 0.8);  // Y
		canonCmyk.SetElement(3, 0.2);  // K
		
		icmm::CVarColor epsonCmyk;
		if (transform.Calculate(canonCmyk, epsonCmyk)) {
			// epsonCmyk now contains CMYK values that will produce
			// on the Epson printer the same visual appearance as
			// canonCmyk would produce on the Canon printer
			qDebug() << "Epson CMYK: C=" << epsonCmyk.GetElement(0)
			         << " M=" << epsonCmyk.GetElement(1)
			         << " Y=" << epsonCmyk.GetElement(2)
			         << " K=" << epsonCmyk.GetElement(3);
		}
	}
	
	// Example 3: Transform entire image
	void TransformImage(const QString& inputPath, const QString& outputPath)
	{
		// Load profiles
		iimgprint::CPrinterProfile source, target;
		source.LoadFromFile("canon_profile.xml");
		target.LoadFromFile("epson_profile.xml");
		
		// Create transformation
		iimgprint::CPrinterColorTransformation transform(
			source, target,
			iimgprint::RenderingIntent::RelativeColorimetric
		);
		
		// Load image (pseudocode - actual implementation depends on iimg API)
		iimg::CBitmap image;
		image.Load(inputPath);
		
		// Transform each pixel
		// (Actual pixel iteration depends on iimg::IBitmap interface)
		TransformBitmapColors(&image, &transform);
		
		// Save result
		image.Save(outputPath);
	}
	
	// Example 3: Different rendering intents
	void CompareRenderingIntents(
		const iimgprint::CPrinterProfile& source,
		const iimgprint::CPrinterProfile& target,
		const icmm::CVarColor& testColor)
	{
		// Try different rendering intents
		QList<RenderingIntent> intents = {
			RenderingIntent::Perceptual,
			RenderingIntent::RelativeColorimetric,
			RenderingIntent::Saturation,
			RenderingIntent::AbsoluteColorimetric
		};
		
		for (RenderingIntent intent : intents) {
			CPrinterColorTransformation transform(source, target, intent);
			
			icmm::CVarColor result;
			if (transform.Calculate(testColor, result)) {
				qDebug() << "Intent:" << (int)intent 
				         << "Result:" << result.GetElement(0);
			}
		}
	}
	\endcode
	
	\section TransformationPerformance Performance
	- Profile creation is expensive, but done once per printer calibration
	- Transformation calculation is fast enough for real-time pixel processing
	- Consider caching transformations when processing multiple images
	
	\sa iimgprint::IPrinterColorTransformation, iimgprint::CPrinterProfile,
	    iimgprint::RenderingIntent, icmm::IColorTransformation
	
	\ingroup Color
	\ingroup ImageProcessing
*/
class CPrinterColorTransformation : public IPrinterColorTransformation
{
public:
	/**
		Constructs a transformation between two printer profiles.
		
		\param sourceProfile Profile of the source printer
		\param targetProfile Profile of the target printer
		\param intent Rendering intent for the transformation
	*/
	CPrinterColorTransformation(
		const CPrinterProfile& sourceProfile,
		const CPrinterProfile& targetProfile,
		RenderingIntent intent = RenderingIntent::Perceptual);
	
	/**
		Destructor.
	*/
	virtual ~CPrinterColorTransformation();
	
	// IPrinterColorTransformation interface
	virtual RenderingIntent GetRenderingIntent() const override;
	virtual PrinterColorSpace GetSourceColorSpace() const override;
	virtual PrinterColorSpace GetTargetColorSpace() const override;
	virtual bool HandlesOutOfGamut() const override;
	
	// IColorTransformation interface (via IPrinterColorTransformation)
	/**
		Transforms a color from source printer space to target printer space.
		
		\param input Color in source printer's color space
		\param output Resulting color in target printer's color space
		\return True if transformation succeeded
	*/
	virtual bool Calculate(const icmm::CVarColor& input, icmm::CVarColor& output) const override;

private:
	class Impl;
	Impl* m_impl;
};


} // namespace iimgprint


