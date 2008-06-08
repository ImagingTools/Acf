#include "iqtsig/CScriptSampleAcquisitionComp.h"


namespace iqtsig
{


CScriptSampleAcquisitionComp::CScriptSampleAcquisitionComp()
:	frameNumber(0)
{
}


// reimplemented (iproc::TSyncProcessorWrap<isig::ISamplesProcessor>)

int CScriptSampleAcquisitionComp::DoSyncProcess(
			const iprm::IParamsSet* /*paramsPtr*/,
			const isig::ISamplesContainer* /*inputPtr*/,
			isig::ISamplesContainer* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	QScriptValue frameValue(&m_scriptEngine, ++frameNumber);
	m_scriptEngine.globalObject().setProperty("frame", frameValue);

	QString script = iqt::GetQString(*m_defaultScriptAttrPtr);

	int samplesCount = outputPtr->GetSamplesCount();
	for (int i = 0; i < samplesCount; ++i){
		QScriptValue xValue(&m_scriptEngine, i);
		m_scriptEngine.globalObject().setProperty("x", xValue);

		double sample = m_scriptEngine.evaluate(script).toNumber();

		if (m_scriptEngine.hasUncaughtException()) {
			return TS_INVALID;
		}

		outputPtr->SetSample(i, sample);
	}

	return TS_OK;
}


// reimplemented (isig::ISamplesProcessor)

istd::CRange CScriptSampleAcquisitionComp::GetValueRange(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return istd::CRange(*m_minValueAttrPtr, *m_maxValueAttrPtr);
}


int CScriptSampleAcquisitionComp::GetMaximalSamplesCount(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return -1;
}


// reimplemented (icomp::IComponent)

void CScriptSampleAcquisitionComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	QScriptValue minValue(&m_scriptEngine, *m_minValueAttrPtr);
	m_scriptEngine.globalObject().setProperty("minValue", minValue);
	QScriptValue maxValue(&m_scriptEngine, *m_maxValueAttrPtr);
	m_scriptEngine.globalObject().setProperty("maxValue", maxValue);
}


} // namespace iqtsig


