#include "ilog/CLogComp.h"


namespace ilog
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CLogComp::OnComponentCreated()
{
	if (m_maxMessageCountAttrPtr.IsValid()){
		SetMaxMessageCount(m_maxMessageCountAttrPtr->GetValue());
	}
}


} // namespace ilog


