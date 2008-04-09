#ifndef iqt_TGuiModelJoinerWrap_included
#define iqt_TGuiModelJoinerWrap_included


#include "imod/IModelEditor.h"


namespace iqt
{


/**
	Join functionality of \c iqt::IGuiObject interface and \c imod::IObserver.
*/
template <class Gui, class Model>
class TGuiModelJoinerWrap: public Gui, public Model, virtual public imod::IModelEditor
{
public:
	TGuiModelJoinerWrap();

	// pseudo-reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

protected:
	/**
		Called when model is attached and GUI is shown.
	*/
	virtual void OnGuiModelShown();

	/**
		Called when model is detached or GUI is hidden.
	*/
	virtual void OnGuiModelHidden();

	/**
		Called when model is attached and GUI is created.
	*/
	virtual void OnGuiModelAttached();

	/**
		Called when model is detached or GUI is destroyed.
	*/
	virtual void OnGuiModelDetached();

	// pseudo-reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiShown();
	virtual void OnGuiHidden();
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// pseudo-reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

	// pseudo-reimplemented (imod::IModelEditor)
	virtual bool IsReadOnly() const;
	virtual void SetReadOnly(bool state);

private:
	bool m_ignoreUpdates;
	bool m_isReadOnly;
};


// public methods

template <class Gui, class Model>
TGuiModelJoinerWrap<Gui, Model>::TGuiModelJoinerWrap()
:	m_ignoreUpdates(false), m_isReadOnly(false)
{
}


// pseudo-reimplemented (imod::IObserver)

template <class Gui, class Model>
bool TGuiModelJoinerWrap<Gui, Model>::OnAttached(imod::IModel* modelPtr)
{
	bool retVal = Model::OnAttached(modelPtr);

	if (retVal && IsGuiCreated() && IsModelAttached(NULL)){
		OnGuiModelAttached();
	}

	return retVal;
}


template <class Gui, class Model>
bool TGuiModelJoinerWrap<Gui, Model>::OnDetached(imod::IModel* modelPtr)
{
	if (!m_isReadOnly && IsModelAttached(modelPtr)){
		UpdateModel();
	}

	bool retVal = Model::OnDetached(modelPtr);

	if (retVal && IsGuiCreated() && !IsModelAttached(NULL)){
		OnGuiModelDetached();
	}

	return retVal;
}


// protected methods

template <class Gui, class Model>
void TGuiModelJoinerWrap<Gui, Model>::OnGuiModelShown()
{
}


template <class Gui, class Model>
void TGuiModelJoinerWrap<Gui, Model>::OnGuiModelHidden()
{
}


template <class Gui, class Model>
void TGuiModelJoinerWrap<Gui, Model>::OnGuiModelAttached()
{
	I_ASSERT(IsGuiCreated());
	I_ASSERT(IsModelAttached(NULL));

	if (!m_ignoreUpdates){
		m_ignoreUpdates = true;

		UpdateEditor();

		m_ignoreUpdates = false;
	}
}


template <class Gui, class Model>
void TGuiModelJoinerWrap<Gui, Model>::OnGuiModelDetached()
{
	if (!m_isReadOnly && IsModelAttached(NULL)){
		UpdateModel();
	}
}


// pseudo-reimplemented (iqt::CGuiComponentBase)

template <class Gui, class Model>
void TGuiModelJoinerWrap<Gui, Model>::OnGuiShown()
{
	Gui::OnGuiShown();

	if (IsModelAttached(NULL)){
		OnGuiModelShown();
	}
}


template <class Gui, class Model>
void TGuiModelJoinerWrap<Gui, Model>::OnGuiHidden()
{
	Gui::OnGuiHidden();

	if (IsModelAttached(NULL)){
		OnGuiModelHidden();
	}
}


template <class Gui, class Model>
void TGuiModelJoinerWrap<Gui, Model>::OnGuiCreated()
{
	Gui::OnGuiCreated();

	if (IsModelAttached(NULL)){
		OnGuiModelAttached();
	}
}


template <class Gui, class Model>
void TGuiModelJoinerWrap<Gui, Model>::OnGuiDestroyed()
{
	if (IsModelAttached(NULL)){
		OnGuiModelDetached();
	}

	Gui::OnGuiDestroyed();
}


// pseudo-reimplemented (imod::IObserver)

template <class Gui, class Model>
void TGuiModelJoinerWrap<Gui, Model>::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(modelPtr != NULL);
	I_ASSERT(IsModelAttached(modelPtr));

	Model::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);

	if (!m_ignoreUpdates && IsGuiCreated()){
		m_ignoreUpdates = true;

		UpdateEditor();

		m_ignoreUpdates = false;
	}
}


// pseudo-reimplemented (imod::IModelEditor)

template <class Gui, class Model>
bool TGuiModelJoinerWrap<Gui, Model>::IsReadOnly() const
{
	return m_isReadOnly;
}


template <class Gui, class Model>
void TGuiModelJoinerWrap<Gui, Model>::SetReadOnly(bool state)
{
	m_isReadOnly = state;
}


} // namespace iqt


#endif //!iqt_TGuiModelJoinerWrap_included


