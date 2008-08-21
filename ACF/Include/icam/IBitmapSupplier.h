#ifndef icam_IBitmapSupplier_included
#define icam_IBitmapSupplier_included


#include "istd/CIndex2d.h"

#include "iimg/IBitmap.h"

#include "iproc/ISupplier.h"


namespace icam
{


/**
	Bitmap supplier allowing to access of produced bitmap.
*/
class IBitmapSupplier: virtual public iproc::ISupplier
{
public:
	/**
		Get access to produced bitmap object.
		\param	objectId	ID of inspected object. \sa	iproc::ISupplier::BeginNextObject.
		\return	pointer to bitmap instance if this bitmap is accessible, or NULL.
	*/
	virtual const iimg::IBitmap* GetBitmap(I_DWORD objectId) const = 0;
};


} // namespace icam


#endif // !icam_IBitmapSupplier_included

