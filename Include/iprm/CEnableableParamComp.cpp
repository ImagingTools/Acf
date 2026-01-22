// SPDX-License-Identifier: LGPL-2.1-only
#include <iprm/CEnableableParamComp.h>


namespace iprm
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CEnableableParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultIsEnabledAttrPtr.IsValid()){
		SetEnabled(*m_defaultIsEnabledAttrPtr);
	}
}


} // namespace iprm


