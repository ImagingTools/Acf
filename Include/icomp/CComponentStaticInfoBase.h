// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// STL includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>

// ACF includes
#include <icomp/IComponentStaticInfo.h>


namespace icomp
{


class CComponentStaticInfoBase: virtual public IComponentStaticInfo
{
public:
	// reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const override;
	virtual const IAttributeStaticInfo* GetAttributeInfo(const QByteArray& attributeId) const override;
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const QByteArray& embeddedId) const override;
	virtual const QString& GetDescription() const override;
	virtual const QString& GetKeywords() const override;

	//	reimplemented (icomp::IElementStaticInfo)
	virtual Ids GetMetaIds(int metaGroupId) const override;
	virtual const IElementStaticInfo* GetSubelementInfo(const QByteArray& subcomponentId) const override;

	//	reimplemented (iattr::IAttributesMetaInfoProvider)
	virtual iattr::IAttributesProvider::AttributeIds GetAttributeMetaIds() const override;
	virtual const iattr::IAttributeMetaInfo* GetAttributeMetaInfo(const QByteArray& attributeId) const override;
};


} // namespace icomp




