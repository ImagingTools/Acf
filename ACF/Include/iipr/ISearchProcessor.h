#ifndef iipr_ISearchProcessor_included
#define iipr_ISearchProcessor_included


#include "iimg/IBitmap.h"

#include "iproc/TIAssyncProcessor.h"

#include "iipr/ISearchResult.h"


namespace iipr
{


/**
	Interface for the shape based pattern matching processor.
*/
class ISearchProcessor: virtual public iproc::TIAssyncProcessor<iimg::IBitmap, iipr::ISearchResult>
{
};


} // namespace qstd


#endif // !iipr_ISearchProcessor_included

