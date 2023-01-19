#pragma once


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QVector>
#include <QtCore/QTextStream>

// ACF includes
#include <iser/CTextWriteArchiveBase.h>
#include <istd/TDelPtr.h>


namespace iser
{


/**
	Implementation of an ACF Archive serializing to JSON string
*/
class CJsonWriteArchiveBase: public iser::CTextWriteArchiveBase
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;

	CJsonWriteArchiveBase(
				const iser::IVersionInfo* versionInfoPtr = NULL,
				QJsonDocument::JsonFormat jsonFormat = QJsonDocument::Compact);

	virtual ~CJsonWriteArchiveBase();

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(QString& value);

	using BaseClass::Process;

protected:
	bool InitStream();
	bool InitArchive(QIODevice* devicePtr);
	bool InitArchive(QByteArray& inputString);
	void WriteTag(const iser::CArchiveTag& tag, QString separator, bool isWriteTag = true);

	// reimplemented (iser::CTextWriteArchiveBase)
	virtual bool WriteTextNode(const QByteArray& text);

protected:
	istd::TDelPtr<QTextStream> m_stream;
	bool m_firstTag;
	QJsonDocument::JsonFormat m_jsonFormat;

	QByteArray m_currentAttribute;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;

	bool m_isSeparatorNeeded;	// idicate that separator must be added before something is outputted
	bool m_allowAttribute;		// indicate if attribute outputting is allowed now

	QList<const iser::CArchiveTag*> m_tagsStack;
};


} // namespace iser


