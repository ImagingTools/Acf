#include <iser/CJsonReadArchiveBase.h>


namespace iser
{


// public methods

// reimplemented (iser::IArchive)

bool CJsonReadArchiveBase::BeginTag(const iser::CArchiveTag& tag)
{
	Q_ASSERT(m_currentAttribute.isEmpty());

	QString tagId(tag.GetId());

	m_currentAttribute.clear();

	if (m_tagsStack.isEmpty() || (m_tagsStack.back() == NULL) || (m_tagsStack.back()->GetTagType() != iser::CArchiveTag::TT_MULTIPLE)){
		int tagType = tag.GetTagType();
		if (m_allowAttribute && (tagType == iser::CArchiveTag::TT_LEAF)){
			m_currentAttribute = tag.GetId();

			m_tagsStack.push_back(NULL);

			return true;
		}
		else if (tagType == iser::CArchiveTag::TT_WEAK){
			m_tagsStack.push_back(NULL);

			return true;
		}
	}
	QJsonObject::const_iterator objIterator;

	objIterator = m_objectsStack.last();

	QJsonObject jsonObject = (*objIterator).toObject();
	objIterator = jsonObject.find(tagId);
	if (objIterator != jsonObject.end() && jsonObject.value(tagId).isObject()){
		m_objectsStack.push_back(objIterator);
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

	m_tagsStack.push_back(&tag);

	m_allowAttribute = true;

	return true;
}


bool CJsonReadArchiveBase::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	QString tagId(tag.GetId());
	QJsonObject::const_iterator objIterator;

	objIterator = m_objectsStack.last();

	QJsonObject jsonObject = (*objIterator).toObject();
	objIterator = jsonObject.find(tagId);
	if (objIterator != jsonObject.end() && jsonObject.value(tagId).isArray()){
		m_objectsStack.push_back(objIterator);
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

	count = jsonObject.value(tagId).toArray().count();

	m_tagsStack.push_back(&tag);

	m_allowAttribute = true;

	return true;
}


bool CJsonReadArchiveBase::EndTag(const iser::CArchiveTag& tag)
{
	m_currentAttribute.clear();

	if (m_tagsStack.isEmpty()){
		return false;
	}

	const iser::CArchiveTag* lastTagPtr = m_tagsStack.back();
	m_tagsStack.pop_back();

	if (lastTagPtr == NULL){
		return true;
	}

	if (&tag != lastTagPtr){
		SendLogMessage(
					istd::IInformationProvider::IC_ERROR,
					MI_TAG_ERROR,
					QString("Closing tag '%1' was not opened!").arg(QString(tag.GetId())),
					"JSON Reader",
					istd::IInformationProvider::ITF_SYSTEM);

		return false;
	}

	QString tagId(tag.GetId());
	QJsonObject::const_iterator objIterator;

	objIterator = m_objectsStack.last();
	QJsonObject jsonObject = (*objIterator).toObject();

	Q_ASSERT (jsonObject.contains(lastTagPtr->GetId()));

	m_objectsStack.pop_back();

	if (m_isNewFormat){
		m_allowAttribute = false;
	}

	return true;
}


bool CJsonReadArchiveBase::Process(QString& value)
{
	return ReadStringNode(value);
}


// protected methods

void CJsonReadArchiveBase::initArchive(const QByteArray &inputString)
{
	QJsonParseError error;
	m_document = QJsonDocument::fromJson(inputString, &error);
	if (error.error != QJsonParseError::NoError && IsLogConsumed()){
		SendLogMessage(
					istd::IInformationProvider::IC_ERROR,
					MI_TAG_ERROR,
					error.errorString(),
					"CJsonStringReadArchive",
					istd::IInformationProvider::ITF_SYSTEM);
	}
}


bool CJsonReadArchiveBase::ReadStringNode(QString &text)
{
	if (m_currentAttribute.isEmpty()){
		return false;
	}
	else{
		QJsonObject::const_iterator objIterator;

		objIterator = m_objectsStack.last();
		QJsonObject jsonObject = (*objIterator).toObject();

		if (jsonObject.contains(m_currentAttribute)){
			text = jsonObject.value(m_currentAttribute).toString();
		}
		else{
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							MI_TAG_ERROR,
							QString("No attribute '%1' found!").arg(QString(m_currentAttribute)),
							"JSON Reader",
							istd::IInformationProvider::ITF_SYSTEM);
			}

			return false;
		}
	}

	return true;
}


bool CJsonReadArchiveBase::ReadTextNode(QByteArray &text)
{
	QString stringText;

	if (ReadStringNode(stringText)){
		text = stringText.toLocal8Bit();

		return true;
	}

	return false;
}


} // namespace iser


