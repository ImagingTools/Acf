#ifndef iipr_TGaussianProcessorComp_included
#define iipr_TGaussianProcessorComp_included


#include "iipr/TConvolutionProcessorComp.h"
#include "iipr/TGaussianKernel2d.h"
#include "iipr/CGaussian2dFilterParams.h"


namespace iipr
{
	

/**
	Implementation of general gaussian filter.
*/

template <typename PixelType>
class TGaussianProcessorComp: 
	public iipr::TConvolutionProcessorComp<PixelType, iipr::CGaussian2dFilterParams, iipr::CGaussianKernel2d>
{
public:
	typedef iipr::TConvolutionProcessorComp<PixelType, iipr::CGaussian2dFilterParams, iipr::CGaussianKernel2d> BaseClass;

protected:		
	// reimplemented (iipr::TConvolutionProcessorComp)
	virtual iipr::CGaussianKernel2d* CreateKernel(const iipr::CGaussian2dFilterParams* paramsPtr);
};


// reimplemented (iipr::TConvolutionProcessorComp)

template <typename PixelType>
iipr::CGaussianKernel2d* TGaussianProcessorComp<PixelType>::CreateKernel(const iipr::CGaussian2dFilterParams* paramsPtr)
{
	if (paramsPtr == NULL){
		return NULL;
	}

	iipr::CGaussianKernel2d* kernelPtr = new iipr::CGaussianKernel2d(	paramsPtr->GetSigma1(),
																		paramsPtr->GetSigma2(),
																		paramsPtr->GetAngle());
	return kernelPtr;
}


typedef TGaussianProcessorComp<I_BYTE> CGaussianFilterComp;


} // namespace iipr


#endif // !iipr_TGaussianProcessorComp_included

