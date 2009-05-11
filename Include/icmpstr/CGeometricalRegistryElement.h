#ifndef icmpstr_CGeometricalRegistryElement_included
#define icmpstr_CGeometricalRegistryElement_included


#include "i2d/IObject2d.h"

#include "icomp/CComponentAddress.h"
#include "icomp/CRegistryElement.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


class CGeometricalRegistryElement: public icomp::CRegistryElement, virtual public i2d::IObject2d
{
public:
	typedef icomp::CRegistryElement BaseClass;

	CGeometricalRegistryElement();

	const icomp::CComponentAddress& GetAddress() const;

	// overloaded (icomp::CRegistryElement)
	void Initialize(const icomp::IComponentStaticInfo* infoPtr, const icomp::CComponentAddress& address);

	// reimplemented (IObject2d)
	virtual i2d::CVector2d GetCenter() const;
	virtual void MoveTo(const i2d::CVector2d& position);

private:
	icomp::CComponentAddress m_addess;
	i2d::CVector2d m_center;
};


} // namespace icmpstr


#endif //!icmpstr_CGeometricalRegistryElement_included


