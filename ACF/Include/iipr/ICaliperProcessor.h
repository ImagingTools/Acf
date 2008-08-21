#ifndef iipr_ICaliperProcessor_included
#define iipr_ICaliperProcessor_included


#include "iproc/IProcessor.h"

#include "iipr/IFeaturesConsumer.h"
#include "iipr/CProjectionData.h"


namespace iipr
{


class ICaliperParams;


class ICaliperProcessor: virtual public iproc::IProcessor
{
public:
	virtual bool DoCaliper(
				const CProjectionData& projection,
				const ICaliperParams* paramsPtr,
				IFeaturesConsumer& results) = 0;
};


} // namespace iipr


#endif // !iipr_ICaliperProcessor_included


