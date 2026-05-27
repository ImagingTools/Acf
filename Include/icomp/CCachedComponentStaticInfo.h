// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QString>

// ACF includes
#include <istd/TDelPtr.h>
#include <icomp/CComponentStaticInfoBase.h>
#include <icomp/CCachedAttributeStaticInfo.h>


namespace icomp
{


class CCachedElementStaticInfo;


/**
	Lightweight implementation of IComponentStaticInfo for cached component meta-info.
	This class stores component metadata loaded from a binary cache file,
	avoiding the need to load the actual DLL package.

	\note This class does not support component instantiation (CreateComponent)
	as it only provides meta-information for display purposes (e.g. Compositor UI).
*/
class CCachedComponentStaticInfo: public CComponentStaticInfoBase
{
public:
	typedef CComponentStaticInfoBase BaseClass;

	CCachedComponentStaticInfo(
				const QString& description,
				const QString& keywords,
				int componentType);

	/**
		Add an interface ID to this cached component's interface list.
	*/
	void AddInterfaceId(const QByteArray& interfaceId);

	/**
		Register a cached subelement info with ownership transfer.
		\param	subelementId		ID of the subelement.
		\param	subelementInfoPtr	subelement info object. Ownership is transferred.
	*/
	void RegisterSubelementInfo(const QByteArray& subelementId, CCachedElementStaticInfo* subelementInfoPtr);

	/**
		Register a cached embedded component info with ownership transfer.
		\param	embeddedId			ID of the embedded component.
		\param	componentInfoPtr	embedded component info object. Ownership is transferred.
	*/
	void RegisterEmbeddedComponentInfo(const QByteArray& embeddedId, CCachedComponentStaticInfo* componentInfoPtr);

	/**
		Register a cached attribute static info.
		Takes ownership of the provided object.
	*/
	void RegisterAttributeInfo(const QByteArray& attributeId, CCachedAttributeStaticInfo* attributeInfoPtr);

	// reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const override;
	virtual const IAttributeStaticInfo* GetAttributeInfo(const QByteArray& attributeId) const override;
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const QByteArray& embeddedId) const override;
	virtual const QString& GetDescription() const override;
	virtual const QString& GetKeywords() const override;

	// reimplemented (icomp::IElementStaticInfo)
	virtual Ids GetMetaIds(int metaGroupId) const override;
	virtual const IElementStaticInfo* GetSubelementInfo(const QByteArray& subcomponentId) const override;

	// reimplemented (iattr::IAttributesMetaInfoProvider)
	virtual iattr::IAttributesProvider::AttributeIds GetAttributeMetaIds() const override;
	virtual const iattr::IAttributeMetaInfo* GetAttributeMetaInfo(const QByteArray& attributeId) const override;

private:
	QString m_description;
	QString m_keywords;
	int m_componentType;

	Ids m_interfaceIds;

	typedef QMap<QByteArray, istd::TDelPtr<CCachedElementStaticInfo>> SubelementInfos;
	SubelementInfos m_subelementInfos;

	typedef QMap<QByteArray, istd::TDelPtr<CCachedComponentStaticInfo>> EmbeddedComponentInfos;
	EmbeddedComponentInfos m_embeddedComponentInfos;

	typedef QMap<QByteArray, istd::TDelPtr<CCachedAttributeStaticInfo>> AttributeInfos;
	AttributeInfos m_attributeInfos;
};


} // namespace icomp


