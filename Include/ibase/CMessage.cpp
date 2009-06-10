#include "ibase/CMessage.h"


#include "isys/IDateTime.h"

#include "iser/IArchive.h"


namespace ibase
{		


CMessage::CMessage()
{
	m_time.SetCurrentTime();
}


CMessage::CMessage(
			istd::ILogger::MessageCategory category,
			int id,
			const istd::CString& text,
			const istd::CString& source,
			int flags)
:	m_category(category),
	m_id(id),
	m_text(text),
	m_source(source),
	m_flags(flags)
{
	m_time.SetCurrentTime();
}


void CMessage::SetCategory(istd::ILogger::MessageCategory category)
{
	m_category = category;
}


void CMessage::SetText(const istd::CString& text)
{
	m_text = text;
}


void CMessage::SetSource(const istd::CString& source)
{
	m_source = source;
}


// reimplemented (iser::ISerializable)

bool CMessage::Serialize(iser::IArchive& archive)
{
	int category = m_category;

	static iser::CArchiveTag categoryTag("Category", "Message category");
	bool retVal = archive.BeginTag(categoryTag);
	retVal = retVal && archive.Process(category);
	retVal = retVal && archive.EndTag(categoryTag);

	if (!archive.IsStoring()){
		m_category = istd::ILogger::MessageCategory(category);
	}

	static iser::CArchiveTag textTag("Text", "Message text");
	retVal = retVal && archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	static iser::CArchiveTag sourceTag("Source", "Message source");
	retVal = retVal && archive.BeginTag(sourceTag);
	retVal = retVal && archive.Process(m_source);
	retVal = retVal && archive.EndTag(sourceTag);

	static iser::CArchiveTag timeStampTag("Timestamp", "Message time stamp");
	retVal = retVal && archive.BeginTag(timeStampTag);
	retVal = retVal && m_time.Serialize(archive);
	retVal = retVal && archive.EndTag(timeStampTag);

	return retVal;
}


} // namespace ibase

