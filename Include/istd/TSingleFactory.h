#ifndef istd_TSingleFactory_included
#define istd_TSingleFactory_included


#include "istd/istd.h"


#include <string>


#include "istd/TIFactory.h"


namespace istd
{


/**
	Template based object factory interface.
*/
template <class Interface, class Implementation, class Key = std::string>
class TSingleFactory: virtual public TIFactory<Interface, Key>
{
public:
	typedef Implementation ImplementationType;
	typedef TIFactory<Interface, Key> FactoryInterface;

	explicit TSingleFactory(const Key& key);

	// reimplemented (istd::IFactoryInfo)
	virtual IFactoryInfo::KeyList GetFactoryKeys() const;

	// reimplemented (istd::TIFactory)
	virtual Interface* CreateInstance(const Key* keyPtr = NULL) const;

private:
	Key m_key;
};


// public methods

template <class Interface, class Implementation, class Key>
TSingleFactory<Interface, Implementation, Key>::TSingleFactory(const Key& key)
:	m_key(key)
{
}


// reimplemented (istd::IFactoryInfo)

template <class Interface, class Implementation, class Key>
IFactoryInfo::KeyList TSingleFactory<Interface, Implementation, Key>::GetFactoryKeys() const
{
	typename TIFactory<Interface, Key>::KeyList retVal;

	retVal.push_back(m_key);

	return retVal;
}


// reimplemented (istd::TIFactory)

template <class Interface, class Implementation, class Key>
Interface* TSingleFactory<Interface, Implementation, Key>::CreateInstance(const Key* keyPtr) const
{
	if ((keyPtr == NULL) || (*keyPtr == m_key)){
		return new Implementation;
	}

	return NULL;
}


} // namespace istd


#endif // !istd_TSingleFactory_included


