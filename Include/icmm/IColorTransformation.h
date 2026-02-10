// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <imath/TIMathFunction.h>
#include <icmm/CVarColor.h>


namespace icmm
{


/**
	Basic interface for color transformations between color models.
	
	\section ColorTransformationPurpose Purpose
	IColorTransformation defines a mathematical function that converts colors
	from one color model to another. It extends TIMathFunction to provide
	color-specific transformation capabilities, taking a CVarColor as input
	and producing a CVarColor as output.

	\section ColorTransformationTypes Transformation Types
	Common color transformations include:
	- **RGB <-> HSV**: Conversion between RGB and HSV color spaces
	- **RGB <-> CMYK**: Conversion for display vs. print
	- **RGB -> XYZ -> Lab**: Device-dependent to perceptually uniform space
	- **Spectral -> XYZ**: Full spectral data to tristimulus values
	- **Any -> XYZ -> Any**: XYZ often serves as an intermediate color space

	\section ColorTransformationUsage Usage Examples
	\code
	// Example 1: Basic color transformation
	void ConvertRgbToHsv(const icmm::CRgb& rgb)
	{
		// Get source and target models
		icmm::CRgbColorModel rgbModel;
		icmm::CHsvColorModel hsvModel;
		
		// Create transformation
		const IColorTransformation* transform = 
			rgbModel.CreateColorTranformation(hsvModel);
		
		if (transform) {
			// Convert RGB to CVarColor
			icmm::CVarColor rgbColor(3);
			rgbColor.SetElement(0, rgb.GetRed());
			rgbColor.SetElement(1, rgb.GetGreen());
			rgbColor.SetElement(2, rgb.GetBlue());
			
			// Apply transformation
			icmm::CVarColor hsvColor;
			if (transform->Calculate(rgbColor, hsvColor)) {
				// Extract HSV components
				double hue = hsvColor.GetElement(0);
				double saturation = hsvColor.GetElement(1);
				double value = hsvColor.GetElement(2);
				
				qDebug() << "H:" << hue 
				         << "S:" << saturation 
				         << "V:" << value;
			}
			
			delete transform;
		}
	}

	// Example 2: Batch color conversion
	void ConvertColorPalette(const QList<icmm::CVarColor>& sourceColors,
	                         icmm::ConstColorModelPtr sourceModel,
	                         icmm::ConstColorModelPtr targetModel)
	{
		// Create transformation once
		const IColorTransformation* transform = 
			sourceModel->CreateColorTranformation(*targetModel);
		
		if (!transform) {
			qWarning() << "No transformation available";
			return;
		}
		
		// Convert all colors using same transformation
		QList<icmm::CVarColor> results;
		for (const icmm::CVarColor& color : sourceColors) {
			icmm::CVarColor result;
			if (transform->Calculate(color, result)) {
				results.append(result);
			}
		}
		
		delete transform;
		
		// Process converted colors
		ProcessColors(results);
	}

	// Example 3: Color space conversion chain
	icmm::CVarColor ConvertRgbToLab(const icmm::CVarColor& rgb)
	{
		// RGB -> XYZ -> Lab (common conversion path)
		icmm::CRgbColorModel rgbModel;
		icmm::CXyzColorModel xyzModel;  // Hypothetical
		icmm::CCieLabColorModel labModel;
		
		// First: RGB to XYZ
		const IColorTransformation* rgbToXyz = 
			rgbModel.CreateColorTranformation(xyzModel);
		
		icmm::CVarColor xyz;
		if (rgbToXyz && rgbToXyz->Calculate(rgb, xyz)) {
			delete rgbToXyz;
			
			// Second: XYZ to Lab
			const IColorTransformation* xyzToLab = 
				xyzModel.CreateColorTranformation(labModel);
			
			icmm::CVarColor lab;
			if (xyzToLab && xyzToLab->Calculate(xyz, lab)) {
				delete xyzToLab;
				return lab;
			}
			delete xyzToLab;
		} else {
			delete rgbToXyz;
		}
		
		return icmm::CVarColor();  // Return empty on failure
	}

	// Example 4: Transformation with error handling
	bool TransformColor(const icmm::CVarColor& input,
	                   icmm::CVarColor& output,
	                   const IColorTransformation* transform)
	{
		if (!transform) {
			qWarning() << "Null transformation";
			return false;
		}
		
		if (!transform->Calculate(input, output)) {
			qWarning() << "Transformation failed";
			return false;
		}
		
		// Validate output
		int components = output.GetElementsCount();
		if (components == 0) {
			qWarning() << "Invalid output color";
			return false;
		}
		
		return true;
	}
	\endcode

	\section ColorTransformationPerformance Performance Considerations
	- **Cache transformations**: Creating transformations has overhead; reuse them
	  when converting multiple colors
	- **Direct transformations**: Some conversions (RGB<->HSV) are direct and fast
	- **Indirect transformations**: Others go through XYZ as intermediate, slower
	- **Memory management**: Always delete transformation objects when done

	\section ColorTransformationAccuracy Accuracy Considerations
	- Color transformations may involve approximations
	- Some conversions are lossy (RGB -> CMYK -> RGB may not round-trip exactly)
	- Device-dependent models (RGB, CMYK) need calibration for accuracy
	- Use device-independent spaces (Lab, XYZ) for accurate color matching

	\sa icmm::IColorModel, icmm::CVarColor, imath::TIMathFunction,
	    icmm::CRgbToHsvTranformation, icmm::CRgbToCmykTransformation

	\ingroup Color
*/
class IColorTransformation: virtual public imath::TIMathFunction<icmm::CVarColor, icmm::CVarColor>
{
};


} // namespace icmm


