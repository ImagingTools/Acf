// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iser/IObject.h>
#include <icmm/icmm.h>
#include <icmm/CSpectrum.h>


namespace icmm
{


/**
	Interface for illuminant objects with configurable properties.

	\section IlluminantPurpose Purpose
	IIlluminant represents a light source specification. It combines a standard illuminant type,
	a human readable name and optionally the spectral power distribution of the source, enabling
	proper color adaptation and device-independent color management.

	\section IlluminantConcept Illuminant Concept
	An illuminant represents:
	- **Light source**: The spectral power distribution of light
	- **Standard type**: Predefined standard (D65, D50, A, etc.)
	- **Name**: Human-readable identifier

	An illuminant deliberately provides **no white point**. Tristimulus values of a light source
	are the integral of its spectral power distribution against the color matching functions of an
	observer, so the same illuminant yields different white points for the 2 degree and the
	10 degree observer. The white point is therefore defined on the level of
	icmm::ITristimulusSpecification, where illuminant, observer and calculation method are known.

	\section IlluminantStandardTypes Standard Illuminant Types
	**Daylight illuminants:**
	- **D65**: 6504K daylight - standard for displays (sRGB, Rec. 709)
	- **D50**: 5003K daylight - standard for printing (ICC profiles)
	- **D55**: 5503K mid-morning daylight
	- **D75**: 7504K north sky daylight

	**Incandescent:**
	- **A**: 2856K tungsten lamp - household lighting

	**Fluorescent:**
	- **F2**: Cool white fluorescent - office lighting
	- **F7**: Broad-band daylight fluorescent
	- **F11**: Narrow-band white fluorescent

	**Special:**
	- **E**: Equal energy - theoretical reference
	- **C**: Average daylight (obsolete, replaced by D series)
	- **Custom**: Defined by an explicit spectral power distribution

	\section IlluminantUsage Usage Examples
	\code
	// Example 1: Creating a standard illuminant
	icmm::CIlluminant CreateD65Illuminant()
	{
		// Standard illuminants are fully described by their type
		return icmm::CIlluminant(icmm::StandardIlluminant::D65);
	}

	// Example 2: Creating a custom illuminant from measured data
	icmm::CIlluminant CreateCustomIlluminant(const std::vector<double>& spdSamples)
	{
		// Relative radiant power sampled from 380nm to 730nm
		icmm::CSpectrum spd(380, 730, spdSamples);

		return icmm::CIlluminant("My Light Booth", spd);
	}

	// Example 3: Querying illuminant properties
	void InspectIlluminant(const icmm::IIlluminant* illuminant)
	{
		// Get type and name
		icmm::StandardIlluminant type = illuminant->GetIlluminantType();
		QString name = illuminant->GetIlluminantName();

		qDebug() << "Illuminant:" << name;

		// Get spectral power distribution, it may be empty for standard types
		const icmm::CSpectrum& spd = illuminant->GetSpectralPowerDistribution();
		if (spd.GetSamplesCount() > 0) {
			istd::CIntRange range = spd.GetSpectralRange();

			qDebug() << "SPD:" << range.GetMinValue() << "-" << range.GetMaxValue()
			         << "nm, step" << spd.GetStep();
		}
	}

	// Example 4: Display vs. print illuminants
	void SetupColorManagement()
	{
		// Display workflow (D65)
		icmm::CIlluminant displayIlluminant(icmm::StandardIlluminant::D65);

		// Print workflow (D50)
		icmm::CIlluminant printIlluminant(icmm::StandardIlluminant::D50);

		// Colors need adaptation when going from display to print
		bool needsAdaptation =
			(displayIlluminant.GetIlluminantType() !=
			 printIlluminant.GetIlluminantType());
	}

	// Example 5: Working with tristimulus specifications
	void CreateColorSpecification()
	{
		// Illuminants are used in tristimulus specifications together with an observer
		// to define the viewing conditions of a color space. Only this combination
		// determines a white point.
		icmm::CTristimulusSpecification spec(
			icmm::ObserverType::TwoDegree,
			std::make_shared<icmm::CIlluminant>(icmm::StandardIlluminant::D50),
			icmm::AstmTableType::E308Table5);

		// This specification can then be used with Lab or RGB color models
		// to ensure accurate, device-independent color
	}

	// Example 6: Illuminant selection helper
	icmm::StandardIlluminant SelectIlluminant(const QString& purpose)
	{
		if (purpose == "display" || purpose == "web") {
			return icmm::StandardIlluminant::D65;
		} else if (purpose == "print") {
			return icmm::StandardIlluminant::D50;
		} else if (purpose == "photography") {
			return icmm::StandardIlluminant::D65;  // or D50 for some workflows
		} else if (purpose == "tungsten") {
			return icmm::StandardIlluminant::A;
		}

		return icmm::StandardIlluminant::D65;  // Default
	}
	\endcode

	\section IlluminantApplications Applications
	- **Color space definitions**: Defining RGB primaries and viewing conditions
	- **Chromatic adaptation**: Converting colors between viewing conditions
	- **Color appearance**: Modeling how colors appear under different lights
	- **ICC profiles**: Defining profile connection space (D50)
	- **Display calibration**: Matching display white to standard
	- **Photography**: Simulating different lighting conditions

	\section IlluminantBestPractices Best Practices
	- Use D65 for display/screen-based workflows
	- Use D50 for print workflows and ICC profiles
	- Match illuminant to actual viewing conditions when possible
	- Store illuminant with color data for reproducibility
	- Apply chromatic adaptation when illuminants don't match
	- Use standard illuminants rather than custom when possible
	- Supply a spectral power distribution only for custom light sources

	\note A spectral power distribution is relative radiant power per wavelength. It is neither a
	      reflectance nor normalized to [0, 1], by CIE convention it is scaled so that the value at
	      560nm is 100.

	\sa icmm::CIlluminant, icmm::CSpectrum, icmm::ITristimulusSpecification,
	    icmm::StandardIlluminant

	\ingroup Color
*/
class IIlluminant: virtual public iser::IObject
{
public:
	/**
		Gets the spectral power distribution of this illuminant.

		\return Relative radiant power per wavelength, empty if only the standard type is known.

		\sa SetSpectralPowerDistribution(), icmm::CSpectrum
	*/
	virtual const icmm::CSpectrum& GetSpectralPowerDistribution() const = 0;

	/**
		Sets the spectral power distribution of this illuminant.

		\param spectrum Relative radiant power per wavelength.

		\sa GetSpectralPowerDistribution()
	*/
	virtual void SetSpectralPowerDistribution(const icmm::CSpectrum& spectrum) = 0;

	/**
		Gets the human-readable name of this illuminant.

		\return QString containing illuminant name (e.g., "D65", "D50", "A")

		\sa SetIlluminantName()
	*/
	virtual QString GetIlluminantName() const = 0;

	/**
		Sets the human-readable name for this illuminant.

		\param illuminantName QString with illuminant name

		\sa GetIlluminantName()
	*/
	virtual void SetIlluminantName(const QString& illuminantName) = 0;

	/**
		Gets the standard illuminant type.

		\return StandardIlluminant enum value (D65, D50, A, etc.)

		\sa SetIlluminantType(), StandardIlluminant
	*/
	virtual StandardIlluminant GetIlluminantType() const = 0;

	/**
		Sets the standard illuminant type.

		\param illuminantType StandardIlluminant enum value

		\sa GetIlluminantType(), StandardIlluminant
	*/
	virtual void SetIlluminantType(const StandardIlluminant& illuminantType) = 0;
};


} // namespace icmm
