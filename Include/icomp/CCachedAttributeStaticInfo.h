// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtCore/QString>

// ACF includes
#include <memory>
#include <icomp/IAttributeStaticInfo.h>


namespace icomp
{


/**
	Lightweight implementation of IAttributeStaticInfo for cached component meta-info.
	This class stores attribute metadata loaded from a JSON cache file,
	avoiding the need to load the actual DLL package.
*/
class CCachedAttributeStaticInfo: virtual public IAttributeStaticInfo
{
public:
	CCachedAttributeStaticInfo(
				const QByteArray& id,
				const QString& description,
				const QByteArray& typeId,
				int flags);

	/**
		Add related meta IDs for a specific group and flags combination.
	*/
	void AddRelatedMetaId(int metaGroupId, const QByteArray& id, int flags);

	/**
		Set default value for this attribute.
		Takes ownership of the object.
	*/
	void SetDefaultValue(iser::IObject* defaultValuePtr);

	// reimplemented (icomp::IAttributeStaticInfo)
	virtual IElementStaticInfo::Ids GetRelatedMetaIds(int metaGroupId, int flags, int flagsMask) const override;

	// reimplemented (iattr::IAttributeMetaInfo)
	virtual QString GetAttributeDescription() const override;
	virtual const iser::IObject* GetAttributeDefaultValue() const override;
	virtual QByteArray GetAttributeTypeId() const override;
	virtual int GetAttributeFlags() const override;

private:
	QByteArray m_id;
	QString m_description;
	QByteArray m_typeId;
	int m_attributeFlags;
	std::unique_ptr<const iser::IObject> m_defaultValuePtr;

	typedef QMap<QByteArray, int> IdsToFlagsMap;
	typedef QMap<int, IdsToFlagsMap> RelatedIdsMap;
	RelatedIdsMap m_relatedIdsMap;
};


} // namespace icomp


