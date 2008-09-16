#ifndef iipr_ISearchModel_included
#define iipr_ISearchModel_included


#include "iimg/IBitmap.h"

#include "iser/ISerializable.h"

#include "iipr/ISearchParams.h"


namespace iipr
{


/**
	Common interface for all search model implementations for pattern matching.
*/
class ISearchModel: virtual public iser::ISerializable
{
public:
	/**
		Creates search model from the image. 
	*/
	virtual bool CreateFromImage(const iimg::IBitmap& modelImage, const iipr::ISearchParams* params) = 0;

	/**
		Returns a \c true, if the model was created and is ready for use, otherwise a \c false.
	*/
	virtual bool IsValid() const = 0;

	/**
		Resets the model object. All model data will be cleared.
	*/
	virtual void ResetModel() = 0;

	/**	
		Gets the model image.
	*/
	virtual const iimg::IBitmap& GetImage() const = 0;
};


} // namespace iipr


#endif // !iipr_ISearchModel_included

