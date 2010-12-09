#ifndef icomp_CComponentAccessor_included
#define icomp_CComponentAccessor_included


#include "icomp/CCompositeComponent.h"


namespace icomp
{


class CComponentAccessor
{
public:
	explicit CComponentAccessor(
				const istd::CString& registryFile, 
				const istd::CString& configFile = istd::CString());

	template <class InterfaceType>
	InterfaceType* GetComponentInterface(const std::string& componentId = std::string());

private:
	istd::CString m_registryFile;

	icomp::CCompositeComponent m_composite;

	bool m_isAutoInitBlocked;
};


template <class InterfaceType>
InterfaceType* CComponentAccessor::GetComponentInterface(const std::string& componentId)
{
	InterfaceType* interfacePtr = m_composite.GetComponentInterface<InterfaceType>(componentId);

	if ((interfacePtr != NULL) && m_isAutoInitBlocked){
		m_composite.EndAutoInitBlock();

		m_isAutoInitBlocked = false;
	}

	return interfacePtr;
}


} // namespace icomp


#endif // !icomp_CComponentAccessor_included


