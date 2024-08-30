#pragma once


// ACF includes
#include <icmm/IColorSpecification.h>


namespace icmm
{


template <class Base>
class THierarchicalSpecificationWrap: public Base
{
public:
	void SetBaseSpecification(IColorSpecification::ConstColorSpecPtr baseSpecPtr)
	{
		m_baseSpecPtr = baseSpecPtr;
	}

	// pseudo-reimplemented (IColorSpecification)
	virtual IColorSpecification::ConstColorSpecPtr GetBaseSpecification() const override
	{
		return m_baseSpecPtr;
	}

protected:
	IColorSpecification::ConstColorSpecPtr m_baseSpecPtr;
};


} // namespace icmm


