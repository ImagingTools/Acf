#ifndef iipr_IBitmapProcessor_included
#define iipr_IBitmapProcessor_included


#include "iimg/IBitmap.h"

#include "iproc/TIAssyncProcessor.h"


namespace iipr
{


/**
	Bitmap conversion processor.
*/
class IBitmapProcessor: virtual public iproc::TIAssyncProcessor<iimg::IBitmap, iimg::IBitmap>
{
};


} // namespace iipr


#endif // !iipr_IBitmapProcessor_included

