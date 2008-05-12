#ifndef iproc_IProgressInfo_included
#define iproc_IProgressInfo_included


#include "istd/IPolymorphic.h"

#include "iproc/iproc.h"


namespace iproc
{


/**
	Common inteface for processing progress.
*/
class IProgressInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Returns processing progress.
	*/
	virtual double GetProgress() const = 0;
};


} // namespace iproc


#endif // !iproc_IProgressInfo_included

