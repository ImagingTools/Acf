#pragma once


// ACF includes
#include <iimgprint/iimgprint.h>
#include <icmm/ISpectralColorSpecification.h>
#include <icmm/CVarColor.h>

// Qt includes
#include <QSharedPointer>
#include <QMap>


namespace iimgprint
{


/**
	Represents a printer's color characteristics based on spectral measurements.
	
	\section PrinterProfilePurpose Purpose
	CPrinterProfile stores and manages the color reproduction characteristics of a
	specific printer. It is built from spectral measurements of a test chart printed
	on that printer, capturing how the printer reproduces different input colors.
	
	\section PrinterProfileData Profile Data
	The profile contains:
	- **Test Chart Data**: Spectral reflectance measurements for each patch
	- **Color Space Info**: The printer's native color space (RGB, CMYK, etc.)
	- **Gamut Information**: The range of colors the printer can reproduce
	- **Calibration State**: Whether the profile is current or needs updating
	
	\section PrinterProfileUsage Usage Examples
	\code
	// Example 1: Create profile from spectral measurements
	void CreateProfileFromMeasurements()
	{
		// Load spectral data from test chart measurements
		// (This would come from a spectrophotometer or similar device)
		icmm::ISpectralColorSpecification::ConstColorSpecPtr spectralData = 
			LoadSpectralTestChartData("printer1_testchart.cxf");
		
		// Create printer profile
		iimgprint::CPrinterProfile profile(
			spectralData,
			iimgprint::PrinterColorSpace::CMYK,
			"Canon ImagePROGRAF PRO-1000"
		);
		
		// Save profile for later use
		profile.SaveToFile("printer1_profile.xml");
	}
	
	// Example 2: Load existing profile
	void LoadAndUseProfile()
	{
		iimgprint::CPrinterProfile profile;
		if (profile.LoadFromFile("printer1_profile.xml")) {
			qDebug() << "Printer:" << profile.GetPrinterName();
			qDebug() << "Color space:" << (int)profile.GetColorSpace();
			qDebug() << "Gamut coverage:" << profile.GetGamutCoverage();
		}
	}
	
	// Example 3: Query profile information
	void InspectProfile(const iimgprint::CPrinterProfile& profile)
	{
		// Get basic info
		QString name = profile.GetPrinterName();
		PrinterColorSpace space = profile.GetColorSpace();
		
		// Check if profile has spectral data
		if (profile.HasSpectralData()) {
			auto spectralSpec = profile.GetSpectralSpecification();
			// Use spectral data for high-accuracy transformations
		}
		
		// Check gamut coverage
		double coverage = profile.GetGamutCoverage();
		if (coverage < 0.8) {
			qWarning() << "Limited gamut coverage - may need recalibration";
		}
	}
	\endcode
	
	\section PrinterProfileBestPractices Best Practices
	- Update profiles regularly (monthly for production printers)
	- Use standardized test charts (e.g., IT8.7/4, ECI2002)
	- Measure under consistent illumination conditions
	- Store multiple profiles for different paper types
	- Include metadata (date, substrate, conditions) with profiles
	
	\sa iimgprint::IPrinterColorTransformation, icmm::ISpectralColorSpecification,
	    iimgprint::CPrinterColorTransformation
	
	\ingroup Color
	\ingroup ImageProcessing
*/
class CPrinterProfile
{
public:
	/**
		Default constructor - creates an empty profile.
	*/
	CPrinterProfile();
	
	/**
		Constructs a printer profile from spectral measurements.
		
		\param spectralData Spectral measurements from a test chart
		\param colorSpace The printer's native color space
		\param printerName Optional human-readable printer identifier
	*/
	CPrinterProfile(
		icmm::ISpectralColorSpecification::ConstColorSpecPtr spectralData,
		PrinterColorSpace colorSpace,
		const QString& printerName = QString());
	
	/**
		Copy constructor.
	*/
	CPrinterProfile(const CPrinterProfile& other);
	
	/**
		Assignment operator.
	*/
	CPrinterProfile& operator=(const CPrinterProfile& other);
	
	/**
		Destructor.
	*/
	~CPrinterProfile();
	
	/**
		Gets the printer's native color space.
		
		\return The color space (RGB, CMYK, ExtendedGamut, etc.)
	*/
	PrinterColorSpace GetColorSpace() const;
	
	/**
		Sets the printer's color space.
		
		\param colorSpace The color space to set
	*/
	void SetColorSpace(PrinterColorSpace colorSpace);
	
	/**
		Gets the printer name/identifier.
		
		\return Human-readable printer name
	*/
	QString GetPrinterName() const;
	
	/**
		Sets the printer name/identifier.
		
		\param name Printer name to set
	*/
	void SetPrinterName(const QString& name);
	
	/**
		Checks if the profile contains spectral data.
		
		\return True if spectral measurements are available
	*/
	bool HasSpectralData() const;
	
	/**
		Gets the spectral color specification.
		
		\return Shared pointer to spectral data, or null if not available
	*/
	icmm::ISpectralColorSpecification::ConstColorSpecPtr GetSpectralSpecification() const;
	
	/**
		Sets the spectral color specification from test chart measurements.
		
		\param spectralData Spectral measurements to store
	*/
	void SetSpectralSpecification(icmm::ISpectralColorSpecification::ConstColorSpecPtr spectralData);
	
	/**
		Gets an estimate of the printer's gamut coverage.
		
		Returns a value between 0.0 and 1.0 indicating the gamut size
		relative to a reference color space (typically sRGB or Adobe RGB).
		
		\return Gamut coverage ratio (0.0 to 1.0)
	*/
	double GetGamutCoverage() const;
	
	/**
		Checks if the profile is valid and can be used.
		
		\return True if profile has required data
	*/
	bool IsValid() const;
	
	/**
		Saves the profile to a file.
		
		\param filePath Path to save the profile
		\return True if saved successfully
	*/
	bool SaveToFile(const QString& filePath) const;
	
	/**
		Loads the profile from a file.
		
		\param filePath Path to load the profile from
		\return True if loaded successfully
	*/
	bool LoadFromFile(const QString& filePath);

private:
	class Impl;
	Impl* m_impl;
};


} // namespace iimgprint


