#ifndef iipr_IMultidimensionalFilterConstraints_included
#define iipr_IMultidimensionalFilterConstraints_included


#include "istd/IPolymorphic.h"


namespace iipr
{


class IMultidimensionalFilterConstraints: virtual public istd::IPolymorphic
{
public:
	/**
		Get number of dimensions will be filtered.
	*/
	virtual int GetFilterDimensionsCount() const = 0;

	/**
		Get range of possible filter value for specified dimension.
	*/
	virtual istd::CRange GetFilterLengthRange(int dimension) const = 0;

	/**
		Check if filter length for specified dimension is integral.
		If this length is integral filter length value will be rounded to interger value.
	*/
	virtual bool IsFilterLengthIntegral(int dimension) const = 0;
}


} // namespace iipr


#endif // !iipr_IMultidimensionalFilterConstraints_included


