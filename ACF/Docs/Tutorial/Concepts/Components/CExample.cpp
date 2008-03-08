#include "CExample.h"


namespace itut
{


CExample::CExample(const icomp::IComponentContext* contextPtr)
:	BaseClass(contextPtr)
{
	// initialize all component attributes and references
	I_ASSIGN(m_value, context, "Value", false);
	I_ASSIGN(m_valueList, "ValueList", false);
	I_ASSIGN(m_cloneableRef, "Cloneable", false);
	I_ASSIGN(m_cloneableListRef, "CloneableList", false);
	I_ASSIGN(m_cloneableFact, "CloneableFactory", false);
	I_ASSIGN(m_cloneableListFact, "CloneableFactoryList", false);
}


} // namespace itut


