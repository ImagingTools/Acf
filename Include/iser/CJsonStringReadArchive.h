#pragma once


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QVector>

// ACF includes
#include <iser/CTextReadArchiveBase.h>


namespace iser
{


/**
	Implementation of an ACF archive deserializing from a JSON string

	\note	As to simplify decoding of some more complicated data structures support
			for special annotation tags was added. They are used for guiding the 
			deserialization algorithm in some special cases.
*/
class CJsonStringReadArchive : public iser::CTextReadArchiveBase
{
public:
	typedef CTextReadArchiveBase BaseClass;

	CJsonStringReadArchive(const QByteArray& inputString);

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(QString& value);

protected:
	bool ReadStringNode(QString& text);

private:
	QJsonDocument m_document;
		
	QVector<QJsonObject::const_iterator> m_objectsStack;

	bool m_isNewFormat;			// idicate that new format is enabled
	bool m_allowAttribute;		// indicate if attribute outputting is allowed now

	QByteArray m_currentAttribute;

	typedef QList<const iser::CArchiveTag*> TagsList;
	TagsList m_tagsStack;
};


} // namespace iser


