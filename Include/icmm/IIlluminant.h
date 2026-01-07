#pragma once


// ACF includes
#include <icmm/IWhitePointProvider.h>


namespace icmm
{


/**
	Interface for illuminant objects with configurable properties.
	
	\section IlluminantPurpose Purpose
	IIlluminant extends IWhitePointProvider to represent a complete light source
	specification. It combines white point information with additional metadata
	like illuminant name and type, enabling proper color adaptation and device-independent
	color management.

	\section IlluminantConcept Illuminant Concept
	An illuminant represents:
	- **Light source**: The spectral power distribution of light
	- **White point**: The XYZ coordinates considered as "white"
	- **Standard type**: Predefined standard (D65, D50, A, etc.)
	- **Name**: Human-readable identifier

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

	\section IlluminantUsage Usage Examples
	\code
	// Example 1: Creating and configuring an illuminant
	icmm::CIlluminant* CreateD65Illuminant()
	{
		icmm::CIlluminant* illuminant = new icmm::CIlluminant();
		
		// Set standard type
		illuminant->SetIlluminantType(icmm::StandardIlluminant::SI_D65);
		illuminant->SetIlluminantName("D65");
		
		// Set white point (D65 XYZ coordinates)
		icmm::CVarColor whitePoint(3);
		whitePoint.SetElement(0, 0.95047);  // X
		whitePoint.SetElement(1, 1.00000);  // Y (normalized to 1.0)
		whitePoint.SetElement(2, 1.08883);  // Z
		illuminant->SetWhitePoint(whitePoint);
		
		return illuminant;
	}

	// Example 2: Querying illuminant properties
	void InspectIlluminant(const icmm::IIlluminant* illuminant)
	{
		// Get type and name
		icmm::StandardIlluminant type = illuminant->GetIlluminantType();
		QString name = illuminant->GetIlluminantName();
		
		qDebug() << "Illuminant:" << name;
		
		// Get white point
		icmm::CVarColor wp = illuminant->GetWhitePoint();
		if (wp.GetElementsCount() >= 3) {
			qDebug() << "White Point XYZ:" 
			         << wp.GetElement(0) << wp.GetElement(1) << wp.GetElement(2);
		}
	}

	// Example 3: Chromatic adaptation between illuminants
	icmm::CVarColor AdaptColor(const icmm::CVarColor& colorXyz,
	                          const icmm::IIlluminant* sourceIlluminant,
	                          const icmm::IIlluminant* destIlluminant)
	{
		// Get white points
		icmm::CVarColor sourceWP = sourceIlluminant->GetWhitePoint();
		icmm::CVarColor destWP = destIlluminant->GetWhitePoint();
		
		// Apply chromatic adaptation transform
		// (Bradford, von Kries, etc.)
		// This ensures colors look consistent under different lighting
		
		icmm::CVarColor adaptedColor = colorXyz;
		// ... adaptation calculation ...
		
		return adaptedColor;
	}

	// Example 4: Display vs. print illuminants
	void SetupColorManagement()
	{
		// Display workflow (D65)
		icmm::CIlluminant displayIlluminant;
		displayIlluminant.SetIlluminantType(icmm::StandardIlluminant::SI_D65);
		displayIlluminant.SetIlluminantName("D65");
		
		// Print workflow (D50)
		icmm::CIlluminant printIlluminant;
		printIlluminant.SetIlluminantType(icmm::StandardIlluminant::SI_D50);
		printIlluminant.SetIlluminantName("D50");
		
		// Colors need adaptation when going from display to print
		bool needsAdaptation = 
			(displayIlluminant.GetIlluminantType() != 
			 printIlluminant.GetIlluminantType());
	}

	// Example 5: Working with tristimulus specifications
	void CreateColorSpecification(icmm::IIlluminant* illuminant)
	{
		// Illuminants are used in tristimulus specifications
		// to define the viewing conditions for color spaces
		
		// Create RGB specification with illuminant
		// icmm::CTristimulusSpecification spec;
		// spec.SetIlluminant(std::shared_ptr<IIlluminant>(illuminant));
		
		// This specification can then be used with RGB color models
		// to ensure accurate, device-independent color
	}

	// Example 6: Serialization for persistence
	void SaveIlluminant(const icmm::IIlluminant* illuminant,
	                   const QString& filename)
	{
		ifile::CFileWriteArchive archive(filename);
		if (archive.IsOpen()) {
			// IIlluminant is ISerializable
			icmm::IIlluminant* copy = /* clone illuminant */;
			copy->Serialize(archive);
		}
	}

	// Example 7: Illuminant selection helper
	icmm::StandardIlluminant SelectIlluminant(const QString& purpose)
	{
		if (purpose == "display" || purpose == "web") {
			return icmm::StandardIlluminant::SI_D65;
		} else if (purpose == "print") {
			return icmm::StandardIlluminant::SI_D50;
		} else if (purpose == "photography") {
			return icmm::StandardIlluminant::SI_D65;  // or D50 for some workflows
		} else if (purpose == "tungsten") {
			return icmm::StandardIlluminant::SI_A;
		}
		
		return icmm::StandardIlluminant::SI_D65;  // Default
	}
	\endcode

	\section IlluminantApplications Applications
	- **Color space definitions**: Defining RGB primaries and white point
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

	\note White point Y component is typically normalized to 1.0

	\sa icmm::IWhitePointProvider, icmm::ITristimulusSpecification,
	    icmm::CIlluminant, icmm::StandardIlluminant

	\ingroup Color
*/
class IIlluminant: virtual public IWhitePointProvider, virtual public iser::ISerializable
{
public:
	/**
		Sets the white point for this illuminant.
		
		The white point is typically expressed as XYZ tristimulus values
		with Y component normalized to 1.0.
		
		\param whitePoint CVarColor containing XYZ coordinates of white point
		
		\sa GetWhitePoint(), IWhitePointProvider
	*/
	virtual void SetWhitePoint(const icmm::CVarColor& whitePoint) = 0;
	
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
		
		\return StandardIlluminant enum value (SI_D65, SI_D50, SI_A, etc.)
		
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


