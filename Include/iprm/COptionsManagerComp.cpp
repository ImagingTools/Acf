// SPDX-License-Identifier: LGPL-2.1-only
#include <iprm/COptionsManagerComp.h>


namespace iprm
{


// protected methods

// reimplemented (icomp::CComponentBase)

void COptionsManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetFixedOptionsList(m_fixedOptionsListCompPtr.GetPtr());
	SetSelectionSerialized(*m_serializeSelectionAttrPtr);
}


void COptionsManagerComp::OnComponentDestroyed()
{
	SetFixedOptionsList(NULL);

	BaseClass::OnComponentDestroyed();
}


} // namespace iprm


