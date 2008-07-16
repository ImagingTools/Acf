#ifndef iipr_ISearchResultSet_included
#define iipr_ISearchResultSet_included


#include "iipr/iipr.h"


#include "istd/IPolymorphic.h"

#include "i2d/CTransform.h"
#include "i2d/CVector2d.h"


namespace iipr
{


/**	
	Interface for search results container.
*/
class ISearchResultSet: virtual public istd::IPolymorphic
{
public:
	/*
		Returns number of matches or number of found models.
	*/
	virtual int GetMatchesCount() const = 0;

	/*
		Returns the time need for search in ms.
	*/
	virtual double GetTime() const = 0;

	/*
		Returns match result as a transform object.
	*/
	virtual i2d::CTransform GetTransform(int index) const = 0;

	/*
		Returns the scale factors for x- and y-axes of the found model with the index \c index.
	*/
	virtual i2d::CVector2d GetScale(int index) const = 0;

	/*
		Returns the angle of the found model with the index \c index.
	*/
	virtual double GetAngle(int index) const = 0;

	/*
		Returns the position of the found model with the index \c index.
	*/
	virtual i2d::CVector2d GetPosition(int index) const = 0;
		
	/*
		Returns number of matches or number of found models.
	*/
	virtual double GetScore(int index) const = 0;

	/*
		Clears internal data.
	*/
	virtual void ResetResult() = 0;
};


} // namespace iipr


#endif // !iipr_ISearchResultSet_included

