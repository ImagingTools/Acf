#pragma once


// ACF includes
#include <imath/CSampledFunction.h>
#include <iser/ISerializable.h>
#include <icmm/ISpectrum.h>
#include <icmm/ISpectrumInfo.h>


namespace icmm
{


/**
	Standard spectrum implementation for storing sampled spectral data.
	
	\section SpectrumPurpose Purpose
	CSpectrum provides a complete implementation for storing and manipulating spectral
	data - measurements of light intensity across different wavelengths. It combines
	sampled function capabilities with spectrum-specific information like wavelength
	range and step size. This is the concrete class for working with spectral color data.

	\section SpectrumStructure Data Structure
	CSpectrum stores:
	- **Wavelength range**: Min and max wavelengths (e.g., 380-780 nm for visible light)
	- **Sample values**: Intensity measurements at regular intervals
	- **Step size**: Wavelength spacing between samples
	- **Sample count**: Number of measurement points

	\section SpectrumUsage Usage Examples
	\code
	// Example 1: Creating a spectrum with explicit samples
	std::vector<double> samples = {
		0.0, 0.1, 0.3, 0.5, 0.8, 1.0, 0.8, 0.5, 0.3, 0.1, 0.0
	};
	
	// Visible light range with custom samples
	icmm::CSpectrum spectrum(380, 780, samples);
	
	// Query properties
	istd::CIntRange range = spectrum.GetSpectralRange();
	int step = spectrum.GetStep();           // Calculated from range and samples
	int count = spectrum.GetSamplesCount();  // 11

	// Example 2: Creating a spectrum with automatic initialization
	// Create spectrum covering 400-700nm with 10nm steps (31 samples)
	icmm::CSpectrum spectrum(400, 700, 10);
	
	// Set sample values manually
	int sampleCount = spectrum.GetSamplesCount();
	for (int i = 0; i < sampleCount; ++i) {
		double wavelength = 400 + i * 10;
		
		// Create peaked spectrum (e.g., laser at 550nm)
		double peak = 550.0;
		double width = 20.0;
		double intensity = exp(-pow((wavelength - peak) / width, 2.0));
		
		spectrum.SetSampleValue(i, intensity);
	}

	// Example 3: Getting interpolated values
	double GetIntensityAt(const icmm::CSpectrum& spectrum, double wavelength)
	{
		// Calculate() uses interpolation for wavelengths between samples
		double intensity;
		if (spectrum.Calculate(wavelength, intensity)) {
			return intensity;
		}
		return 0.0;
	}
	
	// Get intensity at 525nm (interpolated)
	double intensity525 = GetIntensityAt(spectrum, 525.0);

	// Example 4: Creating standard illuminants
	icmm::CSpectrum CreateD65Illuminant()
	{
		// D65 daylight illuminant (simplified)
		std::vector<double> d65Samples = {
			50.0, 52.3, 54.6, 68.7, 82.8, 87.1, 91.5, 92.5,
			93.4, 90.1, 86.7, 95.8, 104.9, 110.9, 117.0
			// ... more samples across visible range
		};
		
		return icmm::CSpectrum(380, 780, d65Samples);
	}

	// Example 5: Analyzing spectrum properties
	struct SpectrumStats {
		double peakWavelength;
		double peakIntensity;
		double totalEnergy;
	};
	
	SpectrumStats AnalyzeSpectrum(const icmm::CSpectrum& spectrum)
	{
		SpectrumStats stats;
		stats.peakIntensity = 0.0;
		stats.totalEnergy = 0.0;
		
		istd::CIntRange range = spectrum.GetSpectralRange();
		int step = spectrum.GetStep();
		int count = spectrum.GetSamplesCount();
		
		for (int i = 0; i < count; ++i) {
			double intensity = spectrum.GetSampleValue(i);
			double wavelength = range.GetMin() + i * step;
			
			// Find peak
			if (intensity > stats.peakIntensity) {
				stats.peakIntensity = intensity;
				stats.peakWavelength = wavelength;
			}
			
			// Integrate energy
			stats.totalEnergy += intensity * step;
		}
		
		return stats;
	}

	// Example 6: Saving and loading spectra
	void SaveSpectrum(const icmm::CSpectrum& spectrum, const QString& filename)
	{
		ifile::CFileWriteArchive archive(filename);
		if (archive.IsOpen()) {
			icmm::CSpectrum copy = spectrum;
			copy.Serialize(archive);
		}
	}
	
	void LoadSpectrum(icmm::CSpectrum& spectrum, const QString& filename)
	{
		ifile::CFileReadArchive archive(filename);
		if (archive.IsOpen()) {
			spectrum.Serialize(archive);
		}
	}

	// Example 7: Normalizing spectrum
	void NormalizeSpectrum(icmm::CSpectrum& spectrum)
	{
		// Find maximum intensity
		double maxIntensity = 0.0;
		int count = spectrum.GetSamplesCount();
		
		for (int i = 0; i < count; ++i) {
			double intensity = spectrum.GetSampleValue(i);
			if (intensity > maxIntensity) {
				maxIntensity = intensity;
			}
		}
		
		// Normalize to [0, 1]
		if (maxIntensity > 0.0) {
			for (int i = 0; i < count; ++i) {
				double intensity = spectrum.GetSampleValue(i);
				spectrum.SetSampleValue(i, intensity / maxIntensity);
			}
		}
	}

	// Example 8: Combining spectra (e.g., light filtering)
	icmm::CSpectrum MultiplySpectra(const icmm::CSpectrum& spectrum1,
	                               const icmm::CSpectrum& spectrum2)
	{
		// Assumes same wavelength range and sampling
		icmm::CSpectrum result = spectrum1;
		
		int count = result.GetSamplesCount();
		for (int i = 0; i < count; ++i) {
			double value1 = spectrum1.GetSampleValue(i);
			double value2 = spectrum2.GetSampleValue(i);
			result.SetSampleValue(i, value1 * value2);
		}
		
		return result;
	}
	\endcode

	\section SpectrumApplications Applications
	- **Color science**: Converting spectra to tristimulus values (XYZ)
	- **Lighting**: Characterizing light sources (LEDs, sunlight, etc.)
	- **Materials**: Measuring reflectance, transmittance, emission spectra
	- **Spectroscopy**: Analyzing chemical composition
	- **Rendering**: Physics-based light transport
	- **Photography**: Camera spectral sensitivity

	\section SpectrumRanges Common Wavelength Ranges
	- **Visible light**: 380-780 nm
	- **Extended visible**: 360-830 nm
	- **UV-Visible-IR**: 300-1000 nm
	- **Typical step**: 1, 5, or 10 nm

	\section SpectrumBestPractices Best Practices
	- Use consistent wavelength ranges within a project
	- Choose step size based on required accuracy (smaller = more accurate, larger = more efficient)
	- Normalize spectra for comparison
	- Use interpolation (Calculate method) for wavelengths between samples
	- Store spectra in appropriate units (absolute or relative)
	- Consider using standard illuminants (D65, A, F2) for consistency

	\sa icmm::ISpectrum, icmm::ISpectrumInfo, imath::CSampledFunction,
	    icmm::CSpectralColor, icmm::ISpectrumInfoProvider

	\ingroup Color
*/
class CSpectrum:
			public imath::CSampledFunction,
			virtual public ISpectrum,
			virtual protected ISpectrumInfo,
			virtual public iser::ISerializable
{
public:
	typedef imath::CSampledFunction BaseClass;

	CSpectrum();
	CSpectrum(const CSpectrum& other);

	/**
		Initialize with given samples
		First sample is at the start of the range, last sample is at the end
	*/
	CSpectrum(int startWavelength, int endWavelength, const std::vector<double>& spectrumSamples);
	CSpectrum(const istd::CIntRange& wavelengthRange, const std::vector<double>& spectrumSamples);

	/**
		Zero initialize samples to fill range with given step
	 */
	CSpectrum(int startWavelength, int endWavelength, int step);
	CSpectrum(const istd::CIntRange& wavelengthRange, int step);

	// reimplemented (icmm::ISpectrumInfoProvider)
	virtual const ISpectrumInfo* GetSpectrumInfo() const override;

	// reimplemented (icmm::ISpectrumInfo)
	virtual istd::CIntRange GetSpectralRange() const override;
	virtual int GetStep() const override;
	virtual int GetSamplesCount() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

};


} // namespace icmm


