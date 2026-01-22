// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <imath/TISampledFunction.h>
#include <icmm/ISpectrumInfoProvider.h>
#include <iser/ISerializable.h>


namespace icmm
{


/**
	Common interface for a measured or sampled spectrum.
	
	\section SpectrumPurpose Purpose
	ISpectrum represents spectral data - measurements of light intensity or energy
	across different wavelengths. This is commonly used in color science, spectroscopy,
	lighting simulation, and material analysis. The spectrum is stored as discrete
	samples in a regular 1D grid across the wavelength range.

	\section SpectrumStructure Data Structure
	A spectrum combines three interfaces to provide complete functionality:
	
	- **ISampledFunction**: Provides access to the discrete sample values stored in
	  a regular grid. Use this to get/set individual sample values by index.
	
	- **ISpectrumInfoProvider**: Provides metadata about the spectral range (min/max
	  wavelength) and sampling resolution. Use this to understand what the samples represent.
	
	- **ISerializable**: Enables saving and loading spectrum data to/from archives
	  (files, memory, network, etc.).

	\section SpectrumDomain Domain and Range
	- **Domain (Wavelength)**: Typically measured in nanometers (nm). The spectral
	  range (e.g., 380-780 nm for visible light) is obtained via ISpectralInfo.
	  
	- **Range (Intensity)**: Sample values are typically normalized to [0, 1] where
	  0 represents no energy and 1 represents maximum energy. The actual range can
	  be queried using GetResultValueRange() from TISampledFunction.

	\section SpectrumUsageExample Usage Examples
	\code
	// Example 1: Creating and populating a spectrum
	CSpectrum* CreateRedSpectrum()
	{
		// Create spectrum for visible range (380-780 nm) with 81 samples
		CSpectrum* spectrum = new CSpectrum(380.0, 780.0, 81);
		
		// Set sample values - higher in red region (600-700 nm)
		int sampleCount = spectrum->GetSamplesCount();
		for (int i = 0; i < sampleCount; ++i) {
			double wavelength = spectrum->GetWavelengthForIndex(i);
			
			// Simple red spectrum: high intensity in red region
			double intensity = 0.0;
			if (wavelength >= 600.0 && wavelength <= 700.0) {
				intensity = 1.0;
			} else if (wavelength >= 550.0 && wavelength < 600.0) {
				// Gradual transition from yellow to red
				intensity = (wavelength - 550.0) / 50.0;
			}
			
			spectrum->SetSampleValue(i, intensity);
		}
		
		return spectrum;
	}

	// Example 2: Reading spectrum data
	void AnalyzeSpectrum(const icmm::ISpectrum* spectrum)
	{
		// Get spectral range information
		const ISpectralInfo& info = spectrum->GetSpectralInfo();
		double minWavelength = info.GetMinWavelength();  // e.g., 380 nm
		double maxWavelength = info.GetMaxWavelength();  // e.g., 780 nm
		
		// Get sample count
		int sampleCount = spectrum->GetSamplesCount();
		qDebug() << "Spectrum has" << sampleCount << "samples";
		qDebug() << "Range:" << minWavelength << "-" << maxWavelength << "nm";
		
		// Find peak wavelength
		double peakIntensity = 0.0;
		int peakIndex = 0;
		for (int i = 0; i < sampleCount; ++i) {
			double intensity = spectrum->GetSampleValue(i);
			if (intensity > peakIntensity) {
				peakIntensity = intensity;
				peakIndex = i;
			}
		}
		
		// Convert peak index to wavelength
		double peakWavelength = minWavelength + 
			(peakIndex * (maxWavelength - minWavelength) / (sampleCount - 1));
		qDebug() << "Peak at" << peakWavelength << "nm";
	}

	// Example 3: Saving/Loading spectra
	void SaveSpectrum(const icmm::ISpectrum* spectrum, const QString& filename)
	{
		ifile::CFileWriteArchive archive(filename);
		if (archive.IsOpen()) {
			const_cast<icmm::ISpectrum*>(spectrum)->Serialize(archive);
		}
	}

	void LoadSpectrum(icmm::ISpectrum* spectrum, const QString& filename)
	{
		ifile::CFileReadArchive archive(filename);
		if (archive.IsOpen()) {
			spectrum->Serialize(archive);
		}
	}

	// Example 4: Interpolating spectral values
	double GetSpectralValueAt(const icmm::ISpectrum* spectrum, double wavelength)
	{
		// Use IMathFunction interface for interpolated values
		const imath::IMathFunction* function = 
			dynamic_cast<const imath::IMathFunction*>(spectrum);
		
		if (function) {
			double value;
			if (function->Calculate(wavelength, value)) {
				return value;
			}
		}
		return 0.0;
	}
	\endcode

	\section SpectrumApplications Common Applications
	- **Color Science**: Converting spectra to RGB or XYZ color values
	- **Material Properties**: Storing reflectance, transmittance, or emission spectra
	- **Light Sources**: Characterizing illuminants (daylight, LED, incandescent)
	- **Spectroscopy**: Storing measurement data from spectrophotometers
	- **Rendering**: Physics-based light transport in computer graphics

	\sa icmm::ISpectrumInfoProvider, imath::ISampledFunction, imath::IMathFunction,
	    icmm::CSpectrum, iser::ISerializable

	\ingroup Color
*/
class ISpectrum:
			virtual public imath::ISampledFunction,
			virtual public ISpectrumInfoProvider,
			virtual public iser::ISerializable
{
};


} // namespace icmm


