#ifndef istd_TChangeDelegator_included
#define istd_TChangeDelegator_included


#include "istd/istd.h"


namespace istd
{


/**
	Help class which provides the automatic update mechanism of the model.
*/
template <class BaseClass>
class TChangeDelegator: public BaseClass, virtual public istd::IChangeable
{
public:
	typedef istd::IChangeable BaseClass2;

	TChangeDelegator();
	explicit TChangeDelegator(istd::IChangeable* parentPtr);

	virtual void SetSlavePtr(istd::IChangeable* parentPtr);
	virtual istd::IChangeable* GetSlavePtr() const;

	// reimplemented (istd::IChangeable)
	virtual void BeginChanges(int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);
	virtual void EndChanges(int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);
	
private:
	istd::IChangeable* m_slavePtr;
};


// public methods

template <class BaseClass>
TChangeDelegator<BaseClass>::TChangeDelegator()
{
	m_slavePtr = NULL;
}


template <class BaseClass>
TChangeDelegator<BaseClass>::TChangeDelegator(IChangeable* parentPtr)
	:m_slavePtr(parentPtr)
{
}


template <class BaseClass>
inline void TChangeDelegator<BaseClass>::SetSlavePtr(istd::IChangeable* parentPtr)
{
	m_slavePtr = parentPtr;
}


template <class BaseClass>
inline istd::IChangeable* TChangeDelegator<BaseClass>::GetSlavePtr() const
{
	return m_slavePtr;
}


// reimplemented (istd::IChangeable)

template <class BaseClass>
inline void TChangeDelegator<BaseClass>::BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass2::BeginChanges(changeFlags, changeParamsPtr);

	if (m_slavePtr != NULL){
		m_slavePtr->BeginChanges(changeFlags, changeParamsPtr);
	}
}


template <class BaseClass>
inline void TChangeDelegator<BaseClass>::EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass2::EndChanges(changeFlags, changeParamsPtr);

	if (m_slavePtr != NULL){
		m_slavePtr->EndChanges(changeFlags, changeParamsPtr);
	}
}


} // namespace istd


#endif // !istd_TChangeDelegator_included

