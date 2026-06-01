// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// STL includes
#include <algorithm>
#include <memory>

// ACF include
#include <istd/TNullable.h>


namespace istd
{


/**
	This is a comfort wrap for \c TNullable that allows several \c TNullableValue objects to own the same object.
	In other words this is equivalent for \c std::shared_ptr<TNullable<T>>
*/
template <class T>
class TNullableValue
{
public:
	TNullableValue();
	TNullableValue(const T& value);
	TNullableValue(T&& value);
	~TNullableValue();
	bool IsValid() const;
	bool IsNull() const;
	bool HasValue() const;
	const T* GetPtr() const;
	T* GetPtr();
	T& GetValue();
	const T& GetValue() const;
	void Reset();
	T& Emplace();
	void SetNull();

	bool isValid() const
	{
		return false;
	}
	bool isNull() const
	{
		return true;
	}

	// operators
	TNullableValue& operator=(const T& value);
	TNullableValue& operator=(T&& value);

	// STL-compatibility methods
	// access operators
	const T* operator->() const;
	T* operator->();
	const T& operator*() const &;
	T& operator*() &;
	const T&& operator*() const &&;
	T&& operator*() &&;

	// set operators
	TNullableValue& operator=( std::nullptr_t );
	TNullableValue& operator=(const TNullableValue& other);

	explicit operator bool() const;
	bool has_value() const;

	// value access
	T& value() &;
	const T& value() const &;
	T&& value() &&;
	const T&& value() const &&;
	T value_or(const T& default_value) const;

	void swap(TNullableValue& other );
	void reset();
	T& emplace();
	T& emplace(T&& value)&&;

	//Compare two TNullableValue objects
	bool operator==(const TNullableValue& other) const;
	bool operator!=(const TNullableValue& other) const;

	//Compare a TNullableValue object with a nullptr
	bool operator==(std::nullptr_t) const;
	bool operator!=(std::nullptr_t) const;

	//Compare a TNullableValue object with a value
	bool operator==(const T& value) const;
	bool operator!=(const T& value) const;

private:
	std::shared_ptr<TNullable<T>> m_dataPtr;
};

template <class T>
bool Less(const istd::TNullableValue<T> &lhs, const istd::TNullableValue<T> &rhs)
{
	return *lhs < *rhs;
}


// public methods

template <class T>
TNullableValue<T>::TNullableValue()
	:m_dataPtr(new TNullable<T>())
{
}


template<class T>
inline TNullableValue<T>::TNullableValue(const T& value)
	:m_dataPtr(new TNullable<T>())
{
	*m_dataPtr = value;
}


template<class T>
inline TNullableValue<T>::TNullableValue(T&& value)
	:m_dataPtr(new TNullable<T>())
{
	*m_dataPtr = std::move(value);
}


template <class T>
TNullableValue<T>::~TNullableValue()
{
}


template<class T>
inline bool TNullableValue<T>::IsValid() const
{
	return m_dataPtr->IsValid();
}


template<class T>
bool TNullableValue<T>::IsNull() const
{
	return m_dataPtr->IsNull();
}


template<class T>
bool TNullableValue<T>::HasValue() const
{
	return m_dataPtr->HasValue();
}


template<class T>
const T* TNullableValue<T>::GetPtr() const
{
	return m_dataPtr->GetPtr();
}


template<class T>
T* TNullableValue<T>::GetPtr()
{
	return m_dataPtr->GetPtr();
}


template<class T>
T& TNullableValue<T>::GetValue()
{
	return m_dataPtr->GetValue();
}


template<class T>
const T& TNullableValue<T>::GetValue() const
{
	return m_dataPtr->GetValue();
}


template<class T>
void TNullableValue<T>::Reset()
{
	return m_dataPtr.reset(new TNullable<T>);
}


template<class T>
T& TNullableValue<T>::Emplace()
{
	return m_dataPtr->Emplace();
}


template<class T>
inline void TNullableValue<T>::SetNull()
{
	m_dataPtr->SetNull();
}


// operators

template<class T>
TNullableValue<T>& TNullableValue<T>::operator=(const T& value)
{
	m_dataPtr->operator=(value);

	return *this;
}


template<class T>
TNullableValue<T>& TNullableValue<T>::operator=(T&& value)
{
	m_dataPtr->operator=(std::move(value));

	return *this;
}


// STL-compatibility methods
// access operators

template<class T>
const T* TNullableValue<T>::operator->() const
{
	return m_dataPtr->operator->();
}


template<class T>
T* TNullableValue<T>::operator->()
{
	return m_dataPtr->operator->();
}


template<class T>
const T& TNullableValue<T>::operator*() const &
{
	return m_dataPtr->operator*();
}


template<class T>
T& TNullableValue<T>::operator*() &
{
	return m_dataPtr->operator*();
}


template<class T>
const T&& TNullableValue<T>::operator*() const &&
{
	return m_dataPtr->operator*();
}


template<class T>
T&& TNullableValue<T>::operator*() &&
{
	return m_dataPtr->operator*();
}


template<class T>
TNullableValue<T>& TNullableValue<T>::operator=( std::nullptr_t )
{
	m_dataPtr->operator=(nullptr);

	return *this;
}


template<class T>
TNullableValue<T>& TNullableValue<T>::operator=(const TNullableValue& other)
{
	m_dataPtr = other.m_dataPtr;

	return *this;
}


template<class T>
TNullableValue<T>::operator bool() const
{
	return m_dataPtr->operator bool();
}


template<class T>
bool TNullableValue<T>::has_value() const
{
	return m_dataPtr->has_value();
}


template<class T>
T& TNullableValue<T>::value() &
{
	return m_dataPtr->value();
}


template<class T>
const T& TNullableValue<T>::value() const &
{
	return m_dataPtr->value();
}


template<class T>
T&& TNullableValue<T>::value() &&
{
	return m_dataPtr->value();
}


template<class T>
const T&& TNullableValue<T>::value() const &&
{
	return m_dataPtr->value();
}


template<class T>
T TNullableValue<T>::value_or(const T& default_value) const
{
	return m_dataPtr->value_or(default_value);
}


template<class T>
void TNullableValue<T>::swap(TNullableValue& other )
{
	m_dataPtr.swap(other.m_dataPtr);
}


template<class T>
void TNullableValue<T>::reset()
{
	Reset();
}


template<class T>
T& TNullableValue<T>::emplace()
{
	return Emplace();
}


template<class T>
T& TNullableValue<T>::emplace(T&& value)&&
{
	return m_dataPtr->emplace();
}


template<class T>
bool TNullableValue<T>::operator==(const TNullableValue& other) const
{
	return m_dataPtr.get() == other.m_dataPtr.get();
}


template<class T>
bool TNullableValue<T>::operator!=(const TNullableValue& other) const
{
	return (!operator==(other));
}


template<class T>
bool TNullableValue<T>::operator==(std::nullptr_t) const
{
	return m_dataPtr->operator==(nullptr);
}


template<class T>
bool TNullableValue<T>::operator!=(std::nullptr_t) const
{
	return m_dataPtr->operator!=(nullptr);
}


template<class T>
bool TNullableValue<T>::operator==(const T& value) const
{
	return m_dataPtr->operator==(value);
}


template<class T>
bool TNullableValue<T>::operator!=(const T& value) const
{
	return m_dataPtr->operator!=(value);
}


} // namespace istd


template <class T>
inline unsigned int qHash(const istd::TNullableValue<T> &key, unsigned int seed){
	return qHash(*key, seed);
}

