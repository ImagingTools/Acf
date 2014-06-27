#ifndef istd_CChangeGroup_included
#define istd_CChangeGroup_included


// ACF includes
#include "istd/IChangeable.h"


namespace istd
{


/**
	Help class which provides the group of changes for update mechanism of the model.
	\note It should not be overriden.

	\ingroup ModelObserver
	\ingroup Main
	\ingroup DataModel
*/
class CChangeGroup
{
public:
	/**
		Constructs the group.
		\param	changeablePtr	Your object prepared to change. If it is NULL, nothing will be done.
		\param	changeSet		Set of change flags (its IDs).
	*/
	explicit CChangeGroup(IChangeable* changeablePtr, const IChangeable::ChangeSet& changeSet = IChangeable::GetNoChanges());
	~CChangeGroup();

	/**
		It allows to append new set of changes to the notification.
	*/
	void AppendChangeId(int changeId);

	/**
		It allows to append new set of changes to the notification.
	*/
	void AppendChangeIds(const IChangeable::ChangeSet& changeSet);

	/**
		Check if this pointer is valid.
		It will be invalid after \c Reset() and \c Abort() only.
	*/
	bool IsValid() const;

	/**
		Call update on attached object and detach it.
	*/
	void Reset();

private:
	// blocked copy constructor
	CChangeGroup(const CChangeGroup& group);

	IChangeable* m_changeablePtr;
	IChangeable::ChangeSet m_changeIds;
};


inline CChangeGroup::CChangeGroup(IChangeable* changeablePtr, const IChangeable::ChangeSet& changeSet)
:	m_changeablePtr(changeablePtr),
	m_changeIds(changeSet)
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->BeginChangeGroup(changeSet);
	}
}


inline CChangeGroup::~CChangeGroup()
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->EndChangeGroup(m_changeIds);
	}
}


} // namespace istd


#endif // !istd_CChangeGroup_included


