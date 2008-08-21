#include "iqtsig/CScriptSampleAcquisitionComp.h"


#include "istd/TChangeNotifier.h"

#include "isig/ISamplesContainer.h"


namespace iqtsig
{


CScriptSampleAcquisitionComp::CScriptSampleAcquisitionComp()
:	frameNumber(0)
{
}


// reimplemented (iproc::IProcessor)

int CScriptSampleAcquisitionComp::DoProcessing(
			const iprm::IParamsSet* /*paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	QScriptValue frameValue(&m_scriptEngine, ++frameNumber);
	m_scriptEngine.globalObject().setProperty("frame", frameValue);

	QString functionScript = iqt::GetQString(*m_defaultScriptAttrPtr);
	m_scriptEngine.evaluate(functionScript);

	istd::TChangeNotifier<isig::ISamplesContainer> containerPtr(dynamic_cast<isig::ISamplesContainer*>(outputPtr));
	if (!containerPtr.IsValid()){
		return TS_INVALID;
	}

	int samplesCount;
	if (m_samplesCountAttrPtr.IsValid()){
		samplesCount = *m_samplesCountAttrPtr;
		if (!containerPtr->SetSamplesCount(samplesCount)){
			return TS_INVALID;
		}
	}
	else{
		samplesCount = containerPtr->GetSamplesCount();
	}

	QScriptValue calcCtor = m_scriptEngine.evaluate("Calc");

	for (int i = 0; i < samplesCount; ++i){
		QScriptValueList arguments;
		QScriptValue xValue(&m_scriptEngine, i);
		arguments << xValue;

		double sample = calcCtor.call(m_scriptEngine.nullValue(), arguments).toNumber();

		if (m_scriptEngine.hasUncaughtException()) {
			return TS_INVALID;
		}

		containerPtr->SetSample(i, sample);
	}

	return TS_OK;
}


// reimplemented (isig::ISamplingConstraints)

istd::CRange CScriptSampleAcquisitionComp::GetIntervalRange() const
{
	return istd::CRange(0.001, 1);
}


bool CScriptSampleAcquisitionComp::IsSamplingModeSupported(int /*mode*/) const
{
	return true;
}


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


