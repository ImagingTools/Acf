#pragma once


// ACF includes
#include <icmm/IColorSpecification.h>
#include <icmm/IIluminant.h>


namespace icmm
{


class ITristimulusSpecification: virtual public IColorSpecification
{
public:
	virtual const IIluminant& GetIlluminant() const = 0;
	virtual ObserverType GetObserverType() const = 0;
	virtual AstmTableType GetMethod() const = 0;

protected:
	// reimplemented (IColorSpecification)
	virtual SpecType GetSpecificationType() const override;
};


inline ITristimulusSpecification::SpecType ITristimulusSpecification::GetSpecificationType() const
{
	return SpecType::Tristimulus;
}


} // namespace icmm


