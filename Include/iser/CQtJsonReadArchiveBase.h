// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonValue>
#include <QtCore/QList>
#include <QtCore/QIODevice>

// ACF includes
#include <iser/CTextReadArchiveBase.h>


namespace iser
{


/**
	Reads archive values from a JSON document.
*/
class CQtJsonReadArchiveBase : public iser::CTextReadArchiveBase
{
public:
	typedef CTextReadArchiveBase BaseClass;

	explicit CQtJsonReadArchiveBase(
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const override;
	virtual bool BeginTag(const iser::CArchiveTag& tag) override;
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count) override;
	virtual bool EndTag(const iser::CArchiveTag& tag) override;
	virtual bool Process(QString& value) override;
	using BaseClass::Process;

protected:
	bool SetContent(QIODevice* devicePtr);
	bool ReadStringNode(QString& text);

	// reimplemented (iser::CTextReadArchiveBase)
	virtual bool ReadTextNode(QByteArray& text) override;

private:
	struct Node
	{
		const iser::CArchiveTag* tagPtr = nullptr;
		QJsonValue value;
		int arrayIndex = 0;
		bool isArray = false;
		bool advanceParentArray = false;
	};

	QJsonValue GetCurrentValue() const;
	bool GetScalarText(const QJsonValue& value, QString& text) const;
	bool ReportTagError(const QString& message);

	QJsonDocument m_document;
	QJsonValue m_rootValue;
	iser::CArchiveTag m_rootTag;
	QList<Node> m_nodes;
	bool m_serializeHeader;
};


} // namespace iser
