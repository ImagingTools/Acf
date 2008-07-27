#include "icam/CSnapBitmapSupplierComp.h"


namespace icam
{


iprm::IParamsSet* CSnapBitmapSupplierComp::GetParamsSet() const
{
	return m_paramsSetCompPtr.GetPtr();
}


// reimplemented (icam::IBitmapSupplier)

const iimg::IBitmap* CSnapBitmapSupplierComp::GetBitmap(I_DWORD objectId) const
{
	const WorkInfo* infoPtr = GetWorkInfo(objectId, true);
	if (infoPtr != NULL){
		return infoPtr->product.GetPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CSnapBitmapSupplierComp::Serialize(iser::IArchive& archive)
{
	if (m_paramsSetCompPtr.IsValid()){
		return m_paramsSetCompPtr->Serialize(archive);
	}

	return true;
}


I_DWORD CSnapBitmapSupplierComp::GetMinimalVersion(int versionId) const
{
	if (m_paramsSetCompPtr.IsValid()){
		return m_paramsSetCompPtr->GetMinimalVersion(versionId);
	}

	return BaseClass::GetMinimalVersion(versionId);
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CSnapBitmapSupplierComp::ProduceObject(I_DWORD objectId, istd::TDelPtr<iimg::IBitmap>& result) const
{
	if (m_bitmapCompFact.IsValid() && !result.IsValid()){
		result.SetPtr(m_bitmapCompFact.CreateInstance());
	}

	if (result.IsValid() && m_bitmapAcquisitionCompPtr.IsValid()){
		int status = m_bitmapAcquisitionCompPtr->DoProcessing(m_paramsSetCompPtr.GetPtr(), NULL, result.GetPtr());
		switch (status){
		case IBitmapAcquisition::TS_OK:
			return WS_OK;

		case IBitmapAcquisition::TS_CANCELED:
			return WS_CANCELED;

		default:
			return WS_ERROR;
		}
	}

	return WS_CRITICAL;
}


} // namespace icam


