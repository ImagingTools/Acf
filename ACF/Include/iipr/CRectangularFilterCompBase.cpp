#include "iipr/CRectangularFilterCompBase.h"


#include "iipr/CRectangularFilterParams.h"


namespace iipr
{


// protected methods

// reimplemented (iproc::TSyncProcessorWrap<iipr::IBitmapProcessor>)

int CRectangularFilterCompBase::DoSyncProcess(	
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap* inputPtr,
			iimg::IBitmap* outputPtr)
{
	if (inputPtr == NULL || outputPtr == NULL){
		return BaseClass2::TS_INVALID;
	}

	if (!m_paramsIdAttrPtr.IsValid()){
		return BaseClass2::TS_INVALID;
	}

	const iipr::CRectangularFilterParams* filterParamsPtr = 
		dynamic_cast<const iipr::CRectangularFilterParams*>(paramsPtr->GetParameter(m_paramsIdAttrPtr->GetValue().ToString()));
	if (filterParamsPtr == NULL){
		return BaseClass2::TS_INVALID;
	}

	// expand image with virtual margins:
	if (!iipr::CImageManipulation::GetExpanded(*inputPtr, const_cast<iimg::IBitmap&>(*inputPtr), filterParamsPtr->GetSize())){
		return BaseClass2::TS_INVALID;
	}

	// create output image:
	outputPtr->CopyImageFrom(*inputPtr);

	// do image processing:
	DoBufferFilter(
				*filterParamsPtr,
				*inputPtr, 
				*outputPtr);
	
	// shrink output image to the original size:
	iipr::CImageManipulation::GetShrinked(*outputPtr, const_cast<iimg::IBitmap&>(*outputPtr), filterParamsPtr->GetSize());

	return BaseClass2::TS_OK;
}


} // namespace iipr

