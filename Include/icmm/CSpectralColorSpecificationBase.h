// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <icmm/ISpectralColorSpecification.h>
#include <icmm/CSpectrumInfo.h>


namespace icmm
{


class CSpectralColorSpecificationBase: virtual public ISpectralColorSpecification
{
public:
	CSpectralColorSpecificationBase(const CSpectralColorSpecificationBase& spec);

	// reimplemented (ISpectrumInfoProvider)
	virtual const ISpectrumInfo* GetSpectrumInfo() const override;
	virtual SpectrumType GetSpectrumType() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const istd::IChangeable& other) const override;

protected:
	CSpectralColorSpecificationBase(const ISpectrumInfo& spectrumInfo);
	CSpectralColorSpecificationBase(istd::CIntRange range, int step);

protected:
	SpectrumType m_spectrumType;
	CSpectrumInfo m_info;
};


} // namespace icmm


