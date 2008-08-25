#ifndef iipr_IMultidimensionalFilterParams_included
#define iipr_IMultidimensionalFilterParams_included


#include "iser/ISerializable.h"

#include "imath/TVarVector.h"


namespace iipr
{


class IMultidimensionalFilterConstraints;


class IMultidimensionalFilterParams: virtual public iser::ISerializable
{
public:
	/**
		Get parameter constraints describing possible parameter values.
	*/
	virtual const IMultidimensionalFilterConstraints* GetConstraints() const = 0;

	/**
		Get fileter length for each dimension.
	*/
	virtual imath::CVarVector GetFilterLengths() const = 0;

	/**
		Set filter length for each dimension.
	*/
	virtual bool SetFilterLengths(const imath::CVarVector& lengths) = 0;
};


} // namespace iipr


#endif // !iipr_IMultidimensionalFilterParams_included


