#include "icmpstr/CGeometricalRegistryElement.h"


#include "istd/TChangeNotifier.h"

#include "i2d/IObject2d.h"

#include "icomp/CRegistryElement.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


CGeometricalRegistryElement::CGeometricalRegistryElement()
:	m_center(0, 0)
{
}


const icomp::CComponentAddress& CGeometricalRegistryElement::GetAddress() const
{
	return m_addess;
}


// overloaded (icomp::CRegistryElement)

void CGeometricalRegistryElement::Initialize(const icomp::IComponentStaticInfo* infoPtr, const icomp::CComponentAddress& address)
{
	BaseClass::Initialize(infoPtr);

	m_addess = address;
}


// reimplemented (IObject2d)

i2d::CVector2d CGeometricalRegistryElement::GetCenter() const
{
	return m_center;
}


void CGeometricalRegistryElement::MoveTo(const i2d::CVector2d& position)
{
	if (position != m_center){
		istd::CChangeNotifier notifier(this);

		m_center = position;
	}
}


} // namespace icmpstr


