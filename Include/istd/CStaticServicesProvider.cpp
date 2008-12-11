#include "istd/CStaticServicesProvider.h"


namespace istd
{


void CStaticServicesProvider::SetParent(const IServicesProvider* parentPtr)
{
	m_parentPtr = parentPtr;
}


bool CStaticServicesProvider::RegisterService(const istd::CClassInfo& serviceId, void* servicePtr)
{
	std::pair<Services::iterator, bool> status = m_registeredServices.insert(std::make_pair(serviceId, servicePtr));

	return status.second;
}


void* CStaticServicesProvider::GetService(const istd::CClassInfo& serviceId)
{
	Services::const_iterator iter = m_registeredServices.find(serviceId);

	if (iter != m_registeredServices.end()){
		return iter->second;
	}
	else if(m_parentPtr != NULL){
		return m_parentPtr->GetService(serviceId);
	}

	return NULL;
}


IServicesProvider& CStaticServicesProvider::GetProviderInstance()
{
	return m_providerInstance;
}


// public methods of embedded class IServicesProvider

// reimplemented (istd::IServicesProvider)

void* CStaticServicesProvider::Provider::GetService(const istd::CClassInfo& serviceId) const
{
	return CStaticServicesProvider::GetService(serviceId);
}


// static attributes

CStaticServicesProvider::Services CStaticServicesProvider::m_registeredServices;
CStaticServicesProvider::Provider CStaticServicesProvider::m_providerInstance;
const IServicesProvider* CStaticServicesProvider::m_parentPtr(NULL);


} // namespace istd


