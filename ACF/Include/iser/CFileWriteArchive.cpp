#include "ser/filewritearchive.h"
#include "istd/CString.h"


namespace iser
{


CFileWriteArchive::CFileWriteArchive(const CString& fileName)
	:CBinaryWriteArchiveBase()
{
	m_stream.open(fileName.ascii().c_str(), std::fstream::out | std::fstream::binary);
	CBinaryArchiveBase::Process(m_version);
}


CFileWriteArchive::~CFileWriteArchive()
{
	Flush();
	m_stream.close();
}


bool CFileWriteArchive::ProcessData(void* data, int size)
{
	if (data != NULL && size > 0){
		m_stream.write((char*)data, size);
	}

	return !m_stream.fail();
}


void CFileWriteArchive::Flush()
{
	m_stream.flush();
}


} // namespace iser

