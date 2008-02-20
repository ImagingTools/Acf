#ifndef istd_TDelPtr_included
#define istd_TDelPtr_included


#include "istd/TPointer.h"


namespace istd
{


/**
	Pointer wrapper providing automatic deleting pointed object during destruction.
*/
template <class Type, bool DelArray = false>
class TDelPtr: public TPointer<Type>
{
public:
	typedef TPointer<Type> BaseClass;

	/**
		Construct and init this pointer.
		\param	ptr		pointer to object.
	*/
	TDelPtr(Type* ptr = NULL);

	/**
		Copy constructor.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TDelPtr(const TDelPtr& ptr);

	/**
		Destructor.
		It calls Reset() to delete pointed object.
	 */
	~TDelPtr();

	/**
		Remove object pointed by internal pointer and set this pointer to NULL.
	*/
	void Reset();

	/**
		Set new value of internal pointer.
		If internal pointer wasn't NULL, it will be deleted.
	 */
	void SetPtr(Type* ptr);

	/**
		Reset internal pointer value without deleting instance and return previos value.
	 */
	Type* PopPtr();

	/**
		Assign operator.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TDelPtr& operator=(const TDelPtr& ptr);
	/**
		Assign operator.
		It removes refererenced object before new is assigned.
		\param	ptr		pointer to object.
	*/
	TDelPtr& operator=(Type* ptr);

protected:
	/**
		Remove referenced object.
		Warning: internal pointer will be not changed.
	*/
	void DeleteIntern();
};


// inline methods

template <class Type, bool DelArray>
inline TDelPtr<Type, DelArray>::TDelPtr(Type* ptr)
	:	BaseClass(ptr)
{
}


template <class Type, bool DelArray>
inline TDelPtr<Type, DelArray>::TDelPtr(const TDelPtr<Type, DelArray>& ptr)
{
	I_ASSERT(ptr.GetPtr() == NULL);
}


template <class Type, bool DelArray>
inline TDelPtr<Type, DelArray>::~TDelPtr()
{
	DeleteIntern();
}


template <class Type, bool DelArray>
inline void TDelPtr<Type, DelArray>::Reset()
{
	SetPtr(NULL);
}


template <class Type, bool DelArray>
inline void TDelPtr<Type, DelArray>::SetPtr(Type* ptr)
{
	DeleteIntern();

	BaseClass::SetPtr(ptr);
}


template <class Type, bool DelArray>
inline typename Type* TDelPtr<Type, DelArray>::PopPtr()
{
	Type* slavePtr = BaseClass::GetPtr();
	BaseClass::SetPtr(NULL);

	return slavePtr;
}


// public methods

template <class Type, bool DelArray>
TDelPtr<Type, DelArray>& TDelPtr<Type, DelArray>::operator=(const TDelPtr& ptr)
{
	I_ASSERT(ptr.GetPtr() == NULL);

	Reset();

	return *this;
}


template <class Type, bool DelArray>
TDelPtr<Type, DelArray>& TDelPtr<Type, DelArray>::operator=(Type* ptr)
{
	DeleteIntern();

	BaseClass::SetPtr(ptr);

	return *this;
}


// protected methods

template <class Type, bool DelArray>
void TDelPtr<Type, DelArray>::DeleteIntern()
{
	Type* ptr = BaseClass::GetPtr();
	if (ptr == NULL){
		return;
	}

	if (DelArray){
		delete[] ptr;
	}
	else{
		delete ptr;
	}
}


} // namespace istd


#endif // !istd_TDelPtr_included


