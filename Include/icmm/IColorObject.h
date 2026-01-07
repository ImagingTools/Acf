#pragma once


// ACF includes
#include <icmm/IColorModel.h>
#include <icmm/CVarColor.h>
#include <iser/IObject.h>


namespace icmm
{


/**
	Common interface for color objects in the ACF color management system.
	
	\section ColorObjectPurpose Purpose
	IColorObject represents a color value along with its associated color model.
	It combines a generic color representation (CVarColor) with type information
	and model metadata. This interface allows polymorphic handling of colors
	regardless of their specific color space (RGB, CMYK, Lab, etc.).

	\section ColorObjectUsage Usage Examples
	\code
	// Example 1: Working with color objects
	void ProcessColorObject(const icmm::IColorObject* colorObj)
	{
		// Get the generic color value
		icmm::CVarColor color = colorObj->GetColor();
		
		// Get the color model
		icmm::ConstColorModelPtr model = colorObj->GetColorModel();
		
		// Check what type of color this is
		if (model->GetModelType() == icmm::IColorModel::MT_RGB) {
			qDebug() << "Processing RGB color";
			// Access RGB components
			double red = color.GetElement(0);
			double green = color.GetElement(1);
			double blue = color.GetElement(2);
		}
	}

	// Example 2: Cloning color objects
	void DuplicateColor(const icmm::IColorObject* original)
	{
		// Create independent copy
		std::unique_ptr<IColorObject> copy = original->CloneIntoUniquePtr();
		
		// Modify copy without affecting original
		icmm::CVarColor color = copy->GetColor();
		color.SetElement(0, 0.5);
		// ... use modified copy
	}

	// Example 3: Converting colors through polymorphism
	std::unique_ptr<icmm::IColorObject> ConvertColor(
		const icmm::IColorObject* source,
		icmm::ConstColorModelPtr targetModel)
	{
		// Get source color and model
		icmm::CVarColor sourceColor = source->GetColor();
		icmm::ConstColorModelPtr sourceModel = source->GetColorModel();
		
		// Create transformation
		const icmm::IColorTransformation* transform = 
			sourceModel->CreateColorTranformation(*targetModel);
		
		if (transform) {
			// Transform color
			icmm::CVarColor resultColor;
			if (transform->Calculate(sourceColor, resultColor)) {
				// Create new color object in target model
				// (Implementation depends on specific color class)
			}
			delete transform;
		}
		
		return nullptr;  // Return converted color object
	}

	// Example 4: Polymorphic color storage
	void StoreColors(const QList<IColorObject*>& colors)
	{
		// Can store colors of any type
		for (const IColorObject* color : colors) {
			icmm::ConstColorModelPtr model = color->GetColorModel();
			
			// Serialize or process based on model type
			switch (model->GetModelType()) {
			case icmm::IColorModel::MT_RGB:
				SaveRgbColor(color);
				break;
			case icmm::IColorModel::MT_CMYK:
				SaveCmykColor(color);
				break;
			default:
				SaveGenericColor(color);
				break;
			}
		}
	}
	\endcode

	\section ColorObjectPolymorphism Polymorphic Design
	IColorObject enables writing color-agnostic code that works with any color
	model. This is essential for:
	- Generic color processing pipelines
	- Color management systems
	- User interfaces for color selection
	- Color format conversion tools
	- Serialization frameworks

	\sa icmm::IColorModel, icmm::CVarColor, icmm::IRgbColor, icmm::ICieLabColor,
	    iser::IObject

	\ingroup Color
*/
class IColorObject: virtual public iser::IObject
{
public:
	/**
		Gets the generic color value representation.
		
		Returns the color as a CVarColor, which is a variable-size vector
		containing the component values in the associated color model.
		
		\return CVarColor containing the color component values
		
		\code
		icmm::CVarColor color = colorObj->GetColor();
		int components = color.GetElementsCount();
		for (int i = 0; i < components; ++i) {
			double value = color.GetElement(i);
		}
		\endcode
		
		\sa CVarColor, GetColorModel()
	*/
	virtual icmm::CVarColor GetColor() const = 0;

	/**
		Gets the associated color model.
		
		Returns the color model that defines the interpretation of the color
		component values. This specifies the color space and provides metadata
		about the components.
		
		\return Shared pointer to the const color model
		
		\note The returned pointer is valid for the lifetime of the color object.
		
		\sa IColorModel, GetColor()
	*/
	virtual ConstColorModelPtr GetColorModel() const = 0;

	/**
		Creates an independent copy of the color object.
		
		Clones this color object, creating a new instance with the same color
		value and model. The clone is independent - modifications to it won't
		affect the original.
		
		\return Unique pointer to the cloned color object
		
		\note The returned unique_ptr manages the lifetime automatically.
		
		\code
		std::unique_ptr<IColorObject> copy = original->CloneIntoUniquePtr();
		// Modify copy independently
		\endcode
		
		\sa GetColor()
	*/
	virtual std::unique_ptr<IColorObject> CloneIntoUniquePtr() const = 0;
};


} // namespace icmm


