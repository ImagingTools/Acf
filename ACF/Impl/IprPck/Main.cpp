#include "icomp/export.h"

#include "IprPck.h"


namespace IprPck
{


I_EXPORT_PACKAGE("Ipr", "Image processing package", "Image Processing");

I_EXPORT_COMPONENT(MovingAverageProcessor, "Moving average image processor", "Image Processing");
I_EXPORT_COMPONENT(RectangularFilterParams, "Rectangular filter parameter", "Filter Image Processing Parameter");
I_EXPORT_COMPONENT(LocalDifferenceProcessor, "Line difference image processor", "Image Processing");
I_EXPORT_COMPONENT(LocalDifferenceFilterParams, "Line difference processor params", "Filter Image Processing Parameter");
I_EXPORT_COMPONENT(ProcessedCamera, "Processed Camera", "Image Bitmap Acquisition Snap Parameters Camera Processing");


} // namespace IprPck


