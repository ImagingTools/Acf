#ifndef imebase_CMeilhausSimpleAOComp_included
#define imebase_CMeilhausSimpleAOComp_included

#include "imebase.h"
#include "CMeBase.h"
#include "CMeContext.h"

#include "isig/ISamplesProcessor.h"
#include "isig/ISamplingParams.h"
#include "icomp/CComponentBase.h"


namespace imebase
{

/**
	Interface allowing to operate on ME4600 card's analog I/O.
*/
class CMeilhausSimpleAOComp:
			public icomp::CComponentBase,
			virtual public isig::ISamplesProcessor,
			virtual public isig::ISamplingParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMeilhausSimpleAOComp)
		I_REGISTER_INTERFACE(isig::ISamplingParams)
		I_REGISTER_INTERFACE(isig::ISamplesProcessor)

		I_ASSIGN(m_minIntervalAttrPtr, "MinInterval", "Minimal interval of single sample", true, 0.000002);
		I_ASSIGN(m_maxIntervalAttrPtr, "MaxInterval", "Maximal interval of single sample", true, 1.000000);
		I_ASSIGN(m_minVoltageAttrPtr, "MinVoltage", "Minimal input voltage", true, -10.0);
		I_ASSIGN(m_maxVoltageAttrPtr, "MaxVoltage", "Maximal input voltage", true, 9.9996);
		I_ASSIGN(m_maxSampleCountAttrPtr, "SampleCount", "Maximal size of sample buffer", true, 1024);
	I_END_COMPONENT

	CMeilhausSimpleAOComp();

	// reimplemented (isig::ISamplesProcessor)
	virtual istd::CRange GetValueRange(	bool forInput = true,
										bool forOutput = true,
										const iprm::IParamsSet* paramsSetPtr = NULL
									  ) const;
	virtual int GetMaximalSamplesCount(	bool forInput = true,
										bool forOutput = true,
										const iprm::IParamsSet* paramsSetPtr = NULL
									  ) const;

	// reimplemented (isig::ISamplingParams)
	virtual istd::CRange GetIntervalRange() const;
	virtual double GetInterval() const;
	virtual void SetInterval(double value);
	virtual bool IsSamplingModeSupported(int mode) const;
	virtual int GetSamplingMode() const;
	virtual bool SetSamplingMode(int mode);

	// reimplemented (iproc::TIAssyncProcessor)
	virtual int GetProcessorState(const iprm::IParamsSet* paramsPtr) const;
	virtual void ResetAllTasks();
	virtual bool AreParamsAccepted(const iprm::IParamsSet* paramsPtr) const;
	virtual int BeginTask(	const iprm::IParamsSet* paramsPtr,
							const InputType* inputPtr,
							OutputType* outputPtr
						 );
	virtual int WaitTaskFinished(	int taskId = -1,
									double timeoutTime = -1,
									bool killOnTimeout = true
								);

	virtual int GetReadyTask();
	virtual int GetTaskState(int taskId = -1) const;
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

private:
	virtual WaitAllTasksFinished(double timeoutTime, bool killOnTimeout);
	virtual WaitSingleTaskFinished(int taskId, double timeoutTime, bool killOnTimeout);

private:
	CMeBase m_meBase;

	I_ATTR(double, m_minIntervalAttrPtr);
	I_ATTR(double, m_maxIntervalAttrPtr);

	I_ATTR(double, m_minVoltageAttrPtr);
	I_ATTR(double, m_maxVoltageAttrPtr);

	I_ATTR(int, m_maxSampleCountAttrPtr);

	double m_interval;

	typedef QList<CMeContext*> CMeContextList;
	CMeContextList m_activeTaskList;

	int maxTaskId;

};


} // namespace imebase


#endif // !imebase_CMeilhausSimpleAOComp_included

