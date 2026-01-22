// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <istd/TDelPtr.h>

#include <icomp/CPackageStaticInfo.h>
#include <icomp/CCompositeComponentStaticInfo.h>


namespace icomp
{


class IComponentEnvironmentManager;


/**
	Implementation of package static info designing to provide composite component information 'on demand'.
*/
class CCompositePackageStaticInfo: public CPackageStaticInfo
{
public:
	typedef CPackageStaticInfo BaseClass;

	CCompositePackageStaticInfo(
				const QByteArray& packageId,
				const icomp::IComponentEnvironmentManager* managerPtr);

	void RegisterEmbeddedComponent(const QByteArray& componentId);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual Ids GetMetaIds(int metaGroupId) const override;
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const QByteArray& embeddedId) const override;

private:
	struct ComponentInfo
	{
		ComponentInfo():isInitialized(false){}

		istd::TDelPtr<icomp::CCompositeComponentStaticInfo> componentInfoPtr;
		bool isInitialized;
	};

	typedef QMap<QByteArray, ComponentInfo> EmbeddedComponentInfos;
	mutable EmbeddedComponentInfos m_embeddedComponentInfos;

	QByteArray m_packageId;
	const icomp::IComponentEnvironmentManager& m_envManager;
};


} // namespace icomp




