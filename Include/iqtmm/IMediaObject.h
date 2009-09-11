#ifndef iqtmm_IMediaObject_included
#define iqtmm_IMediaObject_included


// ACF includes
#include "istd/IChangeable.h"


namespace iqtmm
{


/**
	Common interface for a medium object.
*/
class IMediaObject: virtual public istd::IChangeable
{
public:
	/**
		Get URL of the medium object.
	*/
	virtual QString GetMediumUrl() const = 0;

	/**
		Set URL of the medium object.
	*/
	virtual void SetMediumUrl(const QString& mediumUrl) = 0;
};


} // namespace iqtmm


#endif // !iqtmm_IMediaObject_included


