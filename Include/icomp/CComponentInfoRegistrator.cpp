// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CComponentInfoRegistrator.h>


namespace icomp
{


// public methods

CComponentInfoRegistrator::CComponentInfoRegistrator(
			const QByteArray& componentId,
			const IRealComponentStaticInfo& componentInfo,
			CPackageStaticInfo& packageStaticInfo,
			const QString& description,
			const QString& keywords)
:	BaseClass(&componentInfo),
	m_componentInfo(componentInfo),
	m_description(description),
	m_keywords(keywords)
{
	packageStaticInfo.RegisterEmbeddedComponentInfo(componentId, this);
	m_keywords += " ";
	m_keywords += packageStaticInfo.GetKeywords();
}


// reimplemented (icomp::IRealComponentStaticInfo)

IComponentUniquePtr CComponentInfoRegistrator::CreateComponent() const
{
	return m_componentInfo.CreateComponent();
}


//	reimplemented (icomp::IComponentStaticInfo)

int CComponentInfoRegistrator::GetComponentType() const
{
	return m_componentInfo.GetComponentType();
}


const QString& CComponentInfoRegistrator::GetDescription() const
{
	return m_description;
}


const QString& CComponentInfoRegistrator::GetKeywords() const
{
	return m_keywords;
}


} // namespace icomp


