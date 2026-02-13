// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <istd/TIFactory.h>


namespace istd
{


/**
	Template based object factory interface.
*/
template <class Interface, class Implementation>
class TSingleFactory: virtual public TIFactory<Interface>
{
public:
	typedef Implementation ImplementationType;
	typedef TIFactory<Interface> FactoryInterface;

	explicit TSingleFactory(const QByteArray& keyId);

	// reimplemented (istd::IFactoryInfo)
	virtual IFactoryInfo::KeyList GetFactoryKeys() const override;

	// reimplemented (istd::TIFactory)
	virtual istd::TUniqueInterfacePtr<Interface> CreateInstance(const QByteArray& keyId = "") const override;

private:
	QByteArray m_keyId;
};


// public methods

template <class Interface, class Implementation>
TSingleFactory<Interface, Implementation>::TSingleFactory(const QByteArray& keyId)
:	m_keyId(keyId)
{

}


// reimplemented (istd::TIFactory)

template <class Interface, class Implementation>
IFactoryInfo::KeyList TSingleFactory<Interface, Implementation>::GetFactoryKeys() const
{
	typename TIFactory<Interface>::KeyList retVal;

	retVal.insert(m_keyId);

	return retVal;
}


template <class Interface, class Implementation>
istd::TUniqueInterfacePtr<Interface> TSingleFactory<Interface, Implementation>::CreateInstance(const QByteArray& keyId) const
{
	if (keyId.isEmpty() || (keyId == m_keyId)){
		return istd::TUniqueInterfacePtr<Interface>(new Implementation);
	}

	return istd::TUniqueInterfacePtr<Interface>();
}


} // namespace istd




