#include "CExample.h"


namespace itut
{


CExample::CExample(const icomp::IComponentContext* contextPtr)
:	BaseClass(contextPtr)
{
	// initialize all component attributes and references
	I_REGISTER_INTERFACE(ITestInterface);
	I_ASSIGN(m_value, "Value", "Test of attribute", 0, false);
	I_ASSIGN(m_valueList, "ValueList", "Test of attribute list", , false);
	I_ASSIGN(m_cloneableRef, "Cloneable", "Test of reference", "Cloneable", false);
	I_ASSIGN(m_cloneableListRef, "CloneableList", "Test of reference list", , false);
	I_ASSIGN(m_cloneableFact, "CloneableFactory", "Test of factory", "CloneableFactory", false);
	I_ASSIGN(m_cloneableListFact, "CloneableFactoryList", "Test of factory list", , false);
}


} // namespace itut


