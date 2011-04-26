#ifndef iproc_CModelBasedProcessingTriggerComp_included
#define iproc_CModelBasedProcessingTriggerComp_included


// ACF includes
#include "ibase/TLoggerCompWrap.h"

#include "imod/CSingleModelObserverBase.h"

#include "iproc/IProcessor.h"


namespace iproc
{


/**
	Component to trigger a processin action depending on model changes.
*/
class CModelBasedProcessingTriggerComp:
			public ibase::CLoggerComponentBase,
			public imod::CSingleModelObserverBase
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	
	I_BEGIN_COMPONENT(CModelBasedProcessingTriggerComp);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_processorCompPtr, "Processor", "Target processing component", true, "Processor");
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Processing progress manager", false, "ProgressManager");
		I_ASSIGN(m_paramsSetCompPtr, "ProcessingParamsSet", "Processing parameters", false, "ProcessingParameters");
		I_ASSIGN(m_outputDataCompPtr, "OutputData", "Processing output data", true, "OutputData");
	I_END_COMPONENT;

protected:
	// reimplemented (imod::CMultiModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	I_REF(iproc::IProcessor, m_processorCompPtr);
	I_REF(iproc::IProgressManager, m_progressManagerCompPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(istd::IChangeable, m_outputDataCompPtr);
};


} // namespace iproc


#endif // !iproc_CModelBasedProcessingTriggerComp_included


