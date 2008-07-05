#include "iipr/CLocalDifferenceProcessorComp.h"


#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (iipr::TImageProcessorCompBase)

bool CLocalDifferenceProcessorComp::ProcessImage(
			const iipr::CLocalDifferenceFilterParams* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	int alpha;
	double beta;;
	int gamma;

	if (paramsPtr == NULL){
		if (m_alphaAttrPtr.IsValid()){
			alpha = m_alphaAttrPtr->GetValue();
		}
		else{
			return false;
		}

		if (m_betaAttrPtr.IsValid()){
			beta = m_betaAttrPtr->GetValue();
		}
		else{
			return false;
		}
		
		if (m_gammaAttrPtr.IsValid()){
			gamma = m_gammaAttrPtr->GetValue();
		}
		else{
			return false;
		}
	}
	else{
		alpha = paramsPtr->GetAlpha();
		beta = paramsPtr->GetBeta();
		gamma = paramsPtr->GetGamma();
	}

	int imageHeight = inputImage.GetImageSize().GetY();
	int imageWidth = inputImage.GetImageSize().GetX();

	for (int lineIndex = 0; lineIndex < imageHeight; ++lineIndex){
		I_BYTE* inputLinePtr = (I_BYTE*)inputImage.GetLinePtr(lineIndex);
		I_BYTE* outputLinePtr = (I_BYTE*)outputImage.GetLinePtr(lineIndex);

		for (int x = 0; x < imageWidth; x++){
			int beginPoint = x - alpha;
			if (beginPoint < 0){
				beginPoint = 0;
			}
			int endPoint = x + alpha;
			if (endPoint >= imageWidth){
				endPoint = imageWidth - 1;
			}

			I_BYTE firstValue = *(inputLinePtr + beginPoint);
			I_BYTE lastValue = *(inputLinePtr + endPoint);
 
			I_BYTE* outputImagePtr = outputLinePtr + x;
			int outputValue = int(labs(firstValue - lastValue) * beta + gamma);
			if (outputValue < 0){
				outputValue = 0;
			}
			if (outputValue > 255){
				outputValue = 255;
			}

			*outputImagePtr = I_BYTE(outputValue);
		}
	}
	
	return true;
}


} // namespace iipr

