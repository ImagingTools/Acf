// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/CQtJsonReadArchiveBase.h>


// Qt includes
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>


namespace iser
{


CQtJsonReadArchiveBase::CQtJsonReadArchiveBase(
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
	:m_rootValue(QJsonObject()),
	m_rootTag(rootTag),
	m_serializeHeader(serializeHeader)
{
}


bool CQtJsonReadArchiveBase::IsTagSkippingSupported() const
{
	return true;
}


bool CQtJsonReadArchiveBase::BeginTag(const iser::CArchiveTag& tag)
{
	const bool parentIsArray = !m_nodes.isEmpty() && m_nodes.last().isArray;
	QJsonValue parentValue = GetCurrentValue();
	QJsonValue value;

	if (parentIsArray){
		const QJsonArray parentArray = m_nodes.last().value.toArray();
		const int index = m_nodes.last().arrayIndex;
		if (index < 0 || index >= parentArray.size()){
			return ReportTagError(QString("No element remains in array for tag '%1'").arg(QString::fromUtf8(tag.GetId())));
		}

		value = parentArray.at(index);
	}
	else if (parentValue.isObject()){
		const QJsonObject parentObject = parentValue.toObject();
		const QString tagId = QString::fromUtf8(tag.GetId());
		if (parentObject.contains(tagId)){
			value = parentObject.value(tagId);
		}
		else if (tag.GetTagType() == iser::CArchiveTag::TT_WEAK){
			value = parentValue;
		}
		else{
			return ReportTagError(QString("Tag '%1' not found").arg(tagId));
		}
	}
	else{
		return ReportTagError(QString("Cannot find tag '%1' in a non-object value").arg(QString::fromUtf8(tag.GetId())));
	}

	Node node;
	node.tagPtr = &tag;
	node.value = value;
	node.advanceParentArray = parentIsArray;
	m_nodes.push_back(node);

	return true;
}


bool CQtJsonReadArchiveBase::BeginMultiTag(
			const iser::CArchiveTag& tag,
			const iser::CArchiveTag& /*subTag*/,
			int& count)
{
	const bool parentIsArray = !m_nodes.isEmpty() && m_nodes.last().isArray;
	QJsonValue value;

	if (parentIsArray){
		const QJsonArray parentArray = m_nodes.last().value.toArray();
		const int index = m_nodes.last().arrayIndex;
		if (index < 0 || index >= parentArray.size()){
			return ReportTagError(QString("No element remains in array for tag '%1'").arg(QString::fromUtf8(tag.GetId())));
		}

		value = parentArray.at(index);
	}
	else{
		const QJsonValue parentValue = GetCurrentValue();
		if (parentValue.isObject()){
			const QJsonObject parentObject = parentValue.toObject();
			const QString tagId = QString::fromUtf8(tag.GetId());
			if (!parentObject.contains(tagId)){
				return ReportTagError(QString("Tag '%1' not found").arg(tagId));
			}

			value = parentObject.value(tagId);
		}
		else if (parentValue.isArray()){
			value = parentValue;
		}
		else{
			return ReportTagError(QString("Cannot find array tag '%1' in a non-container value").arg(QString::fromUtf8(tag.GetId())));
		}
	}

	if (!value.isArray()){
		return ReportTagError(QString("Tag '%1' is not an array").arg(QString::fromUtf8(tag.GetId())));
	}

	Node node;
	node.tagPtr = &tag;
	node.value = value;
	node.isArray = true;
	node.advanceParentArray = parentIsArray;
	m_nodes.push_back(node);
	count = value.toArray().size();

	return true;
}


bool CQtJsonReadArchiveBase::EndTag(const iser::CArchiveTag& tag)
{
	if (m_nodes.isEmpty()){
		return false;
	}

	const Node node = m_nodes.last();
	if (node.tagPtr != &tag){
		return ReportTagError(QString("Closing tag '%1' does not match the opened tag").arg(QString::fromUtf8(tag.GetId())));
	}

	m_nodes.pop_back();

	if (node.advanceParentArray && !m_nodes.isEmpty() && m_nodes.last().isArray){
		++m_nodes.last().arrayIndex;
	}

	return true;
}


bool CQtJsonReadArchiveBase::Process(QString& value)
{
	return ReadStringNode(value);
}


bool CQtJsonReadArchiveBase::SetContent(QIODevice* devicePtr)
{
	if (devicePtr == nullptr){
		return false;
	}

	QJsonParseError parseError;
	m_document = QJsonDocument::fromJson(devicePtr->readAll(), &parseError);
	if (parseError.error != QJsonParseError::NoError){
		return ReportTagError(parseError.errorString());
	}

	if (m_document.isObject()){
		m_rootValue = m_document.object();
		const QString rootTagId = QString::fromUtf8(m_rootTag.GetId());
		const QJsonObject rootObject = m_document.object();
		if (!rootTagId.isEmpty() && rootObject.value(rootTagId).isObject()){
			m_rootValue = rootObject.value(rootTagId);
		}
	}
	else if (m_document.isArray()){
		m_rootValue = m_document.array();
	}
	else{
		return ReportTagError("JSON document must contain an object or an array");
	}

	m_nodes.clear();

	return !m_serializeHeader || SerializeAcfHeader();
}


bool CQtJsonReadArchiveBase::ReadStringNode(QString& text)
{
	return GetScalarText(GetCurrentValue(), text);
}


bool CQtJsonReadArchiveBase::ReadTextNode(QByteArray& text)
{
	QString stringValue;
	if (!ReadStringNode(stringValue)){
		return false;
	}

	text = stringValue.toUtf8();

	return true;
}


QJsonValue CQtJsonReadArchiveBase::GetCurrentValue() const
{
	if (m_nodes.isEmpty()){
		return m_rootValue;
	}

	const Node& node = m_nodes.last();
	if (node.isArray){
		const QJsonArray array = node.value.toArray();
		if (node.arrayIndex < 0 || node.arrayIndex >= array.size()){
			return QJsonValue(QJsonValue::Undefined);
		}

		return array.at(node.arrayIndex);
	}

	return node.value;
}


bool CQtJsonReadArchiveBase::GetScalarText(const QJsonValue& value, QString& text) const
{
	if (value.isString()){
		text = value.toString();
		return true;
	}

	if (value.isBool()){
		text = value.toBool() ? QStringLiteral("true") : QStringLiteral("false");
		return true;
	}

	if (value.isDouble()){
		text = QString::number(value.toDouble(), 'g', 17);
		return true;
	}

	return false;
}


bool CQtJsonReadArchiveBase::ReportTagError(const QString& message)
{
	if (IsLogConsumed()){
		SendLogMessage(
					istd::IInformationProvider::IC_ERROR,
					MI_TAG_ERROR,
					message,
					"Qt JSON Reader",
					istd::IInformationProvider::ITF_SYSTEM);
	}

	return false;
}


} // namespace iser
