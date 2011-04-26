#include "iproc/CModelBasedProcessingTriggerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"


namespace iproc
{


// protected methods
	
// reimplemented (imod::CMultiModelObserverBase)

void CModelBasedProcessingTriggerComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (!m_processorCompPtr.IsValid()){
		SendErrorMessage(0, "Processor not set");

		return;
	}

	if (!m_outputDataCompPtr.IsValid()){
		SendErrorMessage(0, "Output data object not set");

		return;
	}

	istd::CChangeNotifier changePtr(m_outputDataCompPtr.GetPtr());

	istd::IChangeable* inputDataPtr = dynamic_cast<istd::IChangeable*>(GetModelPtr());

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				inputDataPtr,
				m_outputDataCompPtr.GetPtr(),
				m_progressManagerCompPtr.GetPtr());
	if (retVal != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Processing failed");
	}
}


} // namespace iproc


