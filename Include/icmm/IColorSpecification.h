// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// STL includes
#include <memory>

// ACF includes
#include <istd/IChangeable.h>
#include <icmm/icmm.h>


namespace icmm
{


/**
	Interface for color space specification information.
	
	\section ColorSpecPurpose Purpose
	IColorSpecification provides metadata about how a color space is defined.
	This includes information about the reference white point, color primaries
	(for RGB), viewing conditions, and other parameters needed for accurate
	color reproduction and conversion between color spaces.

	\section ColorSpecTypes Specification Types
	Color specifications fall into two categories:
	
	**Tristimulus-based (SpecType::Tristimulus):**
	- RGB spaces (sRGB, Adobe RGB, ProPhoto RGB)
	- Defined by primaries and white point
	- Use 3 values to represent any color
	- Examples: sRGB (Rec. 709), Adobe RGB (1998), DCI-P3
	
	**Spectral-based (SpecType::Spectral):**
	- Full wavelength representation
	- Defined by spectral power distribution
	- Use many values across wavelength range
	- Most accurate but computationally expensive

	\section ColorSpecUsage Usage Examples
	\code
	// Example 1: Checking specification type
	void InspectSpecification(const icmm::IColorSpecification* spec)
	{
		IColorSpecification::SpecType type = spec->GetSpecificationType();
		
		switch (type) {
		case IColorSpecification::SpecType::Tristimulus:
			qDebug() << "Tristimulus-based (e.g., RGB)";
			// Can cast to ITristimulusSpecification
			break;
			
		case IColorSpecification::SpecType::Spectral:
			qDebug() << "Spectral-based (full spectrum)";
			// Can cast to ISpectralColorSpecification
			break;
		}
	}

	// Example 2: Working with RGB specifications
	void HandleRgbSpec(IColorSpecification::ConstColorSpecPtr spec)
	{
		if (spec && spec->GetSpecificationType() == 
		    IColorSpecification::SpecType::Tristimulus) 
		{
			// Can use tristimulus-specific operations
			const icmm::ITristimulusSpecification* tristim = 
				dynamic_cast<const icmm::ITristimulusSpecification*>(spec.get());
			
			if (tristim) {
				// Access primaries, white point, gamma, etc.
				icmm::CVarColor whitePoint = tristim->GetWhitePoint();
			}
		}
	}

	// Example 3: Comparing specifications
	bool SpecificationsMatch(IColorSpecification::ConstColorSpecPtr spec1,
	                        IColorSpecification::ConstColorSpecPtr spec2)
	{
		if (!spec1 || !spec2) {
			return spec1 == spec2;  // Both null or one null
		}
		
		// Different types don't match
		if (spec1->GetSpecificationType() != spec2->GetSpecificationType()) {
			return false;
		}
		
		// Would need type-specific comparison
		// ...
		
		return false;
	}

	// Example 4: Creating color with specification
	icmm::IColorObject* CreateColorWithSpec(
		const icmm::CVarColor& colorValue,
		IColorSpecification::ConstColorSpecPtr spec)
	{
		// Create appropriate color object based on specification
		if (spec) {
			switch (spec->GetSpecificationType()) {
			case IColorSpecification::SpecType::Tristimulus:
				// Create RGB color with specification
				// return new CRgbColor(colorValue, spec);
				break;
				
			case IColorSpecification::SpecType::Spectral:
				// Create spectral color with specification
				// return new CSpectralColor(colorValue, spec);
				break;
			}
		}
		
		return nullptr;
	}

	// Example 5: Specification-aware conversion
	bool CanConvertAccurately(IColorSpecification::ConstColorSpecPtr sourceSpec,
	                         IColorSpecification::ConstColorSpecPtr destSpec)
	{
		// Spectral -> Tristimulus: Always possible, may lose info
		if (sourceSpec && sourceSpec->GetSpecificationType() == 
		    IColorSpecification::SpecType::Spectral) {
			return true;
		}
		
		// Tristimulus -> Tristimulus: Depends on gamuts
		if (sourceSpec && destSpec &&
		    sourceSpec->GetSpecificationType() == IColorSpecification::SpecType::Tristimulus &&
		    destSpec->GetSpecificationType() == IColorSpecification::SpecType::Tristimulus) {
			// Check if gamuts overlap
			return true;  // Simplified
		}
		
		return false;
	}
	\endcode

	\section ColorSpecExamples Common Specifications
	**RGB Specifications:**
	- **sRGB**: Standard RGB (Rec. 709 primaries, D65 white, gamma 2.2)
	- **Adobe RGB (1998)**: Wider gamut for professional work
	- **ProPhoto RGB**: Very wide gamut, includes most visible colors
	- **DCI-P3**: Digital cinema standard
	
	**Purpose:**
	- Ensures consistent color reproduction
	- Enables accurate color space conversions
	- Supports color management workflows
	- Provides device characterization

	\section ColorSpecBestPractices Best Practices
	- Always check specification type before casting
	- Use specifications for accurate color conversions
	- Match specifications when comparing colors
	- Store specification with colors for reproducibility
	- Use appropriate specification for intended output (display vs. print)

	\sa icmm::ITristimulusSpecification, icmm::ISpectralColorSpecification,
	    icmm::IColorModel, icmm::IWhitePointProvider

	\ingroup Color
*/
class IColorSpecification: virtual public istd::IChangeable
{
public:
	typedef std::shared_ptr<const IColorSpecification> ConstColorSpecPtr;
	typedef std::shared_ptr<IColorSpecification> ColorSpecPtr;

	enum class SpecType
	{
		/**
			Tristimulus-based specification (e.g., RGB with primaries and white point).
		*/
		Tristimulus,
		
		/**
			Spectral-based specification (full wavelength representation).
		*/
		Spectral
	};

	/**
		Gets the logical type of the specification.
		
		Returns whether this is a tristimulus-based specification (3 values like RGB)
		or a spectral-based specification (full wavelength distribution).
		
		\return SpecType indicating Tristimulus or Spectral
		
		\sa SpecType
	*/
	virtual SpecType GetSpecificationType() const = 0;
};


} // namespace icmm


