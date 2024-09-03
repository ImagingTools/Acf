#pragma once


// ACF includes
#include <icmm/ITristimulusSpecification.h>
#include <icmm/ISpectralColorSpecification.h>
#include <icmm/CIlluminant.h>


namespace icmm
{


class CTristimulusSpecification: virtual public ITristimulusSpecification
{
public:
	CTristimulusSpecification(
				ObserverType observerType = ObserverType::TwoDegree,
				AstmTableType method = AstmTableType::Unknown,
				const IIlluminant* illuminantPtr = nullptr,
				std::shared_ptr<ISpectralColorSpecification> baseSpec = std::shared_ptr<ISpectralColorSpecification>());

	CTristimulusSpecification(const CTristimulusSpecification& other);

	// reimplemented (ITristimulusSpecification)
	virtual const IIlluminant& GetIlluminant() const override;
	virtual ObserverType GetObserverType() const override;
	virtual AstmTableType GetMethod() const override;
	virtual std::shared_ptr<ISpectralColorSpecification> GetBaseSpecification() const override;

private:
	ObserverType m_observerType = ObserverType::TwoDegree;
	AstmTableType m_method = AstmTableType::Unknown;
	CIlluminant m_illuminant;
	std::shared_ptr<ISpectralColorSpecification> m_baseSpec;
};


} // namespace icmm


