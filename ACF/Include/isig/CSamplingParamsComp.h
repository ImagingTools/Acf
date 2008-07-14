#ifndef isig_CSamplingParamsComp_included
#define isig_CSamplingParamsComp_included


#include "icomp/CComponentBase.h"

#include "isig/ISamplingParams.h"


namespace isig
{


/**
	General implementation of sampling parameters.
	In this implementation the boundaries are fix defined using component attributes.
*/
class CSamplingParamsComp: public icomp::CComponentBase, virtual public isig::ISamplingParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSamplingParamsComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(isig::ISamplingParams);
		I_ASSIGN(m_samplingConstraintsCompPtr, "SamplingConstraints", "Information about acceptable values for this parameters", false, "SamplingConstraints");
	I_END_COMPONENT

	CSamplingParamsComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (isig::ISamplingParams)
	virtual const ISamplingConstraints* GetConstraints() const;
	virtual double GetInterval() const;
	virtual void SetInterval(double value);
	virtual int GetSamplingMode() const;
	virtual bool SetSamplingMode(int mode);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_interval;
	int m_samplingMode;

	I_REF(ISamplingConstraints, m_samplingConstraintsCompPtr);
};


} // namespace isig


#endif // !isig_CSamplingParamsComp_included


