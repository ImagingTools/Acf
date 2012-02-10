#ifndef i2d_CPosition2dComp_included
#define i2d_CPosition2dComp_included


#include "i2d/CPosition2d.h"

#include "icomp/CComponentBase.h"


namespace i2d
{		


/**
	Implementation of a 2D-position as a component.
	It gives the possibility to define a 2D-position model via component attributes.
*/
class CPosition2dComp: public icomp::CComponentBase, public i2d::CPosition2d
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CPosition2dComp);
		I_REGISTER_INTERFACE(i2d::CPosition2d);
		I_REGISTER_INTERFACE(IObject2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_xAttrPtr, "X", "X-Position", true, 0);
		I_ASSIGN(m_yAttrPtr, "Y", "Y-Position", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_xAttrPtr);
	I_ATTR(double, m_yAttrPtr);
};


} // namespace i2d


#endif // !i2d_CPosition2dComp_included


