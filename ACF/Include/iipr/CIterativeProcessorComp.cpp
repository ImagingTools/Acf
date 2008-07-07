#include "iipr/CIterativeProcessorComp.h"


#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (iproc::TSyncProcessorWrap<iipr::IBitmapProcessor>)

int CIterativeProcessorComp::DoSyncProcess(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap* inputPtr,
			iimg::IBitmap* outputPtr)
{
	if (outputPtr == NULL){
		return BaseClass2::TS_INVALID;
	}

	if (!m_paramsIdAttrPtr.IsValid()){
		return BaseClass2::TS_INVALID;
	}

	int retVal = TS_INVALID;

	const CIterativeProcessorParams* processorParamsPtr = 
		dynamic_cast<const CIterativeProcessorParams*>(paramsPtr->GetParameter(m_paramsIdAttrPtr->GetValue().ToString()));
	if (processorParamsPtr != NULL && m_slaveProcessorCompPtr.IsValid()){
		int iterationsCount = processorParamsPtr->GetIterationsCount();

		if (iterationsCount == 1){
			return ProcessSlave(paramsPtr, inputPtr, outputPtr);
		}
			
		iimg::CGeneralBitmap inputBitmap;
		if (!inputBitmap.CopyImageFrom(*inputPtr)){
			return TS_INVALID;
		}

		for (int iterationIndex = 0; iterationIndex < iterationsCount; iterationIndex++){
			retVal = ProcessSlave(paramsPtr, &inputBitmap, outputPtr);
			if (retVal != TS_OK){
				return retVal;
			}

			if (iterationIndex < (iterationsCount - 1)){
				inputBitmap.CopyImageFrom(*outputPtr);
			}
		}
	}

	return retVal;
}


// private methods

int CIterativeProcessorComp::ProcessSlave(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap* inputPtr,
			iimg::IBitmap* outputPtr)
{
	I_ASSERT(m_slaveProcessorCompPtr.IsValid());

	int retVal = TS_INVALID;

	int taskId = m_slaveProcessorCompPtr->BeginTask(paramsPtr, inputPtr, outputPtr);
	if (taskId >= 0){
		retVal = m_slaveProcessorCompPtr->WaitTaskFinished(taskId);
	}

	return retVal;
}


} // namespace iipr

