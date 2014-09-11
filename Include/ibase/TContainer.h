#ifndef ibase_TContainer_included
#define ibase_TContainer_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include "istd/IContainerInfo.h"
#include "istd/CChangeNotifier.h"


namespace ibase
{


/**
	Common implementation of an abstract container. 
*/
template <typename ItemClass>
class TContainer: virtual public istd::IContainerInfo
{
public:
	enum ChangeFlags
	{
		CF_ELEMENT_ADDED = 0x382b230,
		CF_ELEMENT_REMOVED,
		CF_RESET
	};

	const ItemClass& GetAt(int index) const;
	ItemClass& GetAt(int index);
	ItemClass& PushBack(const ItemClass& item);
	ItemClass& PushFront(const ItemClass& item);
	ItemClass& InsertAt(const ItemClass& item, int index);
	void PopBack();
	void PopFront();
	void RemoveAt(int index);
	virtual void Reset();

	TContainer& operator=(const TContainer& container);

	// reimplemented (istd::IContainerInfo)
	virtual int GetItemsCount() const;
	virtual bool IsEmpty() const;
	virtual bool IsIndexValid(int index) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

protected:
    typedef QList<ItemClass> Items;

	Items m_items;
};


template <typename ItemClass>
ItemClass& TContainer<ItemClass>::GetAt(int index)
{
	Q_ASSERT(TContainer<ItemClass>::IsIndexValid(index));

	return m_items[index];
}


template <typename ItemClass>
const ItemClass& TContainer<ItemClass>::GetAt(int index) const
{
	Q_ASSERT(TContainer<ItemClass>::IsIndexValid(index));

	return m_items[index];
}


template <typename ItemClass>
ItemClass& TContainer<ItemClass>::PushBack(const ItemClass& item)
{
	static ChangeSet changeSet(CF_ELEMENT_ADDED);
 	BeginChanges(changeSet);

	m_items.append(item);

	EndChanges(changeSet);

	return m_items.back();
}


template <typename ItemClass>
ItemClass& TContainer<ItemClass>::PushFront(const ItemClass& item)
{
	static ChangeSet changeSet(CF_ELEMENT_ADDED);
	BeginChanges(changeSet);

	m_items.prepend(item);

	EndChanges(changeSet);

	return m_items.front();
}


template <typename ItemClass>
ItemClass& TContainer<ItemClass>::InsertAt(const ItemClass& item, int index)
{
	static ChangeSet changeSet(CF_ELEMENT_ADDED);
	BeginChanges(changeSet);

	if ((index < 0) || (index >= m_items.size())){
		m_items.append(item);

 		EndChanges(changeSet);

		return m_items.back();
	}
	else{
		m_items.insert(index, item);

		EndChanges(changeSet);

		return m_items[index];
	}
}


template <typename ItemClass>
void TContainer<ItemClass>::PopBack()
{
	static ChangeSet changeSet(CF_ELEMENT_REMOVED);
	BeginChanges(changeSet);

	m_items.pop_back();

	EndChanges(changeSet);
}


template <typename ItemClass>
void TContainer<ItemClass>::PopFront()
{
	static ChangeSet changeSet(CF_ELEMENT_REMOVED);
	BeginChanges(changeSet);

	m_items.pop_front();

	EndChanges(changeSet);
}


template <typename ItemClass>
void TContainer<ItemClass>::RemoveAt(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_items.size()));

	if (index < int(m_items.size())){
		static ChangeSet changeSet(CF_ELEMENT_REMOVED);
		BeginChanges(changeSet);
	
		m_items.erase(m_items.begin()  + index);

		EndChanges(changeSet);
	}
}


template <typename ItemClass>
void TContainer<ItemClass>::Reset()
{
	static ChangeSet changeSet(CF_RESET, CF_ALL_DATA);
	BeginChanges(changeSet);

	m_items.clear();

	EndChanges(changeSet);
}


template <typename ItemClass>
TContainer<ItemClass>& TContainer<ItemClass>::operator=(const TContainer& container)
{
	m_items = container.m_items;

	return *this;
}


// reimplemented (istd::IContainerInfo)

template <typename ItemClass>
int TContainer<ItemClass>::GetItemsCount() const
{
	return int(m_items.size());
}


template <typename ItemClass>
bool TContainer<ItemClass>::IsEmpty() const
{
	return m_items.isEmpty();
}


template <typename ItemClass>
bool TContainer<ItemClass>::IsIndexValid(int index) const
{
	return (index >= 0 && index < int(m_items.size()));
}


// reimplemented (istd::IChangeable)

template <typename ItemClass>
bool TContainer<ItemClass>::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const TContainer* containerPtr = dynamic_cast<const TContainer*>(&object);
	if (containerPtr != NULL){
		m_items = containerPtr->m_items;

		return true;
	}

	return false;
}


} // namespace ibase


#endif // !ibase_TContainer_included


