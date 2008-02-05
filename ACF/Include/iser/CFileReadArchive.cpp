#include <assert.h>

#include "ser/filereadarchive.h"
#include "istd/CString.h"


namespace iser
{


CFileReadArchive::CFileReadArchive(const CString& fileName): CBinaryReadArchiveBase()
{
	m_stream.open(fileName.ascii().c_str(), std::fstream::in | std::fstream::binary);

	CBinaryArchiveBase::Process(m_version);
}


CFileReadArchive::~CFileReadArchive()
{
	m_stream.close();
}


bool CFileReadArchive::ProcessData(void* data, int size)
{
	if (data != NULL && size > 0){
		m_stream.read((char*)data, size);
	}

	return !m_stream.fail();
}


int CFileReadArchive::tagSize()
{
	int size = 0;
	m_stream.read((char*)&size, sizeof(int));
	
	return m_stream.fail()? 0 : size;
}


} // namespace iser

