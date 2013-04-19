#include "ifile/CTextFileWriteArchive.h"


namespace ifile
{


CTextFileWriteArchive::CTextFileWriteArchive(
	const QString& filePath,
	const iser::IVersionInfo* versionInfoPtr,
	bool serializeHeader,
	const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr),
	BaseClass2(filePath),
	m_file(filePath)
{
	if (m_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
		m_stream.setDevice(&m_file);

		if (serializeHeader){
			SerializeAcfHeader();
		}
	}
	else{
#if QT_VERSION >= 0x040700
		m_stream.setStatus(QTextStream::WriteFailed);
#endif
	}
}


CTextFileWriteArchive::~CTextFileWriteArchive()
{
	if (m_file.isOpen()){
		m_file.close();
	}
}


// reimplemented (iser::IArchive)

bool CTextFileWriteArchive::BeginTag(const iser::CArchiveTag& tag)
{
	return true;
}


bool CTextFileWriteArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	return true;
}


bool CTextFileWriteArchive::EndTag(const iser::CArchiveTag& tag)
{
	m_stream << "\t";
	return true;
}


bool CTextFileWriteArchive::Process(QString& value)
{
	return WriteString(value.toAscii());
}


bool CTextFileWriteArchive::Process(QByteArray& value)
{
	return WriteString(value);
}


// protected methods

// reimplemented (iser::CTextWriteArchiveBase)

bool CTextFileWriteArchive::WriteString(const QByteArray& value)
{
	m_stream << value;

	return m_stream.status() == QTextStream::Ok;
}


} // namespace ifile


