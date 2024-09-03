#include <icmm/CTristimulusSpecification.h>


namespace icmm
{


CTristimulusSpecification::CTristimulusSpecification(
			ObserverType observerType,
			AstmTableType method,
			const IIlluminant* illuminantPtr,
			std::shared_ptr<ISpectralColorSpecification> baseSpec)
	:m_observerType(observerType),
	m_method(method)
{
	m_baseSpec = baseSpec;

	if (illuminantPtr != nullptr){
		m_illuminant.CopyFrom(*illuminantPtr);
	}
}


CTristimulusSpecification::CTristimulusSpecification(const CTristimulusSpecification& other)
	:m_illuminant(other.m_illuminant),
	m_method(other.m_method),
	m_observerType(other.m_observerType)
{
}


// reimplemented (ITristimulusSpecification)

const IIlluminant& icmm::CTristimulusSpecification::GetIlluminant() const
{
	return m_illuminant;
}


ObserverType CTristimulusSpecification::GetObserverType() const
{
	return m_observerType;
}


AstmTableType CTristimulusSpecification::GetMethod() const
{
	return m_method;
}


std::shared_ptr<ISpectralColorSpecification> CTristimulusSpecification::GetBaseSpecification() const
{
	return m_baseSpec;
}


} // namespace icmm


