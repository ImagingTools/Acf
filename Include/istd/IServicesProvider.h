#ifndef istd_IServicesProvider_included
#define istd_IServicesProvider_included


#include "istd/IPolymorphic.h"
#include "istd/CClassInfo.h"


namespace istd
{


/**
	Allow access to service implementation.
	This interface is used to provide cascade of service implementation distributed in several modules.
*/
class IServicesProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get pointer to service implementation.
		If implementation for specified ID is not available, it returns NULL.
	*/
	virtual void* GetService(const istd::CClassInfo& serviceId) const = 0;
	/**
		Create new service instance.
	*/
	virtual void* CreateService(const istd::CClassInfo& info) const = 0;
};


} // namespace istd


#endif // !istd_IServicesProvider_included


