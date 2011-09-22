#ifndef imod_TModelDispatcherWrap_included
#define imod_TModelDispatcherWrap_included


// STL includes
#include <map>


// ACF includes
#include "imod/CMultiModelObserverBase.h"


namespace imod
{


/**
	Generic implementation of a data model changes notifier.

	You can register several data models objects that you want to observe with RegisterModel method.
	You will get a notification event on each change in the observed data model via OnModelChanged method call of your notification consumer.  
	\note Notification consumer class must implement the method
	void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr) to support the notification callback.
*/
template <class Base>
class TModelDispatcherWrap: public Base
{
public:
	typedef Base BaseClass;

	TModelDispatcherWrap();
	~TModelDispatcherWrap();

	/**
		Register the data model to be observed. If model registration was successfull, the function returns \c true.
		\param modelPtr	Pointer to the data model object, which should be observed.
		\param modelId	Logical model ID for possible event filtering in the notification callback.
		\param relefantFlags	The notification will only be triggered if the value of \c relevantFlags matches the model change flags.
	*/
	bool RegisterModel(imod::IModel* modelPtr, int modelId = 0, int relevantFlags = 0);
	
	/**
		Unregister the data model object.
		\param modelId	object ID used at the object registration.
		\sa RegisterModel
	*/
	void UnregisterModel(int modelId = 0);

	/**
		Unregister all observed data objects.
	*/
	void UnregisterAllModels();

	/**
		Get concrete object instance of the obsered model.
	*/
	template <class Object>
	Object* GetObjectPtr(int modelId) const;

	// abstract methods
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr) = 0;

protected:
	class ModelObserver: public imod::CMultiModelObserverBase
	{
	public:
		typedef imod::CMultiModelObserverBase BaseClass;

		ModelObserver(TModelDispatcherWrap& parent);

		// reimplemented (imod::IObserver)
		virtual bool OnDetached(imod::IModel* modelPtr);

		// reimplemented (imod::CMultiModelObserverBase)
		virtual void OnUpdate(imod::IModel* modelPtr, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		TModelDispatcherWrap& m_parent;
	};

private:
	struct ModelInfo
	{
		int modelId;
		int relevantFlags;
	};

	typedef std::map<imod::IModel*, ModelInfo> ModelMap;

	ModelMap m_modelMap;

	ModelObserver m_modelObserver;
};


// public methods
template <class Base>
TModelDispatcherWrap<Base>::TModelDispatcherWrap()
	:m_modelObserver(*this)
{
}


template <class Base>
TModelDispatcherWrap<Base>::~TModelDispatcherWrap()
{
	UnregisterAllModels();
}


template <class Base>
bool TModelDispatcherWrap<Base>::RegisterModel(imod::IModel* modelPtr, int modelId, int relevantFlags)
{
	if (modelPtr->AttachObserver(&m_modelObserver)){
		ModelInfo modelInfo;
		modelInfo.modelId = modelId;
		modelInfo.relevantFlags = relevantFlags;

		m_modelMap[modelPtr] = modelInfo;

		return true;
	}

	return false;
}


template <class Base>
void TModelDispatcherWrap<Base>::UnregisterModel(int modelId)
{
	for (typename ModelMap::iterator index = m_modelMap.begin(); index != m_modelMap.end(); index++){
		if (modelId == index->second.modelId){
			imod::IModel* modelPtr = index->first;

			m_modelMap.erase(index);

			modelPtr->DetachObserver(&m_modelObserver);

			break;
		}
	}
}


template <class Base>
void TModelDispatcherWrap<Base>::UnregisterAllModels()
{
	while (!m_modelMap.empty()){
		typename ModelMap::iterator currentIter = m_modelMap.begin();

		imod::IModel* modelPtr = currentIter->first;

		m_modelMap.erase(currentIter);

		modelPtr->DetachObserver(&m_modelObserver);
	}

	m_modelMap.clear();
}


template <class Base>
template <class Object>
Object* TModelDispatcherWrap<Base>::GetObjectPtr(int modelId) const
{
	for (typename ModelMap::const_iterator index = m_modelMap.begin(); index != m_modelMap.end(); index++){
		if (modelId == index->second.modelId){
			Object* objectPtr = dynamic_cast<Object*>(index->first);

			return objectPtr;
		}
	}

	return NULL;
}


// protected methods of the embedded class ModelObserver
		
template <class Base>
TModelDispatcherWrap<Base>::ModelObserver::ModelObserver(TModelDispatcherWrap& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)

template <class Base>
bool TModelDispatcherWrap<Base>::ModelObserver::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		for (typename ModelMap::iterator index = m_parent.m_modelMap.begin(); index != m_parent.m_modelMap.end(); index++){
			if (index->first == modelPtr){
				m_parent.m_modelMap.erase(index);

				break;
			}
		}

		return true;
	}

	return false;
}


// reimplemented (imod::CMultiModelObserverBase)

template <class Base>
void TModelDispatcherWrap<Base>::ModelObserver::OnUpdate(imod::IModel* modelPtr, int changeFlags, istd::IPolymorphic* updateParamsPtr)
{
	typename ModelMap::iterator foundModelIter = m_parent.m_modelMap.find(modelPtr);
	if (foundModelIter != m_parent.m_modelMap.end()){
		int relevantFlags = foundModelIter->second.relevantFlags;
		if (relevantFlags != 0 && (relevantFlags & changeFlags) == 0){
			return;
		}
			
		int modelId = foundModelIter->second.modelId;

		m_parent.OnModelChanged(modelId, changeFlags, updateParamsPtr);
	}
}


} // namespace imod


#endif // !imod_TModelDispatcherWrap_included


