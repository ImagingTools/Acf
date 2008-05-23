#ifndef CRegistryModelComp_included
#define CRegistryModelComp_included


#include <QPoint>


#include "icomp/IRegistryGeometryProvider.h"
#include "icomp/CRegistry.h"
#include "icomp/CPackageStaticInfo.h"
#include "icomp/CComponentBase.h"


class CRegistryModelComp:	public icomp::CComponentBase,
							public icomp::CRegistry,
							virtual public icomp::IRegistryGeometryProvider
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef icomp::CRegistry BaseClass2;

	I_BEGIN_COMPONENT(CRegistryModelComp)
		I_REGISTER_INTERFACE(icomp::IRegistry)
		I_ASSIGN(m_staticInfoCompPtr, "StaticComponentInfo", "Static Component Info", true, "StaticComponentInfo")
	I_END_COMPONENT

	enum ChangeFlags
	{
		CF_POSITION = 0x20000
	};

	virtual bool SerializeComponentsLayout(iser::IArchive& archive);

	// reimplemented (icomp::IRegistryGeometryProvider)
	virtual imath::CVector2d GetComponentPosition(const std::string& componentRole) const;
	virtual void SetComponentPosition(const std::string& componentRole, const imath::CVector2d& point);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected:
	bool SerializeComponentPosition(iser::IArchive& archive, std::string& componentName, imath::CVector2d& position);

private:
	I_REF(icomp::CPackageStaticInfo, m_staticInfoCompPtr);

	typedef std::map<std::string, imath::CVector2d> ElementsPositionMap;

	ElementsPositionMap m_elementsPositionMap;
};


#endif // !CRegistryModelComp_included


