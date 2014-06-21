#include "istd/CChangeNotifier.h"


// ACF includes
#include "istd/IChangeable.h"


namespace istd
{


void CChangeNotifier::AppendChangeId(int changeId)
{
	m_changeIds += changeId;
}


void CChangeNotifier::AppendChangeIds(const IChangeable::ChangeSet& changeSet)
{
	m_changeIds += changeSet;
}


bool CChangeNotifier::IsValid() const
{
	return m_changeablePtr != NULL;
}


void CChangeNotifier::Reset()
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->EndChanges(m_changeIds);

		m_changeablePtr = NULL;
	}
}


void CChangeNotifier::Abort()
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->EndChanges(IChangeable::GetNoChanges());

		m_changeablePtr = NULL;
	}
}


// private methods

CChangeNotifier::CChangeNotifier(const CChangeNotifier& /*notifier*/)
{
}


} // namespace istd


