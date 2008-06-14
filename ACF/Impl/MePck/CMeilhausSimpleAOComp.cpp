#include "imebase.h"


#include <QTime>


#include "CMeilhausSimpleAOComp.h"


using namespace imebase;


CMeilhausSimpleAOComp::CMeilhausSimpleAOComp()
{
	maxTaskId = -1;
}

// reimplemented (isig::ISamplesProcessor)
istd::CRange CMeilhausSimpleAOComp::GetValueRange(bool forInput, bool forOutput, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	if (!forInput && forOutput)
		return istd::CRange(*m_minVoltageAttrPtr, *m_maxVoltageAttrPtr);
	return istd::CRange(0, 0);
}

int CMeilhausSimpleAOComp::GetMaximalSamplesCount(bool forInput, bool forOutput, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	if (!forInput && forOutput)
		return *m_maxSampleCountAttrPtr;
	return 0;
}

// reimplemented (isig::ISamplingParams)
istd::CRange CMeilhausSimpleAOComp::GetIntervalRange() const
{// Maximum speed is 500kHz, minimum 1Hz.
	return istd::CRange(*m_minIntervalAttrPtr, *m_maxIntervalAttrPtr);
}

double CMeilhausSimpleAOComp::GetInterval() const
{
	return m_interval;
}

void CMeilhausSimpleAOComp::SetInterval(double value)
{
	// This asserts are too strong.
	/* I_ASSERT(value >= *m_minIntervalAttrPtr); */
	/* I_ASSERT(value <= *m_minIntervalAttrPtr); */

	if (value < *m_minIntervalAttrPtr)
		m_interval = *m_minIntervalAttrPtr;
	else if (value > *m_minIntervalAttrPtr)
		m_interval = *m_minIntervalAttrPtr;
	else
		m_interval = value;
}

bool CMeilhausSimpleAOComp::SetSamplingMode(int mode)
{
	return mode == SM_PERIODIC;
}

bool CMeilhausSimpleAOComp::IsSamplingModeSupported(int mode) const
{
	return mode == SM_PERIODIC;
}

int CMeilhausSimpleAOComp::GetSamplingMode() const
{
	return SM_PERIODIC;
}


// reimplemented (iproc::TIAssyncProcessor)

int CMeilhausSimpleAOComp::GetProcessorState(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return PS_READY;
}


void CMeilhausSimpleAOComp::ResetAllTasks()
{
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		delete m_activeTaskList.at(index);
		m_activeTaskList.removeAt(index);
	}
}

bool CMeilhausSimpleAOComp::AreParamsAccepted(const iprm::IParamsSet* /*paramsPtr*/) const
{
	CMeAddr address;
	address.device = 0;		
	address.subdevice =	5;
	address.channel =	0;

	/// @todo This is not working. I don't know how to get parameter value. :-(
	//address.device =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAODevice")));
	//address.subdevice =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAOSubdevice")));
	//address.channel =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAOChannel")));

	return m_meBase.CheckAddress(address, input);
}

int CMeilhausSimpleAOComp::BeginTask(const iprm::IParamsSet* paramsPtr,
				const InputType* inputPtr,
				OutputType* /*outputPtr*/
				)
{
	if (!AreParamsAccepted(paramsPtr))
		return -1;

	CMeAddr address;
	address.device = 0;		
	address.subdevice =	5;
	address.channel =	0;

	/// @todo This is not working. I don't know how to get parameter value. :-(
	//address.device =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAODevice")));
	//address.subdevice =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAOSubdevice")));
	//address.channel =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAOChannel")));
	int interval_high, interval_low;

	if (meIOStreamTimeToTicks(address.device, address.subdevice,
							ME_TIMER_CONV_START, &m_interval,
							&interval_low, &interval_high,
							ME_VALUE_NOT_USED))
	return -1;

	meIOStreamConfig_t configList;
	configList.iChannel = address.channel;
	configList.iStreamConfig = 0;
	configList.iRef = ME_REF_AO_GROUND;
	configList.iFlags = ME_IO_STREAM_CONFIG_NO_FLAGS;

	meIOStreamTrigger_t trigger;
	trigger.iAcqStartTrigType = ME_TRIG_TYPE_SW;
	trigger.iAcqStartTrigEdge = ME_VALUE_NOT_USED;
	trigger.iAcqStartTrigChan = ME_TRIG_CHAN_DEFAULT;
	trigger.iAcqStartTicksLow = 0;
	trigger.iAcqStartTicksHigh = 0;
	trigger.iScanStartTrigType = ME_TRIG_TYPE_FOLLOW;
	trigger.iScanStartTicksLow = 0;
	trigger.iScanStartTicksHigh = 0;
	trigger.iConvStartTrigType = ME_TRIG_TYPE_TIMER;
	trigger.iConvStartTicksLow = interval_low;
	trigger.iConvStartTicksHigh = interval_high;
	trigger.iScanStopTrigType = ME_TRIG_TYPE_NONE;
	trigger.iScanStopCount = 0;
	trigger.iAcqStopTrigType = ME_TRIG_TYPE_NONE;
	trigger.iAcqStopCount = 0;
	trigger.iFlags = ME_IO_STREAM_CONFIG_WRAPAROUND;

	if (meIOStreamConfig(address.device, address.subdevice, &configList, 1, &trigger, 0, 0))
		return -1;

	CMeContext* context = new CMeContext();
	context->SetId(++maxTaskId);
	context->SetBufferSize(*m_maxSampleCountAttrPtr);
	context->SetSamplesContainer((void*)inputPtr);
	context->copyFromContainer();
	if (!context->Register(address, output)){
		delete context;
		return -1;
	}
	m_activeTaskList.append(context);

	meIOStreamStart_t startList;
	startList.iDevice = address.device;
	startList.iSubdevice = address.subdevice;
	startList.iStartMode = ME_START_MODE_NONBLOCKING;
	startList.iTimeOut = 0;
	startList.iFlags = 0;

	if (meIOStreamStart(&startList, 1, 0))
		return -1;

	return maxTaskId;

}

int CMeilhausSimpleAOComp::WaitTaskFinished(int taskId, double timeoutTime, bool killOnTimeout)
{
	int ret;
	if (taskId == -1){
		ret  = WaitAllTasksFinished(timeoutTime, killOnTimeout);
	}
	else
		ret = WaitSingleTaskFinished(taskId, timeoutTime, killOnTimeout);
	return ret;
}

int CMeilhausSimpleAOComp::WaitSingleTaskFinished(int taskId, double timeoutTime, bool killOnTimeout)
{
	int ret = TS_INVALID;//Not on list!
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		if (entry->GetId() == taskId)
			if (entry->Wait(timeoutTime)){
				entry->copyToContainer();
				delete entry;
				m_activeTaskList.removeAt(index);
				ret = TS_OK;
			}
			else{
				if (killOnTimeout){
					delete entry;
					m_activeTaskList.removeAt(index);
					ret = TS_INVALID;
				}
				else{
					ret = TS_WAIT;
				}
			}
	}
	return ret;
}

int CMeilhausSimpleAOComp::WaitAllTasksFinished(double timeoutTime, bool killOnTimeout)
{
	if (!m_activeTaskList.size())
		return TS_INVALID;

	int ret = TS_OK;
	double localTimeout = timeoutTime;
	QTime stopper;
	stopper.start();
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		int lret = WaitSingleTaskFinished(entry->GetId(), localTimeout, killOnTimeout);
		if (lret != TS_OK){
			ret = lret;
		}
		localTimeout -= (double)stopper.elapsed() / 1000;
		if (localTimeout < 0)
			localTimeout = 0;
	}
	return ret;
}

int CMeilhausSimpleAOComp::GetReadyTask()
{
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		if (entry->IsDone())
			return entry->GetId();
	}
	return -1;
}

int CMeilhausSimpleAOComp::GetTaskState(int taskId) const
{
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		if (taskId == -1){
			if (!entry->IsDone())
				break;

		}
		else {
			if (entry->GetId() == taskId)
				if (entry->IsDone())
					return TS_OK;
				else
					break;
		}
	}
	return TS_WAIT;
}

void CMeilhausSimpleAOComp::InitProcessor(const iprm::IParamsSet* /*paramsPtr*/)
{
}


// namespace isig
