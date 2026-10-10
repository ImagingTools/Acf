// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iser/ISerializable.h>
#include <icmm/IColorSpecification.h>
#include <icmm/ISpectrumInfoProvider.h>


namespace icmm
{


class ISpectralColorSpecification:
			virtual public IColorSpecification,
			virtual public ISpectrumInfoProvider
{
public:
	typedef std::shared_ptr<const ISpectralColorSpecification> ConstSpectralColorSpecPtr;
	typedef std::shared_ptr<ISpectralColorSpecification> SpectralColorSpecPtr;

	/**
		Physical description of the spectral data, based on CxF3.
	*/
	enum SpectrumType
	{
		/**
			The source does not say what was measured.
		*/
		NotSet = 0,
		/**
			Note that reflectance data values are scaled such that 100%=1.0.
		*/
		Reflective,
		Emissive,
		/**
			Direct (aka Regular) transmittance - The amount of light transmitted directly through a material
			in a parallel manner, ignoring light that is "diffused" within the material. Measurement in a typical
			sphere instrument is made by placing the material in the transmission compartment with the material
			mounted against the back wall (adjacent to the lens).
		*/
		Transmissive,
		/**
			Total transmittance - The total amount of light transmitted through a material including both direct
			and diffused light. Measurement in a typical sphere instrument is made by placing the material in the
			transmission compartment with the material mounted against the sphere opening (front wall) and away
			from the lens.
		*/
		TotalTransmissive
	};
	I_DECLARE_ENUM(SpectrumType, NotSet, Reflective, Emissive, Transmissive, TotalTransmissive);

	virtual SpectrumType GetSpectrumType() const = 0;

protected:
	// reimplemented (IColorSpecification)
	virtual SpecType GetSpecificationType() const final;
};


inline IColorSpecification::SpecType ISpectralColorSpecification::GetSpecificationType() const
{
	return SpecType::Spectral;
}


} // namespace icmm


