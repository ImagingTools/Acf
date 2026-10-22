// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/CQtJsonReadArchiveBase.h>


namespace iser
{


// public methods

CQtJsonReadArchiveBase::CQtJsonReadArchiveBase(
			bool serializeHeader,
			const iser::CArchiveTag& /*rootTag*/)
	:m_rootTag("", "", iser::CArchiveTag::TT_GROUP),
	m_rootTagEnabled(false),
	m_serializeHeader(serializeHeader)
{
}


// reimplemented (iser::IArchive)

bool CQtJsonReadArchiveBase::BeginTag(const iser::CArchiveTag& tag)
{
	QString tagId(tag.GetId());
	int tagType = tag.GetTagType();

	if (m_iterators.isEmpty() && !tagId.isEmpty()){
		if (!BeginTag(m_rootTag)){
			return false;
		}

		m_rootTagEnabled = true;
	}

	if (m_iterators.isEmpty()){
		HelperIterator newHelperIterator;
		QJsonObject jsonObject = m_document.object();
		newHelperIterator.SetValue(jsonObject);
		newHelperIterator.SetKey(tagId);
		QString value = newHelperIterator.GetValue();
		m_iterators.push_back(newHelperIterator);
		m_tags.push_back(&tag);

		return true;
	}

	HelperIterator helperIterator = m_iterators.last();

	if (helperIterator.isArray()){
		if (tagType == iser::CArchiveTag::TT_LEAF){
			HelperIterator newHelperIterator;
			newHelperIterator.SetKey(tagId);
			QString value = helperIterator.GetValue();
			newHelperIterator.SetValue(value);
			m_iterators.push_back(newHelperIterator);
			m_tags.push_back(&tag);
		}
		else{
			QJsonObject jsonObject = helperIterator.GetObject();
			HelperIterator newHelperIterator;
			newHelperIterator.SetValue(jsonObject);
			newHelperIterator.SetKey(tagId);
			m_iterators.push_back(newHelperIterator);
			m_tags.push_back(&tag);
		}
	}
	else {
		QJsonObject jsonObject = helperIterator.GetObject();
		if (jsonObject.contains(tagId)){
			HelperIterator newHelperIterator;
			newHelperIterator.SetValue(jsonObject.value(tagId));
			newHelperIterator.SetKey(tagId);
			m_iterators.push_back(newHelperIterator);
			m_tags.push_back(&tag);
		}
		else{
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							MI_TAG_ERROR,
							QString("Tag '%1' not found!").arg(QString(tagId)),
							"CJsonStringReadArchive",
							istd::IInformationProvider::ITF_SYSTEM);
			}

			return false;
		}
	}

	return true;
}


bool CQtJsonReadArchiveBase::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& count)
{
	QString tagId(tag.GetId());

	if (m_iterators.isEmpty()){
		return false;
	}

	HelperIterator helperIterator = m_iterators.last();

	QJsonValue jsonValue = helperIterator.GetJsonValue();

	if (jsonValue.isObject()){
		QJsonObject jsonObject;
		jsonObject = jsonValue.toObject();

		if (jsonObject.contains(tagId)){
			QJsonValue value = jsonObject.value(tagId);
			if (value.isArray()){
				HelperIterator newHelperIterator;
				newHelperIterator.SetValue(value);
				newHelperIterator.SetKey(tagId);

				m_iterators.push_back(newHelperIterator);
				m_tags.push_back(&tag);

				count = value.toArray().count();
			}
		}
		else{
			if (IsLogConsumed()){
				SendLogMessage(
					istd::IInformationProvider::IC_ERROR,
					MI_TAG_ERROR,
					QString("Tag '%1' not found!").arg(QString(tagId)),
					"CJsonStringReadArchive",
					istd::IInformationProvider::ITF_SYSTEM);
			}

			return false;
		}

		return true;
	}
	else{
		HelperIterator newHelperIterator;
		newHelperIterator.SetValue(jsonValue);
		newHelperIterator.SetKey(tagId);
		m_iterators.push_back(newHelperIterator);
		m_tags.push_back(&tag);
		count = jsonValue.toArray().count();

		return true;
	}

	return false;
}


bool CQtJsonReadArchiveBase::EndTag(const iser::CArchiveTag& tag)
{
	if (m_iterators.isEmpty()){
		return false;
	}

	HelperIterator helperIterator = m_iterators.last();
	if (helperIterator.GetKey() == tag.GetId()){
		m_iterators.pop_back();
		m_tags.pop_back();
		if (!m_iterators.isEmpty() && m_iterators.last().isArray()){
			m_iterators.last().NextElementArray();
		}
	}
	else{
		SendLogMessage(
					istd::IInformationProvider::IC_ERROR,
					MI_TAG_ERROR,
					QString("Closing tag '%1' was not opened!").arg(QString(tag.GetId())),
					"JSON Reader",
					istd::IInformationProvider::ITF_SYSTEM);

		return false;
	}

	return true;
}


bool CQtJsonReadArchiveBase::Process(QString& value)
{
	return ReadStringNode(value);
}


// protected methods

bool CQtJsonReadArchiveBase::SetContent(QIODevice* devicePtr)
{
	Q_ASSERT(devicePtr != nullptr);

	QByteArray jsonData = devicePtr->readAll();

	QJsonParseError error;
	m_document = QJsonDocument::fromJson(jsonData, &error);

	if (error.error != QJsonParseError::NoError) {
		if (IsLogConsumed()) {
			SendLogMessage(
				istd::IInformationProvider::IC_ERROR,
				MI_TAG_ERROR,
				error.errorString(),
				"CJsonReadArchiveBase",
				istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	if (m_serializeHeader){
		SerializeAcfHeader();
	}

	return true;
}


bool CQtJsonReadArchiveBase::ReadStringNode(QString &text)
{
	bool openFakeTag = false;

	if (m_iterators.isEmpty() || m_tags.isEmpty()){
		return false;
	}

	if (m_iterators.last().isObject()){
		if (!BeginTag(*m_tags.last())){
			return false;
		}

		openFakeTag = true;
	}

	HelperIterator helperIterator = m_iterators.last();
	QJsonObject jsonObject;

	if (!helperIterator.isObject() && !helperIterator.isArray()){
		text = helperIterator.GetValue();
	}
	else{
		if (openFakeTag){
			EndTag(*m_tags.last());
		}
		return false;
	}

	if (openFakeTag){
		EndTag(*m_tags.last());
	}
	return true;
}


bool CQtJsonReadArchiveBase::ReadTextNode(QByteArray &text)
{
	QString stringText;

	if (ReadStringNode(stringText)){
		text = stringText.toUtf8();

		return true;
	}

	return false;
}

CQtJsonReadArchiveBase::HelperIterator::HelperIterator()
			: activeArrayIndex(0)
{

}

void CQtJsonReadArchiveBase::HelperIterator::SetKey(const QString &key)
{
	m_key = key;
}


const QString CQtJsonReadArchiveBase::HelperIterator::GetKey()
{
	return m_key;
}


QString CQtJsonReadArchiveBase::HelperIterator::GetValue()
{
	QString retVal;
	if (m_value.isBool()){
		retVal = m_value.toBool() ? "true" : "false";
	}
	else{
		if (m_value.isArray() && activeArrayIndex > -1 && activeArrayIndex < m_array.count()){
			retVal = m_array[activeArrayIndex].toVariant().toString();
		}
		else{
			retVal = m_value.toVariant().toString();
		}
	}

	return retVal;
}


QJsonObject CQtJsonReadArchiveBase::HelperIterator::GetObject()
{
	if (m_value.isObject()){

		return m_value.toObject();
	}

	if (m_value.isArray() && activeArrayIndex > -1 && activeArrayIndex < m_array.count()){

		return m_array[activeArrayIndex].toObject();
	}

	return QJsonObject();
}


QJsonValue CQtJsonReadArchiveBase::HelperIterator::GetJsonValue()
{
	if (m_value.isObject()){

		return m_value;
	}

	if (m_value.isArray() && activeArrayIndex > -1 && activeArrayIndex < m_array.count()){

		return m_array[activeArrayIndex];
	}

	return QJsonValue();
}


QJsonArray CQtJsonReadArchiveBase::HelperIterator::GetArray()
{
	if (isArray()){

		return m_array;
	}

	return QJsonArray();
}


bool CQtJsonReadArchiveBase::HelperIterator::NextElementArray()
{
	if (isArray()){
		activeArrayIndex++;

		return activeArrayIndex < m_array.count();
	}

	return false;
}


void CQtJsonReadArchiveBase::HelperIterator::SetValue(const QJsonValue value)
{
	m_value = value;

	if (m_value.isArray()){
		m_array = m_value.toArray();
		activeArrayIndex = 0;
	}
}


bool CQtJsonReadArchiveBase::HelperIterator::isObject()
{
	return m_value.isObject();
}


bool CQtJsonReadArchiveBase::HelperIterator::isArray()
{
	return m_value.isArray();
}


} // namespace iser
