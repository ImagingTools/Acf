#ifndef imod_TModelWrap_included
#define imod_TModelWrap_included


#include "imod/imod.h"


#include "imod/CModelBase.h"


namespace imod
{


/**
	\ingroup imod

	This model wrapper provides a simple connection between a concrete IChangeable implementation
	and a model.
*/
template <class BaseClass>
class TModelWrap: virtual public BaseClass, public imod::CModelBase
{
public:
	TModelWrap();

	// pseudo-reimplemented (istd::IChangeable)
	virtual void BeginChanges(int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);
	virtual void EndChanges(int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);

private:
	int m_changeCounter;
};


template <class BaseClass>
TModelWrap<BaseClass>::TModelWrap()
{
	m_changeCounter = 0;
}


template <class BaseClass>
void TModelWrap<BaseClass>::BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::BeginChanges(changeFlags, changeParamsPtr);

	if (m_changeCounter == 0){
		NotifyBeforeUpdate(changeFlags, changeParamsPtr);
	}

	m_changeCounter++;
}


template <class BaseClass>
void TModelWrap<BaseClass>::EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::EndChanges(changeFlags, changeParamsPtr);

	m_changeCounter--;

	if (m_changeCounter == 0){
		NotifyAfterUpdate(changeFlags, changeParamsPtr);
	}
}


} // namespace imod


#endif // !imod_TModelWrap_included

