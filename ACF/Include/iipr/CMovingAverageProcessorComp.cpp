#include "iipr/CMovingAverageProcessorComp.h"


#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (iipr::CRectangularFilterBase)

void CMovingAverageProcessorComp::DoBufferFilter(
			const CRectangularFilterParams& params,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	int imageHeight = inputImage.GetImageSize().GetY();
	int imageWidth = inputImage.GetImageSize().GetX();

	int kernelWidth = params.GetSize().GetX();
	int kernelHeight = params.GetSize().GetY();

	int kernelHalfWidth = params.GetSize().GetX() / 2;
	int kernelHalfHeight = params.GetSize().GetY() / 2;

	for (int iterationIndex = 0; iterationIndex < params.GetIterationsCount(); ++iterationIndex){
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
				*outputImageBufferPtr++ = I_BYTE(meanValue/kernelWidth);
			}
		}

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
}


} // namespace iipr

