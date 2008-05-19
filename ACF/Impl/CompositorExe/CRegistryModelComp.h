#ifndef CRegistryModelComp_included
#define CRegistryModelComp_included


#include <QPoint>


#include "icomp/CRegistry.h"
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "IRegistryGeometryProvider.h"


class CRegistryModelComp:	public icomp::TModelCompWrap< 
									icomp::TMakeComponentWrap<icomp::IRegistry, icomp::CRegistry> >,
							public IRegistryGeometryProvider
{
public:
	typedef icomp::TModelCompWrap< 
				icomp::TMakeComponentWrap<icomp::IRegistry, icomp::CRegistry> > BaseClass;

	I_BEGIN_COMPONENT(CRegistryModelComp)
		I_ASSIGN(m_staticInfoCompPtr, "StaticComponentInfo", "Static Component Info", true, "StaticComponentInfo")
	I_END_COMPONENT

	enum ChangeFlags
	{
		CF_POSITION = 0x1
	};

	// reimplemented (IRegistryGeometryProvider)
	virtual QPoint GetComponentPosition(const istd::CString& componentRole) const;
	virtual void SetComponentPosition(const istd::CString& componentRole, const QPoint& point);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	bool SerializeComponentPosition(iser::IArchive& archive, istd::CString& componentRole, int& x, int& y);

private:
	I_REF(icomp::IComponentStaticInfo, m_staticInfoCompPtr);

	typedef std::map<istd::CString, QPoint> ElementsPositionMap;

	ElementsPositionMap m_elementsPositionMap;
};


#endif // !CRegistryModelComp_included


