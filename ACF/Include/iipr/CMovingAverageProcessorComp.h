#ifndef iipr_CMovingAverageProcessorComp_included
#define iipr_CMovingAverageProcessorComp_included


#include "iipr/TImageProcessorCompBase.h"
#include "iipr/CRectangularFilterParams.h"


namespace iipr{
	

/**
	Implementation of moving average operator.
*/
class CMovingAverageProcessorComp: public iipr::TImageProcessorCompBase<iipr::CRectangularFilterParams>
{
public:
	typedef iipr::TImageProcessorCompBase<iipr::CRectangularFilterParams> BaseClass;

protected:		
	// reimplemented (iipr::TImageProcessorCompBase<iipr::CRectangularFilterParams>)
	virtual bool ProcessImage(
				const CRectangularFilterParams* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);
};


} // namespace iipr


#endif // !iipr_CMovingAverageProcessorComp_included

