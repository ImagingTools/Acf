#ifndef iproc_IBitmapSupplier_included
#define iproc_IBitmapSupplier_included


#include "istd/CIndex2d.h"

#include "iimg/IBitmap.h"

#include "iproc/ISupplier.h"


namespace iproc
{


/**
	Bitmap supplier allowing to access of produced bitmap.
*/
class IBitmapSupplier: virtual public iproc::ISupplier
{
public:
	/**
		Get access to produced bitmap object.
		\return	pointer to bitmap instance if this bitmap is accessible, or NULL.
	*/
	virtual const iimg::IBitmap* GetBitmap() const = 0;
};


} // namespace iproc


#endif // !iproc_IBitmapSupplier_included

