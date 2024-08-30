#pragma once


// ACF includes
#include <icmm/ITristimulusSpecification.h>
#include <icmm/THierarchicalSpecificationWrap.h>
#include <icmm/CIlluminant.h>


namespace icmm
{


class CTristimulusSpecification: public THierarchicalSpecificationWrap<ITristimulusSpecification>
{
public:
	CTristimulusSpecification(
				ObserverType observerType = ObserverType::TwoDegree,
				AstmTableType method = AstmTableType::Unknown,
				const CIlluminant& illuminant = CIlluminant());

	CTristimulusSpecification(const CTristimulusSpecification& other);

	// reimplemented (ITristimulusSpecification)
	virtual const IIluminant& GetIlluminant() const override;
	virtual ObserverType GetObserverType() const override;
	virtual AstmTableType GetMethod() const override;

private:
	ObserverType m_observerType = ObserverType::TwoDegree;
	AstmTableType m_method = AstmTableType::Unknown;
	CIlluminant m_illuminant;
};


} // namespace icmm


