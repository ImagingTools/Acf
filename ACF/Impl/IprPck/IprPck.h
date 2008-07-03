#ifndef IprPck_included
#define IprPck_included


#include "icomp/TMakeComponentWrap.h"

#include "iipr/CMovingAverageProcessorComp.h"
#include "iipr/CRectangularFilterParams.h"

#include "iipr/CProcessedAcquisitionComp.h"


/**
	system-undependent image processing package.
*/
namespace IprPck
{


	typedef iipr::CMovingAverageProcessorComp MovingAverageProcessor;
	typedef icomp::TMakeComponentWrap<iser::ISerializable, iipr::CRectangularFilterParams> RectangularFilterParams;
	typedef iipr::CProcessedAcquisitionComp ProcessedCamera;


} // namespace IprPck


#endif // !IprPck_included


