#include "iser/CMemoryWriteArchive.h"
#include "istd/CString.h"


namespace iser
{


CMemoryWriteArchive::CMemoryWriteArchive(): CBinaryWriteArchiveBase()
{
	CBinaryArchiveBase::Process(m_version);
}


bool CMemoryWriteArchive::ProcessData(void* data, int size)
{
	if (data == NULL || size <= 0){
		return false;
	}

	m_memory.insert(m_memory.end(), (char*)data, (char*)data + size);

	return true;
}


} // namespace iser

