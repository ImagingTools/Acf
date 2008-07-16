#ifndef iipr_TConvolutionProcessorComp_included
#define iipr_TConvolutionProcessorComp_included


#include "iimg/TBitmapIterator.h"

#include "iipr/TImageProcessorCompBase.h"
#include "iipr/TLocalNeighborhood.h"


namespace iipr
{


/**
	Implementation of general convolution operator.
*/
template <typename PixelType, class ParameterType, class Kernel> 
class TConvolutionProcessorComp: public iipr::TImageProcessorCompBase<ParameterType>
{
public:
	typedef iipr::TImageProcessorCompBase<ParameterType> BaseClass;
	typedef iipr::TLocalNeighborhood<iimg::TBitmapIterator<PixelType>, Kernel> Neighborhood;

protected:		
	// abstract methods
	virtual Kernel* CreateKernel(const ParameterType* paramsPtr) = 0;

	// reimplemented (iipr::TImageProcessorCompBase<ParameterType>)
	virtual bool ProcessImage(
				const ParameterType* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);
};


// reimplemented (iipr::TImageProcessorCompBase<ParameterType>)

template <typename PixelType, class ParameterType, class Kernel> 
bool TConvolutionProcessorComp<PixelType, ParameterType, Kernel>::ProcessImage(
			const ParameterType* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (paramsPtr == NULL){
		return false;
	}

	bool retVal = true;

	istd::TDelPtr<Kernel> filterKernelPtr(CreateKernel(paramsPtr));
	if (!filterKernelPtr.IsValid()){
		return false;
	}

	// initialize kernel with input image:
	filterKernelPtr->InitForBitmap(inputImage);

	i2d::CRectangle kernelBoundingBox = filterKernelPtr->GetBoundingBox();
	i2d::CRectangle bitmapRegion = i2d::CRectangle(inputImage.GetImageSize());
	
	bitmapRegion.Expand(
				-kernelBoundingBox.GetWidth() / 2, 
				-kernelBoundingBox.GetWidth() / 2,
				-kernelBoundingBox.GetHeight() / 2,
				-kernelBoundingBox.GetHeight() / 2);

	iimg::TBitmapIterator<PixelType> inputIterator(&inputImage, &bitmapRegion);
	iimg::TBitmapIterator<PixelType> outputIterator(&outputImage, &bitmapRegion);

	while (!inputIterator.AtEnd()){
		double outputValue = 0.0;

		Neighborhood neighborhood(inputIterator, *filterKernelPtr); 
		Neighborhood::Iterator neighIter = neighborhood.Begin();

		for (		Kernel::Iterator kernelIter = filterKernelPtr->Begin();
					kernelIter != filterKernelPtr->End();
					++kernelIter){
					
			outputValue += (*kernelIter).second * (*neighIter);
			++neighIter;
		}

		*outputIterator = PixelType(outputValue / filterKernelPtr->GetWeightsSum());

		++inputIterator;
		++outputIterator;
	}

	return retVal;
}


} // namespace iipr


#endif // !iipr_TConvolutionProcessorComp_included

