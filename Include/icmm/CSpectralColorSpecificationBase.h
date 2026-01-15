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
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

protected:
	CSpectralColorSpecificationBase(const ISpectrumInfo& spectrumInfo);
	CSpectralColorSpecificationBase(istd::CIntRange range, int step);

	SpectrumType m_spectrumType;

private:
	CSpectrumInfo m_info;
};


} // namespace icmm


