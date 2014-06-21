#include "istd/CChangeDelegator.h"


namespace istd
{


// public methods

CChangeDelegator::CChangeDelegator()
{
	m_slavePtr = NULL;
}


CChangeDelegator::CChangeDelegator(IChangeable* slavePtr)
:	m_slavePtr(slavePtr)
{
}


// reimplemented (istd::IChangeable)

void CChangeDelegator::OnBeginChanges()
{
	if (m_slavePtr != NULL){
		static ChangeSet delegateIds(CF_DELEGATED);

		m_slavePtr->BeginChanges(delegateIds);
	}
}


void CChangeDelegator::OnEndChanges(const ChangeSet& changeSet)
{
	if ((m_slavePtr != NULL) && !changeSet.IsEmpty()){
		static ChangeSet delegateIds(CF_DELEGATED);
		m_slavePtr->EndChanges(delegateIds);
	}
}


} // namespace istd

