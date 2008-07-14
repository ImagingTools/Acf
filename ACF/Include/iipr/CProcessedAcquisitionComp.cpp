#include "iipr/CProcessedAcquisitionComp.h"

#include "istd/TChangeNotifier.h"

#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CProcessedAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* paramsPtr) const
{
	if (m_slaveAcquisitionCompPtr.IsValid()){
		return m_slaveAcquisitionCompPtr->GetBitmapSize(paramsPtr);
	}

	return istd::CIndex2d();
}


// protected methods

// reimplemented iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>

int CProcessedAcquisitionComp::DoProcessing(const iprm::IParamsSet* paramsPtr, const isys::ITimer* inputPtr, iimg::IBitmap* outputPtr)
{
	if (!m_slaveAcquisitionCompPtr.IsValid()){
		return TS_INVALID;
	}

	int retVal = TS_INVALID;

	istd::CChangeNotifier updateBlocker(outputPtr);

	int taskId = m_slaveAcquisitionCompPtr->BeginTask(paramsPtr, inputPtr, outputPtr);
	if (taskId >= 0){
		retVal = m_slaveAcquisitionCompPtr->WaitTaskFinished(taskId);
		if (retVal == TS_OK){
			if (m_processorCompPtr.IsValid()){
				iimg::CGeneralBitmap outputBitmap;
				taskId = m_processorCompPtr->BeginTask(paramsPtr, outputPtr, &outputBitmap);
				if (taskId >= 0){
					retVal = m_processorCompPtr->WaitTaskFinished(taskId);
					if (retVal == TS_OK){
						if (!outputPtr->CopyImageFrom(outputBitmap)){
							retVal = TS_INVALID;
						}
					}
				}
			}
		}
	}

	return retVal;
}


} // namespace iipr

