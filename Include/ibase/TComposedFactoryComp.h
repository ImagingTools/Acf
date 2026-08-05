// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/TIFactory.h>
#include <icomp/CComponentBase.h>


namespace ibase
{

template <class Interface>
class TComposedFactoryComp: public icomp::CComponentBase, public istd::TIFactory<Interface>
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(TComposedFactoryComp);
		I_REGISTER_INTERFACE(istd::TIFactory<Interface>);
		I_ASSIGN_MULTI_0(m_slaveFactoriesCompPtr, "SlaveFactories", "Slave factories", true);
	I_END_COMPONENT;

	// reimplemented (istd::TIFactory)
	virtual istd::TUniqueInterfacePtr<Interface> CreateInstance(const QByteArray& keyId = "") const override;
	
	// reimplemented (istd::IFactoryInfo)
	virtual istd::IFactoryInfo::KeyList GetFactoryKeys() const override;

private:
	I_TMULTIREF( istd::TIFactory<Interface>, m_slaveFactoriesCompPtr);
};


// protected methods

// reimplemented (istd::TIFactory)

template <class Interface>
istd::TUniqueInterfacePtr<Interface> TComposedFactoryComp<Interface>::CreateInstance(const QByteArray& keyId) const
{
	for (int i = 0; i < m_slaveFactoriesCompPtr.GetCount(); ++i) {
		auto factoryPtr = m_slaveFactoriesCompPtr[i];
		if (factoryPtr != nullptr && (keyId.isEmpty() || factoryPtr->GetFactoryKeys().contains(keyId))) {
			istd::TUniqueInterfacePtr<Interface> createdPtr = factoryPtr->CreateInstance(keyId);

			if (createdPtr.IsValid()){
				return createdPtr;
			}
		}
	}

	return istd::TUniqueInterfacePtr<Interface>();
}


// reimplemented (istd::IFactoryInfo)

template <class Interface>
istd::IFactoryInfo::KeyList TComposedFactoryComp<Interface>::GetFactoryKeys() const
{
	istd::IFactoryInfo::KeyList retVal;
	for (int i = 0; i < m_slaveFactoriesCompPtr.GetCount(); ++i) {
		auto factoryPtr = m_slaveFactoriesCompPtr[i];
		if (factoryPtr != nullptr) {
			retVal += factoryPtr->GetFactoryKeys();
		}
	}

	return retVal;
}


} // namespace ibase


