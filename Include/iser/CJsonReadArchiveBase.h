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
class CJsonReadArchiveBase : public iser::CTextReadArchiveBase
{
public:
	typedef CTextReadArchiveBase BaseClass;

	CJsonReadArchiveBase();

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(QString& value);

protected:
	bool InitArchive(const QByteArray& inputString);
	bool ReadStringNode(QString& text);

	// reimplemented (iser::CTextReadArchiveBase)
	virtual bool ReadTextNode(QByteArray& text);

protected:
	QJsonDocument m_document;

	class HelperIterator
	{
	public:
		void SetKey(QString& key) { m_key = key; }
		const QString GetKey() { return m_key; }
		QString GetValue() { return m_value.toString(); }
		QJsonObject GetObject()
		{
			if (m_value.isObject()){

				return m_value.toObject();
			}
			if (m_value.isArray()){
				return m_arrayIterator->toObject();
			}
			return QJsonObject();
		}
		bool NextElementArray() { m_arrayIterator++; return m_arrayIterator != m_array.constEnd(); }
		void SetValue(const QJsonValue value)
		{
			m_value = value;
			if (m_value.isArray()){
				m_array = m_value.toArray();
				m_arrayIterator = m_array.constBegin();
			}
		}
		bool isObject() { return m_value.isObject(); }
		bool isArray() { return m_value.isArray(); }
	private:
		QJsonArray m_array;
		QJsonArray::ConstIterator m_arrayIterator;
		QJsonValue m_value;
		QString m_key;
	};
		
	QVector<HelperIterator> m_iterators;


//	typedef QList<const iser::CArchiveTag*> TagsList;
//	TagsList m_tagsStack;
	iser::CArchiveTag m_rootTag;

};


} // namespace iser


