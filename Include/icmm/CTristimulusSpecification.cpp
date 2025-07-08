#include <icmm/CTristimulusSpecification.h>


namespace icmm
{


CTristimulusSpecification::CTristimulusSpecification(
			ObserverType observerType,
			AstmTableType method,
			std::shared_ptr<IIlluminant> illuminantPtr,
			std::shared_ptr<ISpectralColorSpecification> baseSpec)
:	m_observerType(observerType),
	m_method(method),
	m_illuminant(illuminantPtr),
	m_baseSpec(baseSpec)
{
}


CTristimulusSpecification::CTristimulusSpecification(const ITristimulusSpecification& other)
:	m_observerType(other.GetObserverType()),
	m_method(other.GetMethod()),
	m_illuminant(other.GetIlluminant()),
	m_baseSpec(other.GetBaseSpecification())
{
}


// reimplemented (ITristimulusSpecification)

std::shared_ptr<IIlluminant> icmm::CTristimulusSpecification::GetIlluminant() const
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


bool CTristimulusSpecification::IsEqual(const IChangeable& other) const
{
	const CTristimulusSpecification* objectPtr = dynamic_cast<const CTristimulusSpecification*>(&other);

	if (objectPtr == nullptr){
		return false;
	}

	return *this == *objectPtr;
}


bool CTristimulusSpecification::operator==(const CTristimulusSpecification& other) const
{
	return m_observerType == other.m_observerType
		&& m_method == other.m_method
		&& istd::AreObjectsEqual(m_illuminant.get(), other.m_illuminant.get())
		&& istd::AreObjectsEqual(m_baseSpec.get(), other.m_baseSpec.get());
}


const icmm::CTristimulusSpecification& CTristimulusSpecification::GetD50TwoDegree()
{
	static const icmm::CTristimulusSpecification spec(
		icmm::ObserverType::TwoDegree,
		icmm::AstmTableType::Unknown,
		std::make_shared<icmm::CIlluminant>(icmm::StandardIlluminant::D50));
	return spec;
}

} // namespace icmm


