#ifndef imod_TModelWrap_included
#define imod_TModelWrap_included


#include "iser/TCopySerializedWrap.h"

#include "imod/CModelBase.h"


namespace imod
{


/**
	This model wrapper provides a simple connection between a concrete
	\c istd::IChangeable implementation and a model.

	\ingroup ModelObserver
	\ingroup Helpers
*/
template <class Base>
class TModelWrap: public iser::TCopySerializedWrap<Base>, public imod::CModelBase
{
public:
	typedef iser::TCopySerializedWrap<Base> BaseClass;
	typedef imod::CModelBase BaseClass2;

	void SetBaseObject(const Base& baseObject);

protected:
	// pseudo-reimplemented (istd::IChangeable)
	virtual void OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
};


// public methods

template <class Base>
void TModelWrap<Base>::SetBaseObject(const Base& baseObject)
{
	Base::operator=(baseObject);
}


// protected methods

// pseudo-reimplemented (istd::IChangeable)

template <class Base>
void TModelWrap<Base>::OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnBeginChanges(changeFlags, changeParamsPtr);

	NotifyBeforeUpdate(changeFlags, changeParamsPtr);
}


template <class Base>
void TModelWrap<Base>::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	NotifyAfterUpdate(changeFlags, changeParamsPtr);
}


} // namespace imod


#endif // !imod_TModelWrap_included


