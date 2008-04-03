#ifndef iproc_IProcessingEventHandler_included
#define iproc_IProcessingEventHandler_included


#include "istd/IPolymorphic.h"


namespace iproc
{


class IProcessingEventHandler: virtual public istd::IPolymorphic
{
public:
	virtual void OnPause() = 0;
	virtual void OnResume() = 0;
	virtual void OnAbort() = 0;
	virtual void OnFinish() = 0;
};


} // namespace iproc


#endif // !iproc_IProcessingEventHandler_included
