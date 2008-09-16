#include "iipr/CSearchBasedFeaturesSupplierComp.h"


namespace iipr
{


// reimplemented (iipr::IFeaturesSupplier)

const iipr::IFeaturesContainer* CSearchBasedFeaturesSupplierComp::GetFeatures(I_DWORD objectId) const
{
	const WorkInfo* infoPtr = GetWorkInfo(objectId, true);
	if ((infoPtr != NULL) && (infoPtr->status == WS_OK)){
		return &infoPtr->product;
	}

	return NULL;
}


// reimplemented (iproc::ISupplier)

void CSearchBasedFeaturesSupplierComp::BeginNextObject(I_DWORD objectId)
{
	if (!IsIdKnown(objectId)){
		BaseClass::BeginNextObject(objectId);

		if (m_bitmapSupplierCompPtr.IsValid()){
			m_bitmapSupplierCompPtr->BeginNextObject(objectId);
		}
	}
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CSearchBasedFeaturesSupplierComp::ProduceObject(I_DWORD objectId, CSearchFeatures& result) const
{
	if (		m_bitmapSupplierCompPtr.IsValid() &&
				m_searchProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapSupplierCompPtr->GetBitmap(objectId);
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			int searchState = m_searchProcessorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&result);

			if (searchState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}

			return WS_OK;
		}
	}

	return WS_CRITICAL;
}


} // namespace iipr


