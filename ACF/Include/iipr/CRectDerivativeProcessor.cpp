#include "iipr/CRectDerivativeProcessor.h"


#include "istd/TChangeNotifier.h"

#include "iipr/IMultidimensionalFilterParams.h"


namespace iipr
{


bool CRectDerivativeProcessor::DoDerivativeProcessing(const CProjectionData& source, double filterLength, CProjectionData& results)
{
	const iimg::IBitmap& sourceBitmap = source.GetProjectionImage();

	istd::CIndex2d sourceBitmapSize = sourceBitmap.GetImageSize();

	istd::CChangeNotifier notifier(&results);

	const istd::CRange& sourceProportionRange = source.GetProportionRangeX();
	double proportionXAlpha = 0.5 / sourceBitmapSize.GetX();
	results.SetProportionRangeX(istd::CRange(
				sourceProportionRange.GetValueFromAlpha(proportionXAlpha),
				sourceProportionRange.GetValueFromAlpha(1.0 - proportionXAlpha)));
	results.SetProportionRangeY(source.GetProportionRangeY());

	iimg::IBitmap& resultBitmap = results.GetProjectionImage();

	return DoBitmapProcessing(sourceBitmap, sourceProportionRange.GetLength() * filterLength, resultBitmap);
}


// reimplemented (iproc::IProcessor)

int CRectDerivativeProcessor::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr)
{
	const CProjectionData* inputProjectionPtr = dynamic_cast<const CProjectionData*>(inputPtr);
	CProjectionData* outputProjectionPtr = dynamic_cast<CProjectionData*>(outputPtr);

	double filterLength = 5.0;
	if (paramsPtr != NULL){
		const IMultidimensionalFilterParams* filterParamsPtr = dynamic_cast<const IMultidimensionalFilterParams*>(
					paramsPtr->GetParameter(m_filterParamsId));
		if (filterParamsPtr != NULL){
			imath::CVarVector filterLengths = filterParamsPtr->GetFilterLengths();
			if (filterLengths.GetElementsCount() > 0){
				filterLength = filterLengths[0];
			}
		}
	}

	if (		(inputProjectionPtr == NULL) ||
				(outputProjectionPtr == NULL)){
		const iimg::IBitmap* sourceBitmapPtr = (inputProjectionPtr != NULL)?
					&inputProjectionPtr->GetProjectionImage():
					dynamic_cast<const iimg::IBitmap*>(inputPtr);
		iimg::IBitmap* resultBitmapPtr = (outputProjectionPtr != NULL)?
					&outputProjectionPtr->GetProjectionImage():
					dynamic_cast<iimg::IBitmap*>(outputPtr);

		if (		(sourceBitmapPtr == NULL) ||
					(resultBitmapPtr == NULL)){
			return TS_INVALID;
		}

		return DoBitmapProcessing(*sourceBitmapPtr, filterLength, *resultBitmapPtr)?
					TS_OK:
					TS_INVALID;
	}

	return DoDerivativeProcessing(*inputProjectionPtr, filterLength, *outputProjectionPtr)?
				TS_OK:
				TS_INVALID;
}


// protected methods

bool CRectDerivativeProcessor::DoBitmapProcessing(
			const iimg::IBitmap& sourceBitmap,
			double filterLength,
			iimg::IBitmap& resultBitmap)
{
	if (sourceBitmap.GetComponentBitsCount() != 8){
		return false;
	}

	double halfRealLength = istd::Max(1.0, filterLength * 0.5);

	int sumOffset = int(halfRealLength);
	double sumLastAlpha = halfRealLength - sumOffset;

	istd::CIndex2d sourceBitmapSize = sourceBitmap.GetImageSize();

	int projectionWidth = sourceBitmapSize.GetX() - 1;

	istd::CChangeNotifier notifier(&resultBitmap);

	if (!resultBitmap.CreateBitmap(istd::CIndex2d(projectionWidth, sourceBitmapSize.GetY()), 8, 1)){
		return false;
	}

	if (projectionWidth > 0){
		for (int y = 0; y < sourceBitmapSize.GetY(); ++y){
			const I_BYTE* sourceLinePtr = (const I_BYTE*)sourceBitmap.GetLinePtr(y);
			I_ASSERT(sourceLinePtr != NULL);
			I_BYTE* destLinePtr = (I_BYTE*)resultBitmap.GetLinePtr(y);
			I_ASSERT(destLinePtr != NULL);

			double leftSum = 0.0;
			double leftWeight = 0.0;
			double rightSum = sourceLinePtr[0] * sumLastAlpha;
			double rightWeight = sumLastAlpha;
			for (int x = -sumOffset; x < projectionWidth; ++x){
				if (x <= projectionWidth - sumOffset){
					if (x < projectionWidth - sumOffset){
						rightSum += sourceLinePtr[x + sumOffset + 1] * sumLastAlpha + sourceLinePtr[x + sumOffset] * (1 - sumLastAlpha);
						rightWeight += 1;
					}
					else{
						rightSum += sourceLinePtr[x + sumOffset] * (1 - sumLastAlpha);
						rightWeight += (1 - sumLastAlpha);
					}
				}

				if (x >= 0){
					leftSum += sourceLinePtr[x];
					if (x >= sumOffset){
						leftSum -= sourceLinePtr[x - sumOffset] * (1 - sumLastAlpha);

						if (x > sumOffset){
							leftSum -= sourceLinePtr[x - sumOffset - 1] * sumLastAlpha;
						}
						else{
							leftWeight += sumLastAlpha;
						}
					}
					else{
						leftWeight += 1;
					}

					rightSum -= sourceLinePtr[x];
					rightWeight -= 1;

					int pixelValue = int(128 + (rightSum / rightWeight - leftSum / leftWeight));
					if (pixelValue < 0){
						pixelValue = 0;
					}
					else if (pixelValue > 255){
						pixelValue = 255;
					}

					destLinePtr[x] = I_BYTE(pixelValue);
				}
			}
		}
	}

	return true;
}


} // namespace iipr


