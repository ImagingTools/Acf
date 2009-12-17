#ifndef ibase_IObjectSnap_included
#define ibase_IObjectSnap_included


#include "istd/IChangeable.h"

#include "istd/CIndex2d.h"

#include "iimg/IBitmap.h"


namespace ibase
{


/**
	Interface for getting a bitmap snap of any object.
*/
class IObjectSnap: virtual public istd::IPolymorphic
{
public:
	/**
		Get bitmap snap for the data object \c data.
	*/
	virtual bool GetSnap(
				const istd::IChangeable& data,
				iimg::IBitmap& objectSnap,
				const istd::CIndex2d& size) const = 0;
};


} // namespace ibase


#endif // !ibase_IObjectSnap_included


