#ifndef iipr_IMultidimensionalFilterConstraints_included
#define iipr_IMultidimensionalFilterConstraints_included


#include "istd/IPolymorphic.h"
#include "istd/CRange.h"

#include "imath/TIValueManip.h"

#include "iipr/iipr.h"


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
		Return number manipulator object for filter length value.
	*/
	virtual const imath::IDoubleManip& GetFilterLengthManip(int dimension) const = 0;
};


} // namespace iipr


#endif // !iipr_IMultidimensionalFilterConstraints_included


