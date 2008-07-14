#ifndef iipr_IProjectionConstraints_included
#define iipr_IProjectionConstraints_included


#include "istd/IPolymorphic.h"
#include "istd/CRange.h"


namespace iipr
{


class IProjectionConstraints: virtual public istd::IPolymorphic
{
public:
	/**
		Get range of possible projection line width.
		If this range is invalid, no width setting is suported.
	*/
	virtual istd::CRange GetLineWidthRange() const = 0;

	/**
		Get minimal number of projection elements can be calculated by projection.
		If this value is bigger than returned by \c GetMaxProjectionSize() there is no fixed size supported.
	*/
	virtual int GetMinProjectionSize() const = 0;

	/**
		Get maximal number of projection elements can be calculated by projection.
		If this value is smaller than returned by \c GetMinProjectionSize() there is no fixed size supported.
	*/
	virtual int GetMaxProjectionSize() const = 0;

	/**
		Check if automatical projection size is supported.
	*/
	virtual bool IsAutoProjectionSizeSupported() const = 0;
};


} // namespace iipr


#endif // !iipr_IProjectionConstraints_included


