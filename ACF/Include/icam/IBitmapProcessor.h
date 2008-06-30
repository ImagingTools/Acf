#ifndef icam_IBitmapProcessor_included
#define icam_IBitmapProcessor_included


#include "iimg/IBitmap.h"

#include "iproc/TIAssyncProcessor.h"


namespace icam
{


/**
	Bitmap conversion processor.
*/
class IBitmapProcessor: virtual public iproc::TIAssyncProcessor<iimg::IBitmap, iimg::IBitmap>
{
public:
};


} // namespace icam


#endif // !icam_IBitmapProcessor_included


