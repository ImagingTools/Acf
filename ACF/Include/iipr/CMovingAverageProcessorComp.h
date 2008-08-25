#ifndef iipr_CMovingAverageProcessorComp_included
#define iipr_CMovingAverageProcessorComp_included


#include "iipr/IMultidimensionalFilterParams.h"
#include "iipr/TImageProcessorCompBase.h"


namespace iipr
{
	

/**
	Implementation of moving average operator.
*/
class CMovingAverageProcessorComp: public iipr::TImageProcessorCompBase<IMultidimensionalFilterParams>
{
public:
	typedef iipr::TImageProcessorCompBase<IMultidimensionalFilterParams> BaseClass;

protected:		
	// reimplemented (iipr::TImageProcessorCompBase<iipr::IMultidimensionalFilterParams>)
	virtual bool ProcessImage(
				const IMultidimensionalFilterParams* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);
};


} // namespace iipr


#endif // !iipr_CMovingAverageProcessorComp_included

