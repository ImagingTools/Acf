#ifndef iipr_CCaliperParamsComp_included
#define iipr_CCaliperParamsComp_included


#include "icomp/CComponentBase.h"

#include "iipr/CCaliperParams.h"


namespace iipr
{


class CCaliperParamsComp: public icomp::CComponentBase, public CCaliperParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCaliperParamsComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ICaliperParams);
		I_ASSIGN(m_weightThresholdAttrPtr, "WeightThreshold", "Threshold of edge strength in range (0, 1)", true, 0.1);
		I_ASSIGN(m_polarityModeAttrPtr, "PolarityMode", "Mode of polarity, 0 - reised, 1 - dropped, 2 - any", true, PM_ALL);
		I_ASSIGN(m_directionModeAttrPtr, "DirectionMode", "Mode of direction, 0 - forward, 1 - backwards", true, DM_FORWARD);
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_weightThresholdAttrPtr);
	I_ATTR(int, m_polarityModeAttrPtr);
	I_ATTR(int, m_directionModeAttrPtr);
};


} // namespace iipr


#endif // !iipr_CCaliperParamsComp_included


