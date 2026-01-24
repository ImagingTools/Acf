// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icmm/CTristimulusSpecification.h>
#include <icmm/IColorModel.h>
#include <imath/CGeneralUnitInfo.h>


namespace icmm
{


/**
	Concrete RGB color model implementation.
	
	\section RgbColorModelPurpose Purpose
	CRgbColorModel provides a complete implementation of the RGB color model interface.
	It represents the standard additive RGB color space with three components (Red, Green, Blue)
	and optionally includes a tristimulus specification for device-independent color management.

	\section RgbColorModelFeatures Features
	- **3-dimensional color space**: Red, Green, Blue components
	- **Additive mixing**: Colors combine by adding light
	- **Device-dependent**: Without specification, colors vary by device
	- **Transformations**: Supports conversion to/from other color models
	- **Optional specification**: Can include primaries, white point, gamma for accurate color

	\section RgbColorModelUsage Usage Examples
	\code
	// Example 1: Basic RGB color model
	icmm::CRgbColorModel rgbModel;
	
	// Query properties
	IColorModel::ModelType type = rgbModel.GetModelType();  // MT_RGB
	int dims = rgbModel.GetColorSpaceDimensionality();      // 3
	
	// Get component names
	QString red = rgbModel.GetColorSpaceComponentName(0);   // "Red"
	QString green = rgbModel.GetColorSpaceComponentName(1); // "Green"
	QString blue = rgbModel.GetColorSpaceComponentName(2);  // "Blue"

	// Example 2: Creating transformations
	icmm::CRgbColorModel rgbModel;
	icmm::CHsvColorModel hsvModel;
	
	// Create RGB to HSV transformation
	const icmm::IColorTransformation* rgbToHsv = 
		rgbModel.CreateColorTranformation(hsvModel);
	
	if (rgbToHsv) {
		// Convert RGB color to HSV
		icmm::CVarColor rgb(3);
		rgb.SetElement(0, 1.0);  // Red
		rgb.SetElement(1, 0.5);  // Green
		rgb.SetElement(2, 0.0);  // Blue
		
		icmm::CVarColor hsv;
		rgbToHsv->Calculate(rgb, hsv);
		
		delete rgbToHsv;
	}

	// Example 3: RGB model with specification
	// Create specification with primaries and white point
	icmm::CTristimulusSpecification spec;
	// Set D65 white point, Rec.709 primaries, etc.
	
	icmm::CRgbColorModel calibratedRgb(spec);
	
	// This model now has color specification for accurate conversions
	IColorSpecification::ConstColorSpecPtr specPtr = 
		calibratedRgb.GetSpecification();

	// Example 4: Checking component information
	const imath::IUnitInfo* redInfo = 
		rgbModel.GetColorSpaceComponentInfo(0);
	
	if (redInfo) {
		double minVal = redInfo->GetMinValue();  // 0.0
		double maxVal = redInfo->GetMaxValue();  // 1.0
	}

	// Example 5: Converting colors via model
	void ConvertRgbToLab(const icmm::CVarColor& rgbColor)
	{
		icmm::CRgbColorModel rgbModel;
		icmm::CCieLabColorModel labModel;
		
		const icmm::IColorTransformation* transform = 
			rgbModel.CreateColorTranformation(labModel);
		
		if (transform) {
			icmm::CVarColor labColor;
			if (transform->Calculate(rgbColor, labColor)) {
				// Use Lab color
				double L = labColor.GetElement(0);
				double a = labColor.GetElement(1);
				double b = labColor.GetElement(2);
			}
			delete transform;
		}
	}
	\endcode

	\section RgbColorModelSpec Tristimulus Specification
	When created with an ITristimulusSpecification, the RGB model includes:
	- **White point**: Reference white (e.g., D65 for sRGB)
	- **Primaries**: XYZ coordinates of red, green, blue primaries
	- **Gamma**: Transfer function characteristics
	- **Use**: Enables device-independent color management

	\section RgbColorModelTransformations Available Transformations
	CRgbColorModel can create transformations to:
	- **HSV/HSL/HSI**: Intuitive color spaces (direct conversion)
	- **CMYK**: Printing colors (via simple formulas or ICC)
	- **Lab/XYZ**: Device-independent spaces (via XYZ intermediate)
	- **Other RGB**: Different RGB specifications (chromatic adaptation)

	\section RgbColorModelBestPractices Best Practices
	- Use default constructor for generic RGB (device-dependent)
	- Provide specification for color-managed workflows
	- Cache transformations when converting multiple colors
	- Use sRGB specification for web/display colors
	- Use Adobe RGB or ProPhoto RGB for photography

	\sa icmm::IColorModel, icmm::CRgb, icmm::ITristimulusSpecification,
	    icmm::CRgbToHsvTranformation, icmm::CRgbToCmykTransformation

	\ingroup Color
*/
class CRgbColorModel: virtual public IColorModel
{
public:
	CRgbColorModel();

	/**
		Sets a particular TristimulusSpecification,
		for example, if the RGB color is originating from Lab
	*/
	CRgbColorModel(const ITristimulusSpecification& spec);

	// reimplemented (IColorModel)
	virtual ModelType GetModelType() const override;
	virtual ModelClass GetModelClass() const override;
	virtual ColorSpaceClass GetColorSpaceClass() const override;
	virtual int GetColorSpaceDimensionality() const override;
	virtual const imath::IUnitInfo* GetColorSpaceComponentInfo(int componentIndex) const override;
	virtual QString GetColorSpaceComponentName(int componentIndex) const override;
	virtual const icmm::IColorTransformation* CreateColorTranformation(const IColorModel& otherColorModel, const QByteArray& transformationId) const override;
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

protected:
	imath::CGeneralUnitInfo m_unitInfo;
	CTristimulusSpecification m_spec;
};


} // namespace icmm


