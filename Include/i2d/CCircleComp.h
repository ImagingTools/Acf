#ifndef i2d_CCircleComp_included
#define i2d_CCircleComp_included


#include "i2d/CCircle.h"

#include "icomp/CComponentBase.h"


namespace i2d
{		


/**
	Implementation of a circle as a component.
	It gives the possibility to define a circle model via component attributes.
*/
class CCircleComp: public icomp::CComponentBase, public i2d::CCircle
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCircleComp);
		I_REGISTER_INTERFACE(IObject2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_centerXAttrPtr, "X", "X-Position of the circle center", true, 0);
		I_ASSIGN(m_centerYAttrPtr, "Y", "Y-Position of the circle center", true, 0);
		I_ASSIGN(m_radiusAttrPtr, "Radius", "Radius of the circle", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_centerXAttrPtr);
	I_ATTR(double, m_centerYAttrPtr);
	I_ATTR(double, m_radiusAttrPtr);
};


} // namespace i2d


#endif // !i2d_CCircleComp_included


