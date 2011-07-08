#ifndef i2d_CLine2dComp_included
#define i2d_CLine2dComp_included


#include "i2d/CLine2d.h"

#include "icomp/CComponentBase.h"


namespace i2d
{		


/**
	Implementation of a 2D-line as a component.
	It gives the possibility to define a 2D-line model via component attributes.
*/
class CLine2dComp: public icomp::CComponentBase, public i2d::CLine2d
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CLine2dComp);
		I_REGISTER_INTERFACE(IObject2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_x1AttrPtr, "X1", "X-Position of start line point", true, 0);
		I_ASSIGN(m_y1AttrPtr, "Y1", "Y-Position of start line point", true, 0);
		I_ASSIGN(m_x2AttrPtr, "X2", "X-Position of end line point", true, 0);
		I_ASSIGN(m_y2AttrPtr, "Y2", "Y-Position of end line point", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_x1AttrPtr);
	I_ATTR(double, m_y1AttrPtr);
	I_ATTR(double, m_x2AttrPtr);
	I_ATTR(double, m_y2AttrPtr);
};


} // namespace i2d


#endif // !i2d_CLine2dComp_included


