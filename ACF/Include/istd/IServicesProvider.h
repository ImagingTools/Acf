#ifndef istd_IServicesProvider_h_included
#define istd_IServicesProvider_h_included


#include "istd/IPolymorphic.h"


namespace istd
{


/**
Allow access to service implementation.
 */
class IServicesProvider: public IPolymorphic
{
public:
	/**
	Get pointer to service implementation.
	If implementation for specified ID is not available, it returns NULL.
	*/
	virtual void* GetService(const type_info& serviceId) const = 0;
};


} // namespace istd


#endif // !istd_IServicesProvider_h_included


