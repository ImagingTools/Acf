#ifndef istd_TCascadedMap_included
#define istd_TCascadedMap_included


#include <vector>
#include <map>


namespace istd
{


/**
	Helper class used to manage list of many connected in cascade maps.
	Please note, that elements are accessed using its index.
	At this moment no element removing is supported.
	Local elements have begining indices.
*/
template <typename Key, typename Value>
class TCascadedMap
{
public:
	/**
		Constructor with optional assigning of parent map.
	*/
	TCascadedMap(const TCascadedMap<Key, Value>* parentPtr = NULL);

	/**
		Get access to parent map instance.
	*/
	const TCascadedMap<Key, Value>* GetParent() const;

	/**
		Set instance of parent map.
	*/
	void SetParent(const TCascadedMap<Key, Value>* parentPtr);

	/**
		Element access operator.
		If key object not exists, it will be automatically created.
	*/
	Value& operator[](const Key& key);

	/**
		Element access operator.
		If key object not exists, it will be automatically created.
	*/
	const Value& operator[](const Key& key) const;

	/**
		Find index index of specified key.
	*/
	int FindIndex(const Key& key) const;

	/**
		Find index index of specified key using local context only.
	*/
	int FindLocalIndex(const Key& key) const;

	/**
		Find value element associated with specified key.
		\return	pointer to associated value, or NULL if key not exists.
	*/
	const Value* FindElement(const Key& key) const;

	/**
		Find value element associated with specified key using local context only.
		\return	pointer to associated value, or NULL if key not exists.
	*/
	const Value* FindLocalElement(const Key& key) const;

	/**
		Find value element associated with specified key using local context only.
		\return	pointer to associated value, or NULL if key not exists.
	*/
	Value* FindLocalElement(const Key& key);

	/**
		Get key value at specified index.
	*/
	const Key& GetKeyAt(int index) const;
	/**
		Get key value from local context at specified index.
	*/
	const Key& GetLocalKeyAt(int index) const;
	/**
		Get key value from local context at specified index.
	*/
	Key& GetLocalKeyAt(int index);

	/**
		Get mapped value at specified index.
	*/
	const Value& GetValueAt(int index) const;

	/**
		Get mapped value from local context at specified index.
	*/
	const Value& GetLocalValueAt(int index) const;

	/**
		Get mapped value from local context at specified index.
	*/
	Value& GetLocalValueAt(int index);

	/**
		Get number of elements.
	*/
	int GetElementsCount() const;

	/**
		Get number of elements in local context.
	*/
	int GetLocalElementsCount() const;

	/**
		Insert element in local context.
	*/
	bool InsertLocal(const Key& key, const Value& value);

	/**
		Get owner context of element at specified index.
	*/
	const TCascadedMap<Key, Value>* GetOwnerMap(int index) const;

	/**
		Get owner context of element at specified index.
	*/
	TCascadedMap<Key, Value>* GetOwnerMap(int index);

	// TODO: add element removing and correct comment in class header.

private:
	const TCascadedMap<Key, Value>* m_parentPtr;

	typedef ::std::map<Key, int> IndexsMap;
	typedef ::std::pair<Key, Value> Pair;
	typedef ::std::vector<Pair> PairList;

	mutable IndexsMap m_positionsMap;
	mutable PairList m_pairList;
};


// public methods

template <typename Key, typename Value>
TCascadedMap<Key, Value>::TCascadedMap(const TCascadedMap<Key, Value>* parentPtr)
:	m_parentPtr(parentPtr)
{
}


template <typename Key, typename Value>
typename const TCascadedMap<Key, Value>* TCascadedMap<Key, Value>::GetParent() const
{
	return m_parentPtr;
}


template <typename Key, typename Value>
void TCascadedMap<Key, Value>::SetParent(const TCascadedMap<Key, Value>* parentPtr)
{
	m_parentPtr = parentPtr;
}


template <typename Key, typename Value>
Value& TCascadedMap<Key, Value>::operator[](const Key& key)
{
	IndexsMap::const_iterator iter = m_positionsMap.find(key);
	if (iter != m_positionsMap.end()){
		return m_pairList[iter->second];
	}

	int newIndex = int(m_pairList.size());
	m_positionsMap[key] = newIndex;

	m_pairList.push_back(Pair(key, Value()));

	return m_positionsMap.back();
}


template <typename Key, typename Value>
const Value& TCascadedMap<Key, Value>::operator[](const Key& key) const
{
	return const_cast<TCascadedMap*>(this)->operator[](key);
}


template <typename Key, typename Value>
int TCascadedMap<Key, Value>::FindIndex(const Key& key) const
{
	IndexsMap::const_iterator iter = m_positionsMap.find(key);
	if (iter != m_positionsMap.end()){
		return iter->second;
	}

	if (m_parentPtr != NULL){
		return m_parentPtr->FindIndex(key);
	}

	return -1;
}


template <typename Key, typename Value>
int TCascadedMap<Key, Value>::FindLocalIndex(const Key& key) const
{
	IndexsMap::const_iterator iter = m_positionsMap.find(key);
	if (iter != m_positionsMap.end()){
		return iter->second;
	}

	return -1;
}


template <typename Key, typename Value>
typename const Value* TCascadedMap<Key, Value>::FindElement(const Key& key) const
{
	int index = FindIndex(key);
	if (index >= 0){
		return &GetValueAt(index);
	}

	return NULL;
}


template <typename Key, typename Value>
typename const Value* TCascadedMap<Key, Value>::FindLocalElement(const Key& key) const
{
	int index = FindLocalIndex(key);
	if (index >= 0){
		return &GetLocalValueAt(index);
	}

	return NULL;
}


template <typename Key, typename Value>
typename Value* TCascadedMap<Key, Value>::FindLocalElement(const Key& key)
{
	int index = FindLocalIndex(key);
	if (index >= 0){
		return &GetLocalValueAt(index);
	}

	return NULL;
}


template <typename Key, typename Value>
const Key& TCascadedMap<Key, Value>::GetKeyAt(int index) const
{
	int elementsCount = GetLocalElementsCount();
	if (index < elementsCount){
		return GetLocalKeyAt(index);
	}

	I_ASSERT(m_parentPtr != NULL);	// Index from outside this map!

	return m_parentPtr->GetKeyAt(index - elementsCount);
}


template <typename Key, typename Value>
const Key& TCascadedMap<Key, Value>::GetLocalKeyAt(int index) const
{
	I_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].first;
}


template <typename Key, typename Value>
Key& TCascadedMap<Key, Value>::GetLocalKeyAt(int index)
{
	I_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].first;
}


template <typename Key, typename Value>
const Value& TCascadedMap<Key, Value>::GetValueAt(int index) const
{
	int elementsCount = GetLocalElementsCount();
	if (index < elementsCount){
		return GetLocalValueAt(index);
	}

	I_ASSERT(m_parentPtr != NULL);	// Index from outside this map!

	return m_parentPtr->GetValueAt(index - elementsCount);
}


template <typename Key, typename Value>
Value& TCascadedMap<Key, Value>::GetLocalValueAt(int index)
{
	I_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].second;
}


template <typename Key, typename Value>
const Value& TCascadedMap<Key, Value>::GetLocalValueAt(int index) const
{
	I_ASSERT(index < int(m_pairList.size()));

	return m_pairList[index].second;
}


template <typename Key, typename Value>
int TCascadedMap<Key, Value>::GetElementsCount() const
{
	if (m_parentPtr != NULL){
		return m_parentPtr->GetElementsCount() + GetLocalElementsCount();
	}
	else{
		return GetLocalElementsCount();
	}
}


template <typename Key, typename Value>
int TCascadedMap<Key, Value>::GetLocalElementsCount() const
{
	return int(m_pairList.size());
}


template <typename Key, typename Value>
bool TCascadedMap<Key, Value>::InsertLocal(const Key& key, const Value& value)
{
	IndexsMap::const_iterator iter = m_positionsMap.find(key);
	if (iter != m_positionsMap.end()){
		return false;
	}

	int newIndex = int(m_pairList.size());
	m_positionsMap[key] = newIndex;

	m_pairList.push_back(Pair(key, Value()));

	return true;
}


template <typename Key, typename Value>
const TCascadedMap<Key, Value>* TCascadedMap<Key, Value>::GetOwnerMap(int index) const
{
	int elementsCount = GetLocalElementsCount();
	if (index < elementsCount){
		return this;
	}

	I_ASSERT(m_parentPtr != NULL);	// Index from outside this map!

	return m_parentPtr->GetOwnerMap(index - elementsCount);
}


template <typename Key, typename Value>
TCascadedMap<Key, Value>* TCascadedMap<Key, Value>::GetOwnerMap(int index)
{
	int elementsCount = GetLocalElementsCount();
	if (index < elementsCount){
		return this;
	}

	I_ASSERT(m_parentPtr != NULL);	// Index from outside this map!

	return m_parentPtr->GetOwnerMap(index - elementsCount);
}


}//namespace istd


#endif // !istd_TCascadedMap_included


