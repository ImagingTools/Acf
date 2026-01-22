// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <icmm/CRgb.h>
#include <icmm/IColorModel.h>
#include <icmm/IColorObject.h>


namespace icmm
{


/**
	Interface for RGB color objects with specification support.
	
	\section RgbColorPurpose Purpose
	IRgbColor extends IColorObject to provide specific support for RGB colors.
	It guarantees access to RGB component values and optionally provides color
	specification information (white point, primaries, gamma) for device-independent
	color management.

	\section RgbColorUsage Usage Examples
	\code
	// Example 1: Accessing RGB values from color object
	void ProcessRgbColorObject(const icmm::IRgbColor* rgbColor)
	{
		// Get RGB values
		const icmm::CRgb& rgb = rgbColor->GetRgb();
		
		double red = rgb.GetRed();
		double green = rgb.GetGreen();
		double blue = rgb.GetBlue();
		
		qDebug() << "RGB:" << red << green << blue;
	}

	// Example 2: Checking color specification
	void CheckColorSpecification(const icmm::IRgbColor* rgbColor)
	{
		// Get specification if available
		IColorSpecification::ConstColorSpecPtr spec = 
			rgbColor->GetSpecification();
		
		if (spec) {
			// Has specification (e.g., sRGB, Adobe RGB)
			// Can use for accurate color management
		} else {
			// No specification - generic RGB
		}
	}

	// Example 3: Polymorphic RGB handling
	void ConvertToGrayscale(icmm::IRgbColor* rgbColor)
	{
		const icmm::CRgb& rgb = rgbColor->GetRgb();
		
		// Perceptual grayscale conversion
		double gray = 0.2126 * rgb.GetRed() + 
		             0.7152 * rgb.GetGreen() + 
		             0.0722 * rgb.GetBlue();
		
		// Could update color if interface allowed
		qDebug() << "Grayscale value:" << gray;
	}

	// Example 4: Type-safe color operations
	bool IsRgbColorBright(const icmm::IColorObject* colorObj)
	{
		// Try to cast to RGB color
		const icmm::IRgbColor* rgbColor = 
			dynamic_cast<const icmm::IRgbColor*>(colorObj);
		
		if (rgbColor) {
			const icmm::CRgb& rgb = rgbColor->GetRgb();
			double brightness = (rgb.GetRed() + rgb.GetGreen() + rgb.GetBlue()) / 3.0;
			return brightness > 0.5;
		}
		
		return false;  // Not an RGB color
	}
	\endcode

	\section RgbColorSpecifications Color Specifications
	IRgbColor can provide optional color specification information:
	- **sRGB**: Standard RGB with specific primaries and gamma 2.2
	- **Adobe RGB**: Wider gamut for professional photography
	- **ProPhoto RGB**: Very wide gamut for high-end workflows
	- **Custom profiles**: ICC profile-based specifications

	\sa icmm::IColorObject, icmm::CRgb, icmm::CRgbColor, icmm::CSRgbColor,
	    icmm::IColorSpecification

	\ingroup Color
*/
class IRgbColor : virtual public IColorObject
{
public:
	virtual const icmm::CRgb& GetRgb() const = 0;
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const = 0;

private:
	icmm::CRgb m_Rgb;
	std::shared_ptr<IColorModel> m_modelPtr;
};


} // namespace icmm


