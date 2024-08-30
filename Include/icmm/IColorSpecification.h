#pragma once


// STL includes
#include <memory>

// ACF includes
#include <istd/IChangeable.h>


namespace icmm
{


class IColorSpecification: virtual public istd::IChangeable
{
public:
	typedef std::shared_ptr<const IColorSpecification> ConstColorSpecPtr;
	typedef std::shared_ptr<IColorSpecification> ColorSpecPtr;

	enum class SpecType
	{
		Tristimulus,
		Spectral
	};

	virtual SpecType GetSpecificationType() const = 0;
	virtual ConstColorSpecPtr GetBaseSpecification() const;
};


inline IColorSpecification::ConstColorSpecPtr IColorSpecification::GetBaseSpecification() const
{
	return ConstColorSpecPtr();
}


} // namespace icmm


