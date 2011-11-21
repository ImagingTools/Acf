#include "iprm/CNameParamComp.h"


namespace iprm
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CNameParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultNameAttrPtr.IsValid()){
		SetName(*m_defaultNameAttrPtr);
	}
}


} // namespace ibase


