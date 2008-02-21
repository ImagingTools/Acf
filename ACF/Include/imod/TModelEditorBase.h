#ifndef imod_TModelEditorBase_included
#define imod_TModelEditorBase_included


#include "imod/imod.h"


#include "imod/IModelEditor.h"
#include "imod/TSingleModelObserverBase.h"


namespace imod
{


/**
	Basic implementation for an single model editor
*/
template <class InterfaceClass>
class TModelEditorBase:	virtual public imod::IModelEditor, 
						public imod::TSingleModelObserverBase<InterfaceClass>
{
public:
	typedef imod::TSingleModelObserverBase<InterfaceClass> BaseClass;

	TModelEditorBase();

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual void OnDetached(imod::IModel* modelPtr);

	// reimplemented (imod::IModelEditor)
	virtual bool IsModelChangable() const;

protected:
	// reimplemented (TSingleModelObserverBase<InterfaceClass>)
	virtual void Update(imod::IModel* modelPtr, int updateFlags = 0, imod::IPolymorphic* updateParamsPtr = NULL);

	bool m_ignoreUpdates;
};


template <class InterfaceClass>
TModelEditorBase<InterfaceClass>::TModelEditorBase()
{
	m_ignoreUpdates = false;
}


// reimplemented (IObserver)

template <class InterfaceClass>
bool TModelEditorBase<InterfaceClass>::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		UpdateEditor();

		return true;
	}

	return false;
}


template <class InterfaceClass>
void TModelEditorBase<InterfaceClass>::OnDetached(imod::IModel* modelPtr)
{
	BaseClass::OnDetached(modelPtr);

	UpdateEditor();
}


// reimplemented (imod::IModelEditor)

template <class InterfaceClass>
bool TModelEditorBase<InterfaceClass>::IsModelChangable() const
{
	return true;
}


// protected methods

// reimplemented (TSingleModelObserverBase<InterfaceClass>)

template <class InterfaceClass>
void TModelEditorBase<InterfaceClass>::Update(imod::IModel* modelPtr, int updateFlags, imod::IPolymorphic* updateParamsPtr)
{
	if (m_modelPtr == modelPtr && m_modelPtr != NULL){
		if (!m_ignoreUpdates){
			m_ignoreUpdates = true;
			
			UpdateEditor();
		
			m_ignoreUpdates = false;
		}
	}
}


} // namespace imod


#endif // !imod_TModelEditorBase_included