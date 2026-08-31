// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/TDelPtr.h>
#include <istd/TInterfacePtr.h>
#include <istd/CClassInfo.h>
#include <icomp/IComponent.h>
#include <icomp/IComponentStaticInfo.h>
#include <icomp/IComponentContext.h>
#include <icomp/CCompositeComponent.h>


namespace ipackage
{

	
/**
	A helper class to access public interfaces of a composite component.
*/
class CComponentAccessor
{
public:
	/**
		Constructor.
		
		\param	registryFile	The registry file.
		\param	configFile  	(Optional) the configuration file.
	*/
	explicit CComponentAccessor(
				const QString& registryFile,
				const QString& configFile = QString(),
				bool isDiagnosticEnabled = false,
				bool manualAutoInit = false);

	/**
		Gets component interface.
	
		\tparam	InterfaceType	Type of the interface.
		\param	componentId	(Optional) identifier for the component.
		
		\return	null if it fails, else the requested component interface.
	*/
	template <class InterfaceType>
	InterfaceType* GetComponentInterface(const QByteArray& componentId = QByteArray());

	/**
		Creates a new instance of a registry element marked as "use for factorization" (detached element).

		The created component gets its own component context, so all its references are resolved
		using the loaded registry exactly like for automatically created components.

		\param	componentId	identifier of the registry element used as component template.

		\return	owning pointer to the created component, or an empty pointer if it fails.
	*/
	icomp::IComponentUniquePtr CreateComponentInstance(const QByteArray& componentId);

	/**
		Creates a new instance of a registry element marked as "use for factorization" (detached element)
		and extracts one of its interfaces.

		The ownership of the created component is transferred to the returned pointer.

		\tparam	InterfaceType	type of the extracted interface.
		\param	componentId	identifier of the registry element used as component template.
		\param	subId	optional ID identifying a subelement of the created component.

		\return	owning pointer to the extracted interface, or an empty pointer if it fails.
	*/
	template <class InterfaceType>
	istd::TUniqueInterfacePtr<InterfaceType> CreateComponentInstance(
				const QByteArray& componentId,
				const QByteArray& subId = QByteArray());

	bool EnsureAutoInitComponentsCreated() const;

private:
	icomp::CCompositeComponent m_mainComponent;

	istd::TDelPtr<icomp::IComponentStaticInfo> m_mainComponentStaticInfoPtr;
	icomp::IComponentContextSharedPtr m_mainComponentContextPtr;
};


template <class InterfaceType>
InterfaceType* CComponentAccessor::GetComponentInterface(const QByteArray& componentId)
{
	InterfaceType* interfacePtr = m_mainComponent.GetComponentInterface<InterfaceType>(componentId);

	return interfacePtr;
}


template <class InterfaceType>
istd::TUniqueInterfacePtr<InterfaceType> CComponentAccessor::CreateComponentInstance(
		const QByteArray& componentId,
		const QByteArray& subId)
{
	icomp::IComponentUniquePtr componentPtr = CreateComponentInstance(componentId);
	if (componentPtr != nullptr){
		InterfaceType* interfacePtr = static_cast<InterfaceType*>(
					componentPtr->GetInterface(istd::CClassInfo::GetInfo<InterfaceType>(), subId));
		if (interfacePtr != NULL){
			return istd::TUniqueInterfacePtr<InterfaceType>(componentPtr.release(), interfacePtr);
		}
	}

	return istd::TUniqueInterfacePtr<InterfaceType>();
}


} // namespace ipackage

