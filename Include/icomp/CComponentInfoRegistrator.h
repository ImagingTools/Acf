// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <icomp/CBaseComponentStaticInfo.h>
#include <icomp/CPackageStaticInfo.h>


namespace icomp
{


/**
	Registers the static info of a component that is defined in another object file in a package.
	Unlike TComponentRegistrator it does not need the component type: everything except the description
	and the keywords is taken from the given component info. Used by package sources compiled with I_EXTERN_COMPONENT_INFO.
*/
class CComponentInfoRegistrator: public CBaseComponentStaticInfo
{
public:
	typedef CBaseComponentStaticInfo BaseClass;

	CComponentInfoRegistrator(
				const QByteArray& componentId,
				const IRealComponentStaticInfo& componentInfo,
				CPackageStaticInfo& packageStaticInfo,
				const QString& description,
				const QString& keywords);

	// reimplemented (icomp::IRealComponentStaticInfo)
	virtual IComponentUniquePtr CreateComponent() const override;

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const override;
	virtual const QString& GetDescription() const override;
	virtual const QString& GetKeywords() const override;

private:
	const IRealComponentStaticInfo& m_componentInfo;
	QString m_description;
	QString m_keywords;
};


} // namespace icomp


