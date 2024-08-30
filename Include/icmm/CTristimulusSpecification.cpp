#include <icmm/CTristimulusSpecification.h>


namespace icmm
{


CTristimulusSpecification::CTristimulusSpecification(
			ObserverType observerType,
			AstmTableType method,
			const CIlluminant& illuminant)
	:m_observerType(observerType),
	m_method(method),
	m_illuminant(illuminant)
{
}


CTristimulusSpecification::CTristimulusSpecification(const CTristimulusSpecification& other)
	:m_illuminant(other.m_illuminant),
	m_method(other.m_method),
	m_observerType(other.m_observerType)
{
}


// reimplemented (ITristimulusSpecification)

const IIluminant& icmm::CTristimulusSpecification::GetIlluminant() const
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


} // namespace icmm


