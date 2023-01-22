#include <iser/CJsonReadArchiveBase.h>


namespace iser
{


// public methods

CJsonReadArchiveBase::CJsonReadArchiveBase()
			: CTextReadArchiveBase(),
			m_rootTag("", "", iser::CArchiveTag::TT_GROUP)
{

}


// reimplemented (iser::IArchive)

bool CJsonReadArchiveBase::BeginTag(const iser::CArchiveTag& tag)
{
	QString tagId(tag.GetId());

	int tagType = tag.GetTagType();

	if (m_iterators.isEmpty()){
		if (&tag != &m_rootTag){

			return false;
		}

		HelperIterator newHelperIterator;
		bool isObject = m_document.isObject();
		QJsonObject jsonObject = m_document.object();
		newHelperIterator.SetValue(jsonObject);
		QString value = newHelperIterator.GetValue();
		m_iterators.push_back(newHelperIterator);

		return true;
	}

	HelperIterator helperIterator = m_iterators.last();

	if (helperIterator.isArray()){
		if (tagType == iser::CArchiveTag::TT_LEAF){

				return false;
		}
	}

	QJsonObject jsonObject = helperIterator.GetObject();
	if (jsonObject.contains(tagId)){
		HelperIterator newHelperIterator;
		newHelperIterator.SetValue(jsonObject.value(tagId));
		newHelperIterator.SetKey(tagId);
		m_iterators.push_back(newHelperIterator);
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


bool CJsonReadArchiveBase::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	QString tagId(tag.GetId());
	HelperIterator helperIterator = m_iterators.last();
	QJsonObject jsonObject;

	if (helperIterator.isObject()){
		jsonObject = helperIterator.GetObject();
	}
	else{

		return false;
	}

	if (jsonObject.contains(tagId)){
		HelperIterator newHelperIterator;
		newHelperIterator.SetValue(jsonObject.value(tagId));
		newHelperIterator.SetKey(tagId);
		m_iterators.push_back(newHelperIterator);
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


bool CJsonReadArchiveBase::EndTag(const iser::CArchiveTag& tag)
{

	if (m_iterators.isEmpty()){

		return false;
	}

	HelperIterator helperIterator = m_iterators.last();
	if (helperIterator.GetKey() == tag.GetId()){
		m_iterators.pop_back();
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


bool CJsonReadArchiveBase::Process(QString& value)
{
	return ReadStringNode(value);
}


// protected methods

bool CJsonReadArchiveBase::InitArchive(const QByteArray &inputString)
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

		return false;
	}

	BeginTag(m_rootTag);

	return true;
}


bool CJsonReadArchiveBase::ReadStringNode(QString &text)
{
	HelperIterator helperIterator = m_iterators.last();
	QJsonObject jsonObject;

	if (!helperIterator.isObject() && !helperIterator.isArray()){
		text = helperIterator.GetValue();
	}
	else{
		return false;
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


