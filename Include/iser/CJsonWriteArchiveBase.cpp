#include <iser/CJsonWriteArchiveBase.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>


namespace iser
{


// public methods

CJsonWriteArchiveBase::CJsonWriteArchiveBase(
			const IVersionInfo *versionInfoPtr,
			QJsonDocument::JsonFormat jsonFormat)
			:  CTextWriteArchiveBase(versionInfoPtr),
			m_jsonFormat(jsonFormat)

{

}


CJsonWriteArchiveBase::~CJsonWriteArchiveBase()
{
	m_stream->flush();
}


// reimplemented (iser::IArchive)

bool CJsonWriteArchiveBase::IsTagSkippingSupported() const
{
	return true;
}


bool CJsonWriteArchiveBase::BeginTag(const CArchiveTag& tag)
{
	m_isSeparatorNeeded = false;

	m_currentAttribute.clear();

	if (m_tagsStack.isEmpty() || (m_tagsStack.back() == NULL) || (m_tagsStack.back()->GetTagType() != iser::CArchiveTag::TT_MULTIPLE)){
		int tagType = tag.GetTagType();
		if (m_allowAttribute && (tagType == iser::CArchiveTag::TT_LEAF)){
			m_currentAttribute = tag.GetId();

			WriteTag(tag, "");

			m_tagsStack.push_back(NULL);

			return true;
		}
		else if (tagType == iser::CArchiveTag::TT_WEAK){
			m_tagsStack.push_back(NULL);

			WriteTag(tag, "{");

			return true;
		}
	}

	WriteTag(tag, "{");

	m_firstTag = true;

	m_tagsStack.push_back(&tag);

	m_allowAttribute = true;

	return true;
}


bool CJsonWriteArchiveBase::BeginMultiTag(const CArchiveTag &tag, const CArchiveTag &subTag, int &count)
{
	WriteTag(tag,"[");
	m_firstTag = true;

	m_tagsStack.push_back(&tag);

	m_allowAttribute = true;

	m_isSeparatorNeeded = false;

	return true;
}

bool CJsonWriteArchiveBase::EndTag(const CArchiveTag &tag)
{
	Q_ASSERT(m_stream.IsValid());
	if (!m_stream.IsValid()){

		return false;
	}

	m_currentAttribute.clear();

	if (m_tagsStack.isEmpty()){
		return false;
	}

	const iser::CArchiveTag* lastTagPtr = m_tagsStack.back();
	m_tagsStack.pop_back();

	if (lastTagPtr == NULL){
		return true;
	}

	if (lastTagPtr->GetTagType() == iser::CArchiveTag::TT_MULTIPLE){
		*m_stream << "]";
	}
	else if (lastTagPtr->GetTagType() == iser::CArchiveTag::TT_GROUP){
		*m_stream << "}";
	}
	m_firstTag = false;

	return true;
}


bool CJsonWriteArchiveBase::Process(QString &value)
{
	return WriteTextNode("\"" + value.toUtf8() + "\"");
}


// protected methods

bool CJsonWriteArchiveBase::InitStream()
{
	Q_ASSERT(m_stream.IsValid());
	if (!m_stream.IsValid()){
		return false;
	}

#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
	m_stream->setCodec("UTF-8");
#endif
	m_firstTag = true;

	return true;
}


bool CJsonWriteArchiveBase::InitArchive(QIODevice *devicePtr)
{
	m_stream.SetPtr(new QTextStream(devicePtr));
	
	return InitStream();
}


bool CJsonWriteArchiveBase::InitArchive(QByteArray &inputString)
{
	m_stream.SetPtr(new QTextStream(&inputString));
	
	return InitStream();
}


void CJsonWriteArchiveBase::WriteTag(const CArchiveTag &tag, QString separator, bool isWriteTag)
{
	Q_ASSERT(m_stream.IsValid());
	if (!m_stream.IsValid()){
		return;
	}

	if (!m_firstTag){
		*m_stream << ",";
	}

	if (isWriteTag && !tag.GetId().isEmpty()){
		*m_stream << "\"" << tag.GetId() << "\":";
	}

	*m_stream << separator;

	m_firstTag = false;
}

// reimplemented (iser::CTextWriteArchiveBase)

bool CJsonWriteArchiveBase::WriteTextNode(const QByteArray &text)
{
	Q_ASSERT(m_stream.IsValid());
	if (!m_stream.IsValid()){
		return false;
	}

	*m_stream << text;

	return true;
}


} // namespace iser


