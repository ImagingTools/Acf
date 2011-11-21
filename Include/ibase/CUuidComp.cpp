#include "ibase/CUuidComp.h"


#include "istd/CStaticServicesProvider.h"

#include "isys/ISystemEnvironment.h"


namespace ibase
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CUuidComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	isys::ISystemEnvironment* systemEnvironmentPtr = istd::GetService<isys::ISystemEnvironment>();
	if (systemEnvironmentPtr != NULL){
		SetName(systemEnvironmentPtr->GetUniqueIdentifier());
	}
}


} // namespace ibase


