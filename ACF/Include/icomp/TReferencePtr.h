#ifndef icomp_CReferencePtr_included
#define icomp_CReferencePtr_included


#include "icomp/IComponentContext.h"
#include "icomp/TAttributePtr.h"
#include "icomp/CReferenceAttribute.h"


namespace icomp
{


/**
	Pointer to referenced component object.
	Don't use direct this class, use macros I_REF and I_ASSIGN instead.
*/
template <class Interface>
class TReferencePtr: public TAttributePtr<CReferenceAttribute>
{
public:
	typedef TAttributePtr<CReferenceAttribute> BaseClass;

	TReferencePtr();

	void Init(const IComponent* ownerPtr, const IAttributeStaticInfo* infoPtr);

	/**
		Check if this reference can be resolved.
	*/
	bool IsValid() const;
	/**
		Return access to component associated with this reference.
	 */
	IComponent* GetComponent() const;

	/**
		Access to internal pointer.
	*/
	const Interface* operator->() const;

	/**
		Access to object pointed by internal pointer.
	*/
	const Interface& operator*() const;

protected:
	TReferencePtr(const TReferencePtr& ptr);

	bool EnsureInitialized();

private:
	mutable IComponent* m_componentPtr;
	const IComponentContext* m_realContextPtr;
};


// public methods

template <class Interface>
TReferencePtr::TReferencePtr()
:	m_componentPtr(NULL), m_realContextPtr(NULL)
{
}


template <class Interface>
void TReferencePtr::Init(const IComponent* ownerPtr, const IAttributeStaticInfo* infoPtr)
{
	BaseClass::Init(ownerPtr, infoPtr, &m_realContextPtr);

	m_componentPtr = NULL;
}


template <class Interface>
bool TReferencePtr::IsValid() const
{
	return EnsureInitialized();
}


template <class Interface>
IComponent* TReferencePtr::GetComponent() const
{
	EnsureInitialized();

	return m_componentPtr;
}


// protected methods

template <class Interface>
TReferencePtr<Interface>::TReferencePtr(const TReferencePtr& ptr)
:	BaseClass(ptr), m_componentPtr(ptr.m_componentPtr), m_realContextPtr(ptr.m_realContextPtr)
{
}


template <class Interface>
bool TReferencePtr::EnsureInitialized() const
{
	if (m_componentPtr != NULL){
		return true;
	}

	if ((m_realContextPtr != NULL) && BaseClass::IsValid()){
		const CReferenceAttribute& attribute = *GetAttributePtr();

		const IComponentContext* parentPtr = m_realContextPtr->GetParentContext();
		if (parentPtr != NULL){
			m_componentPtr = parentPtr->GetSubcomponent(attribute.GetComponentId());
		}
	}

	return (m_componentPtr != NULL);
}


}//namespace icomp


#endif // !icomp_CReferencePtr_included


