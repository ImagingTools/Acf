#ifndef iipr_CImageManipulation_included
#define iipr_CImageManipulation_included


#include "iimg/IBitmap.h"

#include "ibase/CSize.h"


namespace iipr
{


class CImageManipulation
{
public:
	static bool GetExpanded(const iimg::IBitmap& inputBitmap, iimg::IBitmap& outputBitmap, const ibase::CSize& size);
	static bool GetShrinked(const iimg::IBitmap& inputBitmap, iimg::IBitmap& outputBitmap, const ibase::CSize& size);
};


} // namespace iipr


#endif // !iipr_CImageManipulation_included

