#include "icomp/export.h"

#include "MilPck.h"


namespace MilPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("Mil", "MIL Package", "MIL");

I_EXPORT_COMPONENT(MilSearchParamsGui, "Search params GUI", "Search parameter GUI MIL");
I_EXPORT_COMPONENT(MilSearchParams, "Search parameters for MIL search", "Search parameter GUI MIL");
I_EXPORT_COMPONENT(MilSearchProcessor, "Search processor", "Search parameter GUI MIL");


} // namespace MilPck
