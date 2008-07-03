#ifndef iipr_CMovingAverageProcessorComp_included
#define iipr_CMovingAverageProcessorComp_included


#include "iipr/CRectangularFilterCompBase.h"


namespace iipr{
	

/**
	Implementation of moving average operator.
*/
class CMovingAverageProcessorComp: public CRectangularFilterCompBase
{
	typedef CRectangularFilterCompBase BaseClass;

protected:		
	// reimplemented (iipr::CRectangularFilterBase)
	virtual void DoBufferFilter(
				const CRectangularFilterParams& params,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);
};


} // namespace iipr


#endif // !iipr_CMovingAverageProcessorComp_included

