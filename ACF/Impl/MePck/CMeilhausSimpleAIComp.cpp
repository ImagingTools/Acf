#include "imebase.h"


#include <QTime>

#include "CMeilhausSimpleAIComp.h"

using namespace imebase;

CMeilhausSimpleAIComp::CMeilhausSimpleAIComp()
{
	maxTaskId = -1;
}


// reimplemented (isig::ISamplesProcessor)
istd::CRange CMeilhausSimpleAIComp::GetValueRange(bool forInput, bool forOutput, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	if (!forInput && forOutput)
		return istd::CRange(*m_minVoltageAttrPtr, *m_maxVoltageAttrPtr);
	return istd::CRange(0, 0);
}

int CMeilhausSimpleAIComp::GetMaximalSamplesCount(bool forInput, bool forOutput, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	if (!forInput && forOutput)
		return *m_maxSampleCountAttrPtr;
	return 0;
}

// reimplemented (isig::ISamplingParams)
istd::CRange CMeilhausSimpleAIComp::GetIntervalRange() const
{// Maximum speed is 500kHz, minimum 1Hz.
	return istd::CRange(*m_minIntervalAttrPtr, *m_maxIntervalAttrPtr);
}

double CMeilhausSimpleAIComp::GetInterval() const
{
	return m_interval;
}

void CMeilhausSimpleAIComp::SetInterval(double value)
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

bool CMeilhausSimpleAIComp::SetSamplingMode(int mode)
{
	return mode == SM_SINGLE;
}

bool CMeilhausSimpleAIComp::IsSamplingModeSupported(int mode) const
{
	return mode == SM_SINGLE;
}

int CMeilhausSimpleAIComp::GetSamplingMode() const
{
	return SM_SINGLE;
}


// reimplemented (iproc::TIAssyncProcessor)

int CMeilhausSimpleAIComp::GetProcessorState(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return PS_READY;
}


void CMeilhausSimpleAIComp::ResetAllTasks()
{
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		delete m_activeTaskList.at(index);
		m_activeTaskList.removeAt(index);
	}
}

bool CMeilhausSimpleAIComp::AreParamsAccepted(const iprm::IParamsSet* /*paramsPtr*/) const
{
	CMeAddr address;
	address.device = 0;		
	address.subdevice =	4;
	address.channel =	0;

	/// @todo This is not working. I don't know how to get parameter value. :-(
	//address.device =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAIDevice")));
	//address.subdevice =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAISubdevice")));
	//address.channel =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAIChannel")));

	return m_meBase.CheckAddress(address, input);
}

int CMeilhausSimpleAIComp::BeginTask(	const iprm::IParamsSet* paramsPtr,
				const InputType* /*inputPtr*/,
				OutputType* outputPtr
				)
{
	if (!AreParamsAccepted(paramsPtr))
		return -1;

	CMeAddr address;
	address.device = 0;		
	address.subdevice =	4;
	address.channel =	0;

	/// @todo This is not working. I don't know how to get parameter value. :-(
	//address.device =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAIDevice")));
	//address.subdevice =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAISubdevice")));
	//address.channel =	*(dynamic_cast<int>(paramsPtr->GetParameter("MeAIChannel")));
	int interval_high, interval_low;

	if (meIOStreamTimeToTicks(address.device, address.subdevice,
							ME_TIMER_CONV_START, &m_interval,
							&interval_low, &interval_high,
							ME_VALUE_NOT_USED))
	return -1;

	meIOStreamConfig_t configList;
	configList.iChannel = address.channel;
	configList.iStreamConfig = 0;
	configList.iRef = ME_REF_AI_GROUND;
	configList.iFlags = ME_IO_STREAM_CONFIG_NO_FLAGS;

	meIOStreamTrigger_t trigger;
	trigger.iAcqStartTrigType = ME_TRIG_TYPE_SW;
	trigger.iAcqStartTrigEdge = ME_VALUE_NOT_USED;
	trigger.iAcqStartTrigChan = ME_TRIG_CHAN_DEFAULT;
	trigger.iAcqStartTicksLow = 66;
	trigger.iAcqStartTicksHigh = 0;
	trigger.iScanStartTrigType = ME_TRIG_TYPE_FOLLOW;
	trigger.iScanStartTicksLow = 0;
	trigger.iScanStartTicksHigh = 0;
	trigger.iConvStartTrigType = ME_TRIG_TYPE_TIMER;
	trigger.iConvStartTicksLow = interval_low;
	trigger.iConvStartTicksHigh = interval_high;
	trigger.iScanStopTrigType = ME_TRIG_TYPE_NONE;
	trigger.iScanStopCount = *m_maxSampleCountAttrPtr;
	trigger.iAcqStopTrigType = ME_TRIG_TYPE_FOLLOW;
	trigger.iAcqStopCount = 0;
	trigger.iFlags = ME_VALUE_NOT_USED;

	if (meIOStreamConfig(address.device, address.subdevice, &configList, 1, &trigger, *m_maxSampleCountAttrPtr, 0))
		return -1;

	CMeContext* context = new CMeContext();
	context->SetId(++maxTaskId);
	context->SetBufferSize(*m_maxSampleCountAttrPtr);
	context->SetSamplesContainer(outputPtr);
	if (!context->Register(address, input)){
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

int CMeilhausSimpleAIComp::WaitTaskFinished(int taskId, double timeoutTime, bool killOnTimeout)
{
	int ret;
	if (taskId == -1){
		ret  = this->WaitAllTasksFinished(timeoutTime, killOnTimeout);
	}
	else
		ret = this->WaitSingleTaskFinished(taskId, timeoutTime, killOnTimeout);
	return ret;
}

int CMeilhausSimpleAIComp::WaitSingleTaskFinished(int taskId, double timeoutTime, bool killOnTimeout)
{
	int ret = TS_NONE;//Not on list!
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		if (entry->GetId() == taskId){
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
	}
	I_ASSERT(TS_NONE == ret);

	return ret;
}

int CMeilhausSimpleAIComp::WaitAllTasksFinished(double timeoutTime, bool killOnTimeout)
{
	if (m_activeTaskList.isEmpty()){
		return TS_OK;
	}

	int ret = TS_OK;
	double localTimeout = timeoutTime;
	QTime stopper;
	stopper.start();
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		int lret = WaitSingleTaskFinished(entry->GetId(), localTimeout, killOnTimeout);
		if (TS_OK != lret){
			ret = lret;
		}
		localTimeout -= (double)stopper.elapsed() / 1000;
		if (localTimeout < 0)
			localTimeout = 0;
	}
	return ret;
}

int CMeilhausSimpleAIComp::GetReadyTask()
{
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		if (entry->IsDone())
			return entry->GetId();
	}
	return -1;
}

int CMeilhausSimpleAIComp::GetTaskState(int taskId) const
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
void CMeilhausSimpleAIComp::InitProcessor(const iprm::IParamsSet* /*paramsPtr*/)
{
}

// namespace isig
