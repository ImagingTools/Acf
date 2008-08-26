#ifndef iipr_IFeaturesMapper_included
#define iipr_IFeaturesMapper_included


#include "imath/TVarVector.h"

#include "iprm/IParamsSet.h"


namespace iipr
{


class IFeature;


/**
	Interface allowing mapping of extracted features to native coordinate systems.
*/
class IFeaturesMapper: virtual public istd::IPolymorphic
{
public:
	/**
		Get position in image coordination system using position extracted from projection.
	*/
	virtual imath::CVarVector GetImagePosition(
				const IFeature& feature,
				const iprm::IParamsSet* paramsPtr) const = 0;
};


} // namespace iipr


#endif // !iipr_IFeaturesMapper_included


