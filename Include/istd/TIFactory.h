// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IFactoryInfo.h>
#include <istd/TInterfacePtr.h>


namespace istd
{


/**
	Generic interface for a factory.
*/
template <class Interface>
class TIFactory: virtual public IFactoryInfo
{
public:
	typedef Interface InterfaceType;

	/**
		Create an instance of the object, mapped to the keyId \c keyId.
		\param	keyId		
		\return			unique pointer to created object or empty pointer if it was not possible to create it or keyId does not exist.
	*/
	virtual istd::TUniqueInterfacePtr<Interface> CreateInstance(const QByteArray& keyId = "") const = 0;
};


typedef TIFactory<void> IVoidFactory;
typedef TIFactory<istd::IPolymorphic> IFactory;


} // namespace istd



