#include "ifile/CTextFileReadArchive.h"


namespace ifile
{


CTextFileReadArchive::CTextFileReadArchive(const QString& filePath, bool serializeHeader, const iser::CArchiveTag& rootTag)
:	BaseClass(),
	BaseClass2(filePath),
	m_file(filePath)
{
	if (m_file.open(QIODevice::ReadOnly | QIODevice::Text)){
		m_stream.setDevice(&m_file);

		if (serializeHeader){
			SerializeAcfHeader();
		}
	}
}


CTextFileReadArchive::~CTextFileReadArchive()
{
	if (m_file.isOpen()){
		m_stream.setDevice(NULL);

		m_file.close();
	}
}


// reimplemented (iser::IArchive)

bool CTextFileReadArchive::BeginTag(const iser::CArchiveTag& tag)
{
	return true;
}


bool CTextFileReadArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	return true;
}


bool CTextFileReadArchive::EndTag(const iser::CArchiveTag& tag)
{
	return true;
}


bool CTextFileReadArchive::Process(QString& value)
{
	m_stream >> value;
	return true;
}


bool CTextFileReadArchive::Process(QByteArray& value)
{
	m_stream >> value;
	return true;
}


// protected methods

// reimplemented (istd::ILogger)

void CTextFileReadArchive::DecorateMessage(
	istd::IInformationProvider::InformationCategory /*category*/,
	int /*id*/,
	int /*flags*/,
	QString& message,
	QString& /*messageSource*/) const
{
	message = m_filePath + " : " + message;
}


} // namespace ifile


