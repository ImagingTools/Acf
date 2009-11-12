#include "icam/CMemoryAcquisitionComp.h"


#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"



namespace icam
{


// reimplemented (iproc::IProcessor)

int CMemoryAcquisitionComp::DoProcessing(const iprm::IParamsSet* /*paramsPtr*/, const istd::IPolymorphic* /*inputPtr*/, istd::IChangeable* outputPtr)
{
	iimg::IBitmap* outputImagePtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputImagePtr == NULL){
		return TS_INVALID;
	}

	if (!outputImagePtr->CopyImageFrom(*this)){
		return TS_INVALID;
	}

	return TS_OK;
}


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CMemoryAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return BaseClass2::GetImageSize();
}


} // namespace icam


