#ifndef iqtsig_CScriptSampleAcquisitionComp_included
#define iqtsig_CScriptSampleAcquisitionComp_included


#include <QtScript>

#include "icomp/CComponentBase.h"

#include "iproc/TSyncProcessorWrap.h"

#include "iqtsig/iqtsig.h"


namespace iqtsig
{


class CScriptSampleAcquisitionComp:
			public icomp::CComponentBase,
			public iproc::CSyncProcessorBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CScriptSampleAcquisitionComp)
		I_REGISTER_INTERFACE(iproc::IProcessor)
		I_ASSIGN(m_minValueAttrPtr, "MinValue", "Minimal value of sample", true, 0.0);
		I_ASSIGN(m_maxValueAttrPtr, "MaxValue", "Maximal value of sample", true, 100.0);
		I_ASSIGN(m_samplesCountAttrPtr, "SamplesCount", "Number of samples processed in single step", false, 100);
		I_ASSIGN(m_defaultScriptAttrPtr, "DefaultScript", "Default script used to calculate sample output in 'Calc' function", false, "minValue");
	I_END_COMPONENT

	CScriptSampleAcquisitionComp();

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

	// reimplemented (isig::ISamplingConstraints)
	virtual istd::CRange GetIntervalRange() const;
	virtual bool IsSamplingModeSupported(int mode) const;
	virtual istd::CRange GetValueRange(bool forInput = true, bool forOutput = true, const iprm::IParamsSet* paramsSetPtr = 0) const;
	virtual int GetMaximalSamplesCount(bool forInput = true, bool forOutput = true, const iprm::IParamsSet* paramsSetPtr = 0) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	QScriptEngine m_scriptEngine;
	int frameNumber;

	I_ATTR(double, m_minValueAttrPtr);
	I_ATTR(double, m_maxValueAttrPtr);
	I_ATTR(int, m_samplesCountAttrPtr);
	I_ATTR(istd::CString, m_defaultScriptAttrPtr);
};


} // namespace iqtsig


#endif // !iqtsig_CScriptSampleAcquisitionComp_included


