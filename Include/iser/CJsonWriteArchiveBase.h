// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QVector>
#include <QtCore/QDataStream>
#include <QtCore/QBuffer>

// ACF includes
#include <iser/CTextWriteArchiveBase.h>
#include <istd/TDelPtr.h>


namespace iser
{


/**
	Implementation of an ACF Archive serializing to JSON string

	Tags of type TT_LEAF are written as JSON values. Other tags are written as JSON objects
	as soon as they contain child tags. A non-leaf tag containing only a single primitive value
	is written as a plain JSON value too, so \c "Name": "value" is produced instead of the
	\c "Name": {"Name": "value"} layout written by older versions (it is still readable).
*/
class CJsonWriteArchiveBase: public iser::CTextWriteArchiveBase
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;

	~CJsonWriteArchiveBase();

	void SetFormat(QJsonDocument::JsonFormat jsonFormat);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const override;
	virtual bool BeginTag(const iser::CArchiveTag& tag) override;
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count) override;
	virtual bool EndTag(const iser::CArchiveTag& tag) override;
	virtual bool Process(QString& value) override;
	virtual bool Process(QByteArray& value) override;
	virtual bool ProcessData(void* dataPtr, int size) override;

	using BaseClass::Process;

protected:
	CJsonWriteArchiveBase(
				const iser::IVersionInfo* versionInfoPtr,
				bool serializeHeader,
				const iser::CArchiveTag& rootTag);

	bool InitStream(bool serializeHeader);
	bool InitArchive(QIODevice* devicePtr);
	bool InitArchive(QByteArray& inputString);
	bool WriteJsonHeader();
	bool Flush();

	/**
		Prepare the current tag for a new child element: open its JSON object if needed and write the separator.
	*/
	bool BeginChildElement();
	/**
		Write the key of a child tag, if the current tag is a JSON object.
	*/
	void WriteChildKey(const iser::CArchiveTag& tag);

	// reimplemented (iser::CTextWriteArchiveBase)
	virtual bool WriteTextNode(const QByteArray& text) override;

protected:
	QTextStream m_stream;
	QBuffer m_buffer;
	QJsonDocument::JsonFormat m_jsonFormat;
	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;

	struct TagsStackItem
	{
		const iser::CArchiveTag* m_tagPtr = nullptr;
		bool m_isMultiTag = false;
		bool m_isObjectOpened = false;	// opening brace of a non-leaf tag was written
		bool m_hasElements = false;		// child element written, next one needs a separator
		bool m_hasValue = false;		// primitive value written directly as value of this tag
	};

	bool m_quotationMarksRequired = false;

	QList<TagsStackItem> m_tagsStack;
};


} // namespace iser


