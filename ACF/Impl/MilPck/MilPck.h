#ifndef MilPck_included
#define MilPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "imil/CMilSearchParamsGuiComp.h"
#include "imil/CMilSearchProcessorComp.h"


/**
	MIL package.
*/
namespace MilPck
{


typedef imil::CMilSearchParamsGuiComp MilSearchParamsGui;
typedef icomp::TMakeComponentWrap<imod::TModelWrap<imil::CMilSearchParams>, iser::ISerializable, iipr::ISearchParams> MilSearchParams;
typedef imil::CMilSearchProcessorComp MilSearchProcessor;


} // namespace MilPck


#endif // !MilPck_included


