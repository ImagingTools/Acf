#include "istd/CChangeGroup.h"


// ACF includes
#include "istd/IChangeable.h"


namespace istd
{


void CChangeGroup::AppendChangeId(int changeId)
{
	m_changeIds += changeId;
}


void CChangeGroup::AppendChangeIds(const IChangeable::ChangeSet& changeSet)
{
	m_changeIds += changeSet;
}


bool CChangeGroup::IsValid() const
{
	return m_changeablePtr != NULL;
}


void CChangeGroup::Reset()
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->EndChanges(m_changeIds);

		m_changeablePtr = NULL;
	}
}


// private methods

CChangeGroup::CChangeGroup(const CChangeGroup& /*group*/)
{
}


} // namespace istd


