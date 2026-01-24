// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// STD includes
#include <memory>

// ACF includes
#include <istd/TTransPtr.h>


namespace istd
{


/**
	\deprecated Use QSharedPointer instead!
	\ingroup Main
*/
template <class Type>
class TSmartPtr: public TTransPtr<Type>
{
public:
	TSmartPtr();
	
	explicit TSmartPtr(Type* pointer);

	TSmartPtr(const TTransPtr<Type>& trans);

	void SetPtr(Type* pointer);
	bool operator==(const TSmartPtr& other) const;

	/**
		Set this pointer using casted pointer of other type.
		If casting is not possible, object will be removed.
	*/
	template <class SourceType>
	bool SetCastedOrRemove(SourceType* pointer)
	{
		Type* castedPtr = dynamic_cast<Type*>(pointer);

		SetPtr(castedPtr);

		if (castedPtr != NULL){
			return true;
		}
		else{
			if (pointer != NULL){
				delete pointer;
			}

			return false;
		}
	}

	using TTransPtr<Type>::m_impl;
};


template <class Type>
inline TSmartPtr<Type>::TSmartPtr() : TTransPtr<Type>()
{
}


template <class Type>
inline TSmartPtr<Type>::TSmartPtr(Type* pointer)
{
	m_impl.reset(pointer);
}


template<class Type>
inline TSmartPtr<Type>::TSmartPtr(const TTransPtr<Type> & trans)
{
	m_impl = trans.m_impl;
}


template <class Type>
inline void TSmartPtr<Type>::SetPtr(Type* pointer)
{
	m_impl.reset(pointer);
}


template <class Type>
inline bool TSmartPtr<Type>::operator==(const TSmartPtr<Type>& other) const
{
	return this->GetPtr() == other.GetPtr();
}


} // namespace istd




