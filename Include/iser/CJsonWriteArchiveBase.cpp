// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/CJsonWriteArchiveBase.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

// ACF includes
#include <iser/CArchiveHeaderInfo.h>


namespace iser
{


namespace
{


QByteArray EscapeJsonString(const QByteArray& value)
{
	QByteArray escapedValue;
	escapedValue.reserve(value.size() * 2);

	static const char hexDigits[] = "0123456789ABCDEF";

	for (char currentByte : value){
		const unsigned char byte = static_cast<unsigned char>(currentByte);

		switch (currentByte){
			case '\\':
				escapedValue += "\\\\";
				break;
			case '\"':
				escapedValue += "\\\"";
				break;
			case '\b':
				escapedValue += "\\b";
				break;
			case '\f':
				escapedValue += "\\f";
				break;
			case '\n':
				escapedValue += "\\n";
				break;
			case '\r':
				escapedValue += "\\r";
				break;
			case '\t':
				escapedValue += "\\t";
				break;
			default:
				// Escape ASCII control characters (0x00-0x1F).
				if (byte < 0x20){
					escapedValue += "\\u00";
					escapedValue += hexDigits[byte >> 4];
					escapedValue += hexDigits[byte & 0x0F];
				}
				else{
					escapedValue += currentByte;
				}
				break;
		}
	}

	return escapedValue;
}


bool IsObjectTag(int tagType)
{
	return
				(tagType == iser::CArchiveTag::TT_GROUP)
				|| (tagType == iser::CArchiveTag::TT_WEAK)
				|| (tagType == iser::CArchiveTag::TT_UNKNOWN);
}


} // namespace


// protected methods

CJsonWriteArchiveBase::CJsonWriteArchiveBase(
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& /*rootTag*/)
	:BaseClass(versionInfoPtr),
	m_jsonFormat(QJsonDocument::Indented),
	m_serializeHeader(serializeHeader),
	m_rootTag("", "", iser::CArchiveTag::TT_GROUP)
{
}


// public methods

CJsonWriteArchiveBase::~CJsonWriteArchiveBase()
{
}


void CJsonWriteArchiveBase::SetFormat(QJsonDocument::JsonFormat jsonFormat)
{
	m_jsonFormat = jsonFormat;
}


// reimplemented (iser::IArchive)

bool CJsonWriteArchiveBase::IsTagSkippingSupported() const
{
	return true;
}


bool CJsonWriteArchiveBase::BeginTag(const CArchiveTag& tag)
{
	int tagType = tag.GetTagType();
	if ((tagType != iser::CArchiveTag::TT_LEAF) && !IsObjectTag(tagType)){
		return false;
	}

	TagsStackItem newItem;
	newItem.m_tagPtr = &tag;

	if (m_tagsStack.isEmpty()){
		// The root tag is always a JSON object.
		m_stream << "{";
		newItem.m_isObjectOpened = true;
	}
	else{
		if (!BeginChildElement()){
			return false;
		}

		WriteChildKey(tag);

		// The opening brace of a non-leaf tag is postponed until its first child tag,
		// so a single primitive value can be written directly as the value of this tag.
	}

	m_tagsStack.push_back(newItem);

	return true;
}


bool CJsonWriteArchiveBase::BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& /*subTag*/, int&/*count*/)
{
	if (m_tagsStack.isEmpty() || !BeginChildElement()){
		return false;
	}

	WriteChildKey(tag);

	m_stream << "[";

	TagsStackItem newItem;
	newItem.m_tagPtr = &tag;
	newItem.m_isMultiTag = true;

	m_tagsStack.push_back(newItem);

	return true;
}


bool CJsonWriteArchiveBase::EndTag(const CArchiveTag& /*tag*/)
{
	if (m_tagsStack.isEmpty()){
		return false;
	}

	TagsStackItem lastItem = m_tagsStack.last();
	m_tagsStack.pop_back();

	if (lastItem.m_tagPtr == nullptr){
		return false;
	}

	if (lastItem.m_isMultiTag){
		m_stream << "]";
	}
	else if (lastItem.m_isObjectOpened){
		m_stream << "}";
	}
	else if (!lastItem.m_hasValue){
		// Nothing was written for this tag, the key must still get a valid value.
		if (IsObjectTag(lastItem.m_tagPtr->GetTagType())){
			m_stream << "{}";
		}
		else{
			m_stream << "null";
		}
	}

	return true;
}


bool CJsonWriteArchiveBase::Process(QString &value)
{
	QByteArray valueData = value.toUtf8();

	return Process(valueData);
}


bool CJsonWriteArchiveBase::Process(QByteArray &value)
{
	m_quotationMarksRequired = true;

	return WriteTextNode(EscapeJsonString(value));
}


bool CJsonWriteArchiveBase::ProcessData(void* dataPtr, int size)
{
	m_quotationMarksRequired = true;

	return BaseClass::ProcessData(dataPtr, size);
}


// protected methods

bool CJsonWriteArchiveBase::InitStream(bool serializeHeader)
{
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
	m_stream.setCodec("UTF-8");
#endif

	WriteJsonHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}

	return true;
}


bool CJsonWriteArchiveBase::InitArchive(QIODevice* devicePtr)
{
	m_stream.setDevice(devicePtr);
	
	return InitStream(m_serializeHeader);
}


bool CJsonWriteArchiveBase::InitArchive(QByteArray& inputString)
{
	m_buffer.setBuffer(&inputString);
	if (m_buffer.open(QIODevice::WriteOnly | QIODevice::Text)){
		m_stream.setDevice(&m_buffer);
	}
	
	return InitStream(m_serializeHeader);
}


bool CJsonWriteArchiveBase::WriteJsonHeader()
{
	return BeginTag(m_rootTag);
}


bool CJsonWriteArchiveBase::Flush()
{
	QIODevice* devicePtr = m_stream.device();
	if (devicePtr != nullptr){
		if (devicePtr->isOpen()){
			bool retVal = EndTag(m_rootTag);

			devicePtr->close();

			return retVal;
		}
	}

	return false;
}


bool CJsonWriteArchiveBase::BeginChildElement()
{
	Q_ASSERT(!m_tagsStack.isEmpty());

	TagsStackItem& parentItem = m_tagsStack.last();

	if (!parentItem.m_isMultiTag){
		if (!IsObjectTag(parentItem.m_tagPtr->GetTagType())){
			// A leaf tag cannot contain child tags.
			return false;
		}

		if (!parentItem.m_isObjectOpened){
			if (parentItem.m_hasValue){
				// The value of this tag was already written directly, it cannot become an object anymore.
				return false;
			}

			m_stream << "{";
			parentItem.m_isObjectOpened = true;
		}
	}

	if (parentItem.m_hasElements){
		m_stream << ",";
	}

	parentItem.m_hasElements = true;

	return true;
}


void CJsonWriteArchiveBase::WriteChildKey(const iser::CArchiveTag& tag)
{
	Q_ASSERT(!m_tagsStack.isEmpty());

	if (!m_tagsStack.last().m_isMultiTag && !tag.GetId().isEmpty()){
		m_stream << "\"" << EscapeJsonString(tag.GetId()) << "\":";
	}
}


// reimplemented (iser::CTextWriteArchiveBase)

bool CJsonWriteArchiveBase::WriteTextNode(const QByteArray &text)
{
	bool quotationMarksRequired = m_quotationMarksRequired;
	m_quotationMarksRequired = false;

	if (m_tagsStack.isEmpty()){
		return false;
	}

	TagsStackItem& lastItem = m_tagsStack.last();

	if (lastItem.m_isMultiTag){
		// Value written directly as an array element.
		if (lastItem.m_hasElements){
			m_stream << ",";
		}

		lastItem.m_hasElements = true;
	}
	else if (lastItem.m_isObjectOpened){
		// The tag contains child tags already, so the value is stored under the key of the tag itself.
		// This is the layout the older versions used for all values of non-leaf tags.
		if (lastItem.m_hasElements){
			m_stream << ",";
		}

		m_stream << "\"" << EscapeJsonString(lastItem.m_tagPtr->GetId()) << "\":";

		lastItem.m_hasElements = true;
	}
	else if (lastItem.m_hasValue){
		// Only one primitive value can be stored as value of a tag.
		return false;
	}
	else{
		lastItem.m_hasValue = true;
	}

	if (quotationMarksRequired){
		m_stream << "\"";
	}

	m_stream << text;
	
	if (quotationMarksRequired){
		m_stream << "\"";
	}

	return true;
}


} // namespace iser


