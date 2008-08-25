#include "iipr/CMovingAverageProcessorComp.h"


#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (iipr::TImageProcessorCompBase)

bool CMovingAverageProcessorComp::ProcessImage(
			const IMultidimensionalFilterParams* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (paramsPtr == NULL){
		return false;
	}

	imath::CVarVector filterLengths = paramsPtr->GetFilterLengths();
	int filterDimensionsCount = filterLengths.GetElementsCount();
	if (filterDimensionsCount < 1){
		return false;
	}

	int kernelHalfWidth = int(filterLengths[0] * 0.5 - 0.5);
	int kernelHalfHeight = (filterDimensionsCount < 2)? kernelHalfWidth: int(filterLengths[1] * 0.5 - 0.5);
	int kernelWidth = kernelHalfWidth * 2 + 1;
	int kernelHeight = kernelHalfHeight * 2 + 1;
	int imageWidth = inputImage.GetImageSize().GetX();
	int imageHeight = inputImage.GetImageSize().GetY();

	if (kernelHalfWidth > 0){
		for (int lineIndex = 0; lineIndex < imageHeight; ++lineIndex){
			double meanValue = 0;

			I_BYTE* beginImagePtr = (I_BYTE*)inputImage.GetLinePtr(lineIndex);
			for (int x = 0; x < kernelWidth; ++x){
				meanValue += beginImagePtr[x];
			}

			I_BYTE* outputImageBufferPtr = (I_BYTE*)outputImage.GetLinePtr(lineIndex) + kernelHalfWidth + 1;
			*(outputImageBufferPtr - 1) = I_BYTE(meanValue / kernelWidth);

			I_BYTE* beginKernelPtr = beginImagePtr;
			I_BYTE* endKernelPtr = beginImagePtr + kernelWidth;
			I_BYTE* endBufferPtr = beginImagePtr + imageWidth;

			while (endKernelPtr < endBufferPtr){
				meanValue += (*endKernelPtr++ - *beginKernelPtr++);
				*outputImageBufferPtr++ = I_BYTE(meanValue / kernelWidth);
			}
		}
	}

	if (kernelHalfHeight > 0){
		iimg::CGeneralBitmap tempBitmap;
		tempBitmap.CopyImageFrom(outputImage);

		int lineDifference = tempBitmap.GetLinesDifference();

		for (int x = 0; x < imageWidth; ++x){	
			double meanValue = 0;
			I_BYTE* beginImagePtr = ((I_BYTE*)tempBitmap.GetLinePtr(0)) + x;

			for (int y = 0; y < kernelHeight; ++y){
				meanValue += beginImagePtr[y * lineDifference];
			}

			*((I_BYTE*)(outputImage.GetLinePtr(kernelHalfHeight)) + x) = I_BYTE(meanValue / kernelHeight);

			I_BYTE* outputImageBufferPtr = (I_BYTE*)(outputImage.GetLinePtr(kernelHalfHeight + 1)) + x;
			I_BYTE* beginKernelPtr = beginImagePtr;
			I_BYTE* endKernelPtr = beginImagePtr + kernelHeight * lineDifference;
			I_BYTE* endBufferPtr = beginImagePtr + imageHeight * lineDifference;

			while (endKernelPtr < endBufferPtr){
				meanValue += (*endKernelPtr - *beginKernelPtr);
				*outputImageBufferPtr = I_BYTE(meanValue / kernelHeight);

				endKernelPtr += lineDifference, beginKernelPtr += lineDifference, outputImageBufferPtr += lineDifference;
			}
		}
	}

	return true;
}


} // namespace iipr

