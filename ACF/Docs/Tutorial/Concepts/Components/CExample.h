#ifndef CExample_included
#define CExample_included


#include "istd/IPolymorphic.h"

#include "icomp/CComponentBase.h"


namespace itut
{


/**
	This is example interface only.
	Normally it shouldn't be defined in class file.
*/
class ITestInterface: public istd::IPolymorphic
{
public:
	virtual int GetValue() const = 0;
};


/**
	Demonstrate using of all elements of component concept.
*/
class CExample: public icomp::CComponentBase, public ITestInterface
{
	typedef CComponentBase BaseClass;

	I_COMPONENT(CExample);
	I_REGISTER_INTERFACE(ITestInterface);

public:
	CExample(const icomp::IComponentContext* contextPtr);

private:
	/**
		Normal attribute.
	*/
	I_ATTR(CIntAttr, m_value);
	/**
		Set of attributes.
	*/
	I_MULTIATTR(CIntAttr, m_valueList);
	/**
		Normal reference to other component.
	*/
	I_REF(IClonable, m_cloneableRef);
	/**
		Set of references to other components.
	*/
	I_MULTIREF(IClonable, m_cloneableListRef);
	/**
		Normal factory.
	*/
	I_FACT(IClonable, m_cloneableFact);
	/**
		Set of factories.
	*/
	I_MULTIFACT(IClonable, m_cloneableListFact);
};


} // namespace itut


#indif // !CExample_included


