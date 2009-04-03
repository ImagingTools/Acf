#include "istd/CStaticServicesProvider.h"


namespace istd
{


void CStaticServicesProvider::InitServices(const IServicesProvider* parentPtr)
{
	if (s_parentPtr == NULL){
		s_parentPtr = parentPtr;
	}
}


bool CStaticServicesProvider::RegisterService(const istd::CClassInfo& info, void* servicePtr)
{
	std::pair<Services::iterator, bool> status = s_registeredServices.insert(std::make_pair(info, servicePtr));

	return status.second;
}


bool CStaticServicesProvider::RegisterFactory(const istd::CClassInfo& info, const IVoidFactory* factoryPtr)
{
	std::pair<Factories::iterator, bool> status = s_registeredFactories.insert(std::make_pair(info, factoryPtr));

	return status.second;
}


void* CStaticServicesProvider::GetService(const istd::CClassInfo& info)
{
	Services::const_iterator iter = s_registeredServices.find(info);

	if (iter != s_registeredServices.end()){
		return iter->second;
	}
	else if(s_parentPtr != NULL){
		return s_parentPtr->GetService(info);
	}

	return NULL;
}


void* CStaticServicesProvider::CreateService(const istd::CClassInfo& info)
{
	Factories::const_iterator iter = s_registeredFactories.find(info);

	if (iter != s_registeredFactories.end()){
		I_ASSERT(iter->second != NULL);		// No NULL-factory could be registered

		return iter->second->CreateInstance();
	}
	else if(s_parentPtr != NULL){
		return s_parentPtr->CreateService(info);
	}

	return NULL;
}


IServicesProvider& CStaticServicesProvider::GetProviderInstance()
{
	return s_providerInstance;
}


// public methods of embedded class IServicesProvider

// reimplemented (istd::IServicesProvider)

void* CStaticServicesProvider::Provider::GetService(const istd::CClassInfo& info) const
{
	return CStaticServicesProvider::GetService(info);
}


void* CStaticServicesProvider::Provider::CreateService(const istd::CClassInfo& info) const
{
	return CStaticServicesProvider::CreateService(info);
}


// static attributes

CStaticServicesProvider::Services CStaticServicesProvider::s_registeredServices;
CStaticServicesProvider::Factories CStaticServicesProvider::s_registeredFactories;
CStaticServicesProvider::Provider CStaticServicesProvider::s_providerInstance;
const IServicesProvider* CStaticServicesProvider::s_parentPtr(NULL);


} // namespace istd


