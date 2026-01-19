#pragma once


// ACF includes
#include <iimgprint/iimgprint.h>
#include <icmm/IColorTransformation.h>
#include <icmm/ISpectralColorSpecification.h>


namespace iimgprint
{


/**
	Interface for printer-to-printer color transformations based on spectral measurements.
	
	\section PrinterTransformationPurpose Purpose
	IPrinterColorTransformation extends IColorTransformation to provide color conversion
	between different printer systems. It uses spectral measurements from test charts
	printed on both printers to create accurate color mappings that preserve the
	visual appearance of printed output across different printing devices.
	
	\section PrinterTransformationApproach Spectral-Based Approach
	The transformation is built from:
	- **Source Printer Profile**: Spectral measurements of a test chart printed on source printer
	- **Target Printer Profile**: Spectral measurements of the same test chart on target printer
	- **Color Mapping**: Mathematical function mapping source colors to target colors
	
	By using spectral data instead of device-dependent RGB/CMYK, the transformation
	accounts for:
	- Different ink/toner formulations
	- Different substrates (paper types)
	- Different illumination conditions
	- Metamerism effects
	
	\section PrinterTransformationUsage Usage Examples
	\code
	// Example 1: Basic printer transformation
	void TransformImageForNewPrinter(const iimg::IBitmap* sourceImage,
	                                  const IPrinterColorTransformation* transform)
	{
		// Get image dimensions
		int width = sourceImage->GetWidth();
		int height = sourceImage->GetHeight();
		
		// Create output image
		iimg::CBitmap targetImage(width, height);
		
		// Process each pixel
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				// Get source pixel color
				icmm::CVarColor sourceColor = GetPixelColor(sourceImage, x, y);
				
				// Transform color for target printer
				icmm::CVarColor targetColor;
				if (transform->Calculate(sourceColor, targetColor)) {
					SetPixelColor(&targetImage, x, y, targetColor);
				}
			}
		}
		
		// Save or print transformed image
		targetImage.Save("output_for_printer2.tif");
	}
	
	// Example 2: Create transformation from test chart measurements
	IPrinterColorTransformation* CreatePrinterTransform(
		const icmm::ISpectralColorSpecification* sourceChartData,
		const icmm::ISpectralColorSpecification* targetChartData)
	{
		// Create printer profiles from spectral measurements
		CPrinterProfile sourceProfile(sourceChartData);
		CPrinterProfile targetProfile(targetChartData);
		
		// Build transformation between profiles
		return new CPrinterColorTransformation(
			sourceProfile,
			targetProfile,
			RenderingIntent::Perceptual
		);
	}
	
	// Example 3: Batch image conversion
	void ConvertImagesBetweenPrinters(
		const QList<QString>& imagePaths,
		const IPrinterColorTransformation* transform)
	{
		for (const QString& path : imagePaths) {
			// Load image
			iimg::CBitmap image;
			image.Load(path);
			
			// Transform colors
			TransformBitmapColors(&image, transform);
			
			// Save with new suffix
			QString outPath = path.replace(".tif", "_printer2.tif");
			image.Save(outPath);
		}
	}
	\endcode
	
	\section PrinterTransformationAccuracy Accuracy Considerations
	- Spectral-based transformations are more accurate than RGB/CMYK conversions
	- Test charts should cover the full gamut of both printers
	- Measurements should be taken under consistent illumination
	- Regular calibration is recommended for production environments
	
	\sa icmm::IColorTransformation, icmm::ISpectralColorSpecification,
	    iimgprint::CPrinterProfile, iimgprint::RenderingIntent
	
	\ingroup Color
	\ingroup ImageProcessing
*/
class IPrinterColorTransformation : virtual public icmm::IColorTransformation
{
public:
	/**
		Gets the rendering intent used for this transformation.
		
		\return The rendering intent (Perceptual, RelativeColorimetric, etc.)
	*/
	virtual RenderingIntent GetRenderingIntent() const = 0;
	
	/**
		Gets the source printer's color space type.
		
		\return Source printer color space
	*/
	virtual PrinterColorSpace GetSourceColorSpace() const = 0;
	
	/**
		Gets the target printer's color space type.
		
		\return Target printer color space
	*/
	virtual PrinterColorSpace GetTargetColorSpace() const = 0;
	
	/**
		Checks if the transformation preserves out-of-gamut colors.
		
		Some transformations may clip or compress colors that fall outside
		the target printer's reproducible gamut.
		
		\return True if out-of-gamut colors are handled, false if clipped
	*/
	virtual bool HandlesOutOfGamut() const = 0;
};


} // namespace iimgprint


