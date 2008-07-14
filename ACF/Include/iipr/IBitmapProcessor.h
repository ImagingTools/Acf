#ifndef iipr_IBitmapProcessor_included
#define iipr_IBitmapProcessor_included


#include "iimg/IBitmap.h"

#include "iproc/TIProcessor.h"


namespace iipr
{


/**
	Bitmap conversion processor.
*/
class IBitmapProcessor: virtual public iproc::TIProcessor<iimg::IBitmap, iimg::IBitmap>
{
};


} // namespace iipr


#endif // !iipr_IBitmapProcessor_included

