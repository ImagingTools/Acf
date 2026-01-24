// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>


namespace icomp
{


/**
	Class to decode/encode the component category into human readable text.
*/
class CComponentMetaDescriptionEncoder
{
public:
	explicit CComponentMetaDescriptionEncoder(const QString& metaDescription);

	/**
		Get meta keys using in the meta-description.
	*/
	const QStringList& GetMetaKeys() const;

	/**
		Get value list for the given key.
	*/
	const QStringList& GetValues(const QString& key = QString()) const;

	/**
		Get the keyword list.
	*/
	const QStringList& GetUnassignedKeywords() const;

private:
	typedef QMap<QString, QStringList> MetaValuesMap;

	MetaValuesMap m_metaValuesMap;
	QStringList m_metaKeys;
	QStringList m_keywords;
	QStringList m_unassignedKeywords;
};


} // namespace icomp




