#ifndef IprPck_included
#define IprPck_included


#include "imod/TModelWrap.h"

#include "icomp/TMakeComponentWrap.h"

#include "iipr/CMovingAverageProcessorComp.h"
#include "iipr/CRectangularFilterParams.h"
#include "iipr/CProcessedAcquisitionComp.h"
#include "iipr/CLocalDifferenceProcessorComp.h"
#include "iipr/CLocalDifferenceFilterParams.h"


/**
	system-undependent image processing package.
*/
namespace IprPck
{


	typedef iipr::CMovingAverageProcessorComp MovingAverageProcessor;
	typedef icomp::TMakeComponentWrap<iser::ISerializable, imod::TModelWrap<iipr::CRectangularFilterParams> > RectangularFilterParams;
	typedef iipr::CProcessedAcquisitionComp ProcessedCamera;
	typedef iipr::CLocalDifferenceProcessorComp LocalDifferenceProcessor;	
	typedef icomp::TMakeComponentWrap<iser::ISerializable, imod::TModelWrap<iipr::CLocalDifferenceFilterParams> > LocalDifferenceFilterParams;


} // namespace IprPck


#endif // !IprPck_included


