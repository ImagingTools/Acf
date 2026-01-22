// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icmm/ISpectralColorSpecification.h>
#include <icmm/IIlluminant.h>


namespace icmm
{


/**
	Interface for tristimulus-based color specifications.
	
	\section TristimulusSpecPurpose Purpose
	ITristimulusSpecification extends IColorSpecification for color spaces based on
	tristimulus values (three-component color representation like XYZ or RGB). It provides
	access to the illuminant (light source), observer type (standard observer), and
	calculation method used for color conversions.

	\section TristimulusSpecConcept Tristimulus Values Concept
	Tristimulus values are based on the trichromatic theory of human color vision:
	- **Three receptors**: Human eyes have three types of color receptors (cones)
	- **Three values**: Any color can be matched with three primary stimuli
	- **XYZ space**: CIE XYZ is the fundamental tristimulus color space
	- **RGB spaces**: Monitor colors are tristimulus-based (derived from XYZ)

	\section TristimulusSpecComponents Components
	**Illuminant:**
	- Light source specification (D65, D50, A, etc.)
	- Defines the reference white point
	- Critical for chromatic adaptation
	
	**Observer Type:**
	- Standard observer (2° or 10° field of view)
	- CIE 1931 (2°) for small color patches
	- CIE 1964 (10°) for larger fields
	
	**Calculation Method:**
	- ASTM table type for spectral calculations
	- Different resolutions/wavelength ranges
	- Affects accuracy of conversions

	\section TristimulusSpecUsage Usage Examples
	\code
	// Example 1: Accessing tristimulus specification
	void InspectTristimulusSpec(const icmm::ITristimulusSpecification* spec)
	{
		// Get illuminant
		std::shared_ptr<icmm::IIlluminant> illuminant = spec->GetIlluminant();
		if (illuminant) {
			QString name = illuminant->GetIlluminantName();  // "D65", "D50", etc.
			icmm::CVarColor whitePoint = illuminant->GetWhitePoint();
			qDebug() << "Illuminant:" << name;
		}
		
		// Get observer type
		icmm::ObserverType observer = spec->GetObserverType();
		// observer == OT_CIE_1931_2_DEGREE or OT_CIE_1964_10_DEGREE
		
		// Get calculation method
		icmm::AstmTableType method = spec->GetMethod();
	}

	// Example 2: Creating sRGB specification
	icmm::TristimulusPtr CreateSRgbSpec()
	{
		// sRGB uses:
		// - D65 illuminant
		// - 2° standard observer
		// - Rec. 709 primaries
		
		// Implementation would create appropriate specification
		// icmm::CTristimulusSpecification* spec = new ...
		// spec->SetIlluminant(d65);
		// spec->SetObserverType(OT_CIE_1931_2_DEGREE);
		// return TristimulusPtr(spec);
		
		return nullptr;  // Simplified
	}

	// Example 3: Comparing specifications
	bool SpecificationsMatch(const icmm::ITristimulusSpecification* spec1,
	                        const icmm::ITristimulusSpecification* spec2)
	{
		if (!spec1 || !spec2) return false;
		
		// Compare illuminants
		auto illum1 = spec1->GetIlluminant();
		auto illum2 = spec2->GetIlluminant();
		
		if (illum1 && illum2) {
			if (illum1->GetIlluminantType() != illum2->GetIlluminantType()) {
				return false;
			}
		}
		
		// Compare observer types
		if (spec1->GetObserverType() != spec2->GetObserverType()) {
			return false;
		}
		
		return true;
	}

	// Example 4: Converting spectral to tristimulus
	void ConvertSpectralToXyz(const icmm::ITristimulusSpecification* spec)
	{
		// Get base spectral specification if available
		auto spectralSpec = spec->GetBaseSpecification();
		
		if (spectralSpec) {
			// Has underlying spectral data
			// Can use for high-accuracy conversions
			qDebug() << "Has spectral base - high accuracy";
		} else {
			// Direct tristimulus specification
			qDebug() << "Direct tristimulus - standard accuracy";
		}
	}

	// Example 5: Working with RGB color models
	void SetupRgbModel(const icmm::ITristimulusSpecification* spec)
	{
		// RGB models use tristimulus specifications
		icmm::CRgbColorModel rgbModel(*spec);
		
		// The model now uses the specified:
		// - Illuminant (white point)
		// - Observer (color matching functions)
		// - Method (calculation precision)
		
		// Enables accurate color conversions
		IColorSpecification::ConstColorSpecPtr modelSpec = 
			rgbModel.GetSpecification();
	}

	// Example 6: Checking specification type
	void ProcessColorSpec(const icmm::IColorSpecification* spec)
	{
		if (spec->GetSpecificationType() == 
		    IColorSpecification::SpecType::Tristimulus) 
		{
			// Cast to tristimulus specification
			const icmm::ITristimulusSpecification* tristimSpec = 
				dynamic_cast<const icmm::ITristimulusSpecification*>(spec);
			
			if (tristimSpec) {
				// Access tristimulus-specific properties
				auto illuminant = tristimSpec->GetIlluminant();
				auto observer = tristimSpec->GetObserverType();
			}
		}
	}
	\endcode

	\section TristimulusSpecStandards Standard Configurations
	**sRGB (web/display standard):**
	- Illuminant: D65
	- Observer: CIE 1931 2°
	- Primaries: Rec. 709
	
	**Adobe RGB (photography):**
	- Illuminant: D65
	- Observer: CIE 1931 2°
	- Wider gamut than sRGB
	
	**Print workflows:**
	- Illuminant: D50 (standard for printing)
	- Observer: CIE 1931 2°
	- ICC profile-based

	\section TristimulusSpecBestPractices Best Practices
	- Match illuminant to viewing conditions (D65 for displays, D50 for print)
	- Use 2° observer for most applications
	- Use 10° observer for large color patches or critical matching
	- Check for spectral base when accuracy is critical
	- Cache specifications for repeated conversions

	\sa icmm::IColorSpecification, icmm::IIlluminant, icmm::IWhitePointProvider,
	    icmm::ISpectralColorSpecification, icmm::CTristimulusSpecification

	\ingroup Color
*/
class ITristimulusSpecification: virtual public IColorSpecification
{
public:
	/**
		Gets the illuminant (light source) used by this specification.
		
		The illuminant defines the reference white point and viewing conditions.
		Common illuminants include D65 (daylight), D50 (print), and A (tungsten).
		
		\return Shared pointer to IIlluminant, or nullptr if not set
		
		\sa IIlluminant, IWhitePointProvider
	*/
	virtual std::shared_ptr<IIlluminant> GetIlluminant() const = 0;
	
	/**
		Gets the standard observer type used for color calculations.
		
		The observer type defines the color matching functions used:
		- OT_CIE_1931_2_DEGREE: Standard 2° observer (small fields)
		- OT_CIE_1964_10_DEGREE: Supplementary 10° observer (large fields)
		
		\return ObserverType indicating which standard observer to use
		
		\sa ObserverType
	*/
	virtual ObserverType GetObserverType() const = 0;
	
	/**
		Gets the ASTM table type used for spectral calculations.
		
		The method determines wavelength range and resolution for
		converting between spectral and tristimulus representations.
		
		\return AstmTableType indicating calculation method
		
		\sa AstmTableType
	*/
	virtual AstmTableType GetMethod() const = 0;

	/**
		Gets the base spectral specification, if one exists.
		
		Some tristimulus specifications are derived from spectral data,
		which provides higher accuracy for certain conversions. If available,
		the base spectral specification can be used for improved accuracy.
		
		\return Shared pointer to ISpectralColorSpecification, or nullptr if none
		
		\sa ISpectralColorSpecification
	*/
	virtual std::shared_ptr<ISpectralColorSpecification> GetBaseSpecification() const = 0;

protected:
	// reimplemented (IColorSpecification)
	virtual SpecType GetSpecificationType() const override;
};


inline IColorSpecification::SpecType ITristimulusSpecification::GetSpecificationType() const
{
	return SpecType::Tristimulus;
}

typedef std::shared_ptr<ITristimulusSpecification> TristimulusPtr;
typedef std::shared_ptr<const ITristimulusSpecification> ConstTristimulusPtr;

} // namespace icmm


