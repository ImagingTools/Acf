#ifndef icmpstr_IRegistryPreview_included
#define icmpstr_IRegistryPreview_included


#include "istd/IChangeable.h"

#include "icomp/IRegistry.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


class IRegistryPreview: virtual public istd::IChangeable
{
public:
	/**
		Start registry execution.
	*/
	virtual bool StartRegistry(const icomp::IRegistry& registry) = 0;

	/**
		Returns \c true if the registry \c registry is executing.
	*/
	virtual bool IsRunning() const = 0;

	/**
		Abort registry execution.
	*/
	virtual void AbortRegistry() = 0;
};


} // namespace icmpstr


#endif // !icmpstr_IRegistryPreview_included

