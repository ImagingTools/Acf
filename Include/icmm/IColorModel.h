// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <imath/IUnitInfo.h>
#include <icmm/IColorTransformation.h>
#include <icmm/IColorSpecification.h>

namespace icmm
{


/**
	Common interface for color model definitions in the ACF color management system.
	
	\section ColorModelPurpose Purpose
	IColorModel defines the abstract interface for color models (color spaces) in ACF.
	A color model represents a mathematical system for representing colors as tuples
	of numbers (typically 3-4 components). Examples include RGB, CMYK, HSV, CIE Lab, etc.
	This interface provides metadata about the color space and enables color transformations
	between different models.

	\section ColorModelTypes Color Model Types
	ACF supports various color model types through the ModelType enumeration:
	
	**Device-Dependent Models:**
	- **RGB** (MT_RGB): Additive color model (Red, Green, Blue) - typical for displays
	- **CMYK** (MT_COLORANTS): Subtractive model (Cyan, Magenta, Yellow, Black) - for printing
	- **HSV** (MT_HSV): Hue, Saturation, Value - intuitive for color picking
	- **HSL** (MT_HSL): Hue, Saturation, Lightness - alternative to HSV
	- **HSI** (MT_HSI): Hue, Saturation, Intensity
	
	**Device-Independent Models:**
	- **CIE XYZ** (MT_XYZ): Device-independent tristimulus values
	- **CIE Lab** (MT_LAB): Perceptually uniform color space
	- **CIE Luv** (MT_LUV): Alternative perceptually uniform space
	- **CIE LCH** (MT_LCH): Cylindrical Lab (Lightness, Chroma, Hue)
	- **Spectral** (MI_SPECTRAL): Full spectral representation
	
	**Video/Broadcast:**
	- **YIQ** (MI_YIQ), **YUV** (MI_YUV), **YCbCr** (MI_YCBCR): Video color spaces

	\section ColorModelClassifications Model Classifications
	Color models are classified along two dimensions:
	
	**Model Class:**
	- **Device-Dependent** (MC_DEVICE_DEPENDENT): Colors vary by device (RGB, CMYK)
	- **Device-Independent** (MC_DEVICE_INDEPENDENT): Absolute color (XYZ, Lab)
	
	**Color Space Class:**
	- **Additive** (CSC_ADDITIVE): Colors combine by adding light (RGB)
	- **Subtractive** (CSC_SUBSTRACTIVE): Colors combine by absorbing light (CMYK)
	- **Perceptual** (CSC_PERCEPTUAL): Based on human perception (Lab, LCH)

	\section ColorModelUsage Usage Examples
	\code
	// Example 1: Query model properties
	void InspectColorModel(const icmm::IColorModel* model)
	{
		// Get model type
		IColorModel::ModelType type = model->GetModelType();
		if (type == IColorModel::MT_RGB) {
			qDebug() << "This is an RGB color model";
		}
		
		// Check if device-dependent
		IColorModel::ModelClass modelClass = model->GetModelClass();
		if (modelClass == IColorModel::MC_DEVICE_DEPENDENT) {
			qDebug() << "Colors will vary by device";
		}
		
		// Get color space class
		IColorModel::ColorSpaceClass spaceClass = model->GetColorSpaceClass();
		if (spaceClass == IColorModel::CSC_ADDITIVE) {
			qDebug() << "Additive color mixing (light-based)";
		}
		
		// Get dimensionality (typically 3 or 4)
		int dimensions = model->GetColorSpaceDimensionality();
		qDebug() << "Color space has" << dimensions << "dimensions";
		
		// List component names
		for (int i = 0; i < dimensions; ++i) {
			QString name = model->GetColorSpaceComponentName(i);
			const imath::IUnitInfo* unit = model->GetColorSpaceComponentInfo(i);
			qDebug() << "Component" << i << ":" << name;
		}
	}

	// Example 2: Create color transformation
	void ConvertRgbToLab(const icmm::IColorModel* rgbModel,
	                     const icmm::IColorModel* labModel,
	                     const icmm::CVarColor& rgbColor)
	{
		// Create transformation from RGB to Lab
		const IColorTransformation* transform = 
			rgbModel->CreateColorTranformation(*labModel);
		
		if (transform) {
			// Apply transformation
			icmm::CVarColor labColor;
			if (transform->Calculate(rgbColor, labColor)) {
				qDebug() << "Converted to Lab:"
				         << labColor.GetElement(0) << ","
				         << labColor.GetElement(1) << ","
				         << labColor.GetElement(2);
			}
			delete transform;
		}
	}

	// Example 3: Working with RGB model
	void UseRgbModel()
	{
		icmm::CRgbColorModel rgbModel;
		
		// RGB has 3 dimensions
		int dims = rgbModel.GetColorSpaceDimensionality();  // 3
		
		// Component names: "Red", "Green", "Blue"
		QString redName = rgbModel.GetColorSpaceComponentName(0);  // "Red"
		
		// All components range from 0.0 to 1.0
		const imath::IUnitInfo* redUnit = rgbModel.GetColorSpaceComponentInfo(0);
		// redUnit->GetMinValue() returns 0.0
		// redUnit->GetMaxValue() returns 1.0
	}

	// Example 4: Custom color space conversions
	void ConvertBetweenSpaces(const icmm::CVarColor& color,
	                         icmm::ConstColorModelPtr fromModel,
	                         icmm::ConstColorModelPtr toModel)
	{
		// Create transformation
		const IColorTransformation* transform = 
			fromModel->CreateColorTranformation(*toModel);
		
		if (transform) {
			icmm::CVarColor result;
			if (transform->Calculate(color, result)) {
				// Use converted color
				ProcessColor(result);
			}
			delete transform;
		} else {
			qDebug() << "No direct transformation available";
			// May need intermediate conversion (e.g., through XYZ)
		}
	}

	// Example 5: Check model compatibility
	bool CanConvert(const IColorModel* from, const IColorModel* to)
	{
		const IColorTransformation* transform = 
			from->CreateColorTranformation(*to);
		
		bool canConvert = (transform != nullptr);
		delete transform;
		return canConvert;
	}
	\endcode

	\section ColorModelTransformations Color Transformations
	Color transformations convert colors from one model to another. Not all
	transformations are available directly:
	- **Direct**: RGB <-> HSV, RGB <-> CMYK
	- **Via XYZ**: Most conversions go through XYZ as an intermediate step
	- **Spectral**: Can be converted to any model via XYZ

	\section ColorModelBestPractices Best Practices
	- Use device-independent models (XYZ, Lab) for color comparisons
	- Store colors in the model they'll be used in (RGB for display, CMYK for print)
	- Use Lab for perceptual color differences (DeltaE calculations)
	- Cache transformations if converting many colors
	- Check transformation availability before converting

	\sa icmm::IColorObject, icmm::IColorTransformation, icmm::CVarColor,
	    icmm::CRgbColorModel, icmm::CCmykColorModel, icmm::CSpectralColorModel

	\ingroup Color
*/
class IColorModel: virtual public istd::IChangeable
{
public:
	enum ModelType
	{
		MT_HSI,
		MT_HSV,
		MT_HSL,
		MI_YIQ,
		MI_YUV,
		MI_YCBCR,
		MT_RGB,

		/**
			Substractive color model based on generic list of colorants.
			This includes CMYK, CMY, combinations with spot colors and ECG.
		*/
		MT_COLORANTS,
		MT_LUV,
		MT_LAB,
		MT_LCH,
		MT_XYZ,
		MT_YXY,

		/**
			Color model is defined by a set of spectral values.
		*/
		MI_SPECTRAL,

		/**
			Base value for the user-defined model types.
		*/
		MI_USER = 1024
	};

	enum ModelClass
	{
		MC_DEVICE_DEPENDENT,
		MC_DEVICE_INDEPENDENT
	};

	enum ColorSpaceClass
	{
		CSC_ADDITIVE,
		CSC_SUBSTRACTIVE,
		CSC_PERCEPTUAL
	};

	/**
		Gets the type of the color model.
		
		Returns the specific type identifier for this color model (e.g., MT_RGB, MT_CMYK).
		This identifies the mathematical representation and component structure.
		
		\return The ModelType enumeration value identifying this color model
		
		\sa ModelType, GetModelClass(), GetColorSpaceClass()
	*/
	virtual ModelType GetModelType() const = 0;

	/**
		Gets the classification of the color model.
		
		Indicates whether the color model is device-dependent or device-independent.
		Device-dependent models (RGB, CMYK) produce different colors on different
		devices, while device-independent models (XYZ, Lab) represent absolute colors.
		
		\return ModelClass classification
		
		\sa ModelClass, GetModelType()
	*/
	virtual ModelClass GetModelClass() const = 0;

	/**
		Gets the classification of the color space defined by this model.
		
		Indicates the physical basis of color mixing: additive (light-based),
		subtractive (pigment-based), or perceptual (human perception-based).
		
		\return ColorSpaceClass classification
		
		\sa ColorSpaceClass, GetModelType()
	*/
	virtual ColorSpaceClass GetColorSpaceClass() const = 0;

	/**
		Gets the dimensionality (number of components) of the color space.
		
		Returns the number of values needed to represent a color in this model.
		Common values are 3 (RGB, HSV, Lab) or 4 (CMYK). Spectral models may
		have higher dimensionality.
		
		\return Number of components/dimensions in the color space
		
		\code
		// RGB has 3 components
		icmm::CRgbColorModel rgbModel;
		int dims = rgbModel.GetColorSpaceDimensionality();  // returns 3
		
		// CMYK has 4 components
		icmm::CCmykColorModel cmykModel;
		dims = cmykModel.GetColorSpaceDimensionality();  // returns 4
		\endcode
		
		\sa GetColorSpaceComponentName(), GetColorSpaceComponentInfo()
	*/
	virtual int GetColorSpaceDimensionality() const = 0;

	/**
		Gets mathematical unit information for a color model component.
		
		Returns information about the mathematical unit, range, and properties
		of the specified component (axis) in the color space.
		
		\param componentIndex Zero-based index of the component (0 to dimensionality-1)
		
		\return Pointer to IUnitInfo describing the component's unit, or nullptr if
		        index is invalid. The pointer is owned by the color model.
		
		\note Most color components range from 0.0 to 1.0, but some (like Lab)
		      have different ranges (L: 0-100, a: -128 to 127, b: -128 to 127).
		
		\code
		const imath::IUnitInfo* info = model->GetColorSpaceComponentInfo(0);
		if (info) {
			double minVal = info->GetMinValue();
			double maxVal = info->GetMaxValue();
			QString unit = info->GetUnitName();
		}
		\endcode
		
		\sa GetColorSpaceComponentName(), GetColorSpaceDimensionality()
	*/
	virtual const imath::IUnitInfo* GetColorSpaceComponentInfo(int componentIndex) const = 0;

	/**
		Gets the human-readable name of a color model component.
		
		Returns the name of the component at the specified index, such as
		"Red", "Green", "Blue" for RGB, or "Lightness", "a*", "b*" for Lab.
		
		\param componentIndex Zero-based index of the component
		
		\return Component name as a QString, or empty string if index is invalid
		
		\code
		// Get RGB component names
		icmm::CRgbColorModel rgb;
		QString name0 = rgb.GetColorSpaceComponentName(0);  // "Red"
		QString name1 = rgb.GetColorSpaceComponentName(1);  // "Green"
		QString name2 = rgb.GetColorSpaceComponentName(2);  // "Blue"
		\endcode
		
		\sa GetColorSpaceComponentInfo(), GetColorSpaceDimensionality()
	*/
	virtual QString GetColorSpaceComponentName(int componentIndex) const = 0;

	/**
		Creates a color transformation for conversion to another color model.
		
		Constructs and returns a transformation object that can convert colors
		from this model to the specified target model. The transformation may
		be direct or may internally use intermediate color spaces (typically XYZ).
		
		\param otherColorModel The target color model to convert to
		
		\param transformationId Optional identifier for a specific transformation
		                       algorithm. Empty (default) uses the standard transformation.
		
		\return Pointer to the created IColorTransformation, or nullptr if no
		        transformation is available. Caller owns the returned pointer
		        and must delete it when done.
		
		\note The returned transformation is newly allocated. Delete it after use.
		\note Returns nullptr if no transformation path exists between the models.
		\note For repeated conversions, cache the transformation for performance.
		
		\code
		// Create RGB to Lab transformation
		icmm::CRgbColorModel rgbModel;
		icmm::CCieLabColorModel labModel;
		
		const IColorTransformation* transform = 
			rgbModel.CreateColorTranformation(labModel);
		
		if (transform) {
			// Use transformation
			icmm::CVarColor rgbColor(3);
			rgbColor.SetElement(0, 1.0);  // Red
			rgbColor.SetElement(1, 0.0);  // Green
			rgbColor.SetElement(2, 0.0);  // Blue
			
			icmm::CVarColor labColor;
			transform->Calculate(rgbColor, labColor);
			
			delete transform;  // Clean up
		}
		\endcode
		
		\sa IColorTransformation, CVarColor
	*/
	virtual const icmm::IColorTransformation* CreateColorTranformation(
				const IColorModel& otherColorModel,
				const QByteArray& transformationId = QByteArray()) const = 0;

	/**
		Gets the color specification for this model, if available.
		
		Returns additional specification information about the color model,
		such as white point, primaries (for RGB), or viewing conditions (for Lab).
		Not all models have specifications.
		
		\return Shared pointer to the color specification, or empty pointer if
		        no specification is available
		
		\note This is optional metadata. Most basic operations don't require it.
		
		\sa IColorSpecification
	*/
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const
	{
		return IColorSpecification::ConstColorSpecPtr();
	}
};


typedef std::shared_ptr<IColorModel> ColorModelPtr;
typedef std::shared_ptr<const IColorModel> ConstColorModelPtr;


} // namespace icmm


