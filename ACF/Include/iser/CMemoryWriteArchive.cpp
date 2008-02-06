#include "iser/CMemoryWriteArchive.h"


#include "istd/CString.h"


namespace iser
{


CMemoryWriteArchive::CMemoryWriteArchive(
			const IVersionInfo* versionInfoPtr,
			bool serializeHeader)
:	BaseClass(versionInfoPtr)
{
	if (serializeHeader){
		SerializeHeader();
	}
}


const void* CMemoryWriteArchive::GetBuffer() const
{
	return &m_memory[0];
}


int CMemoryWriteArchive::GetBufferSize() const
{
	return int(m_memory.size());
}


// reimplemented (iser::IArchive)

bool CMemoryWriteArchive::ProcessData(void* data, int size)
{
	if (data == NULL || size <= 0){
		return false;
	}

	m_memory.insert(m_memory.end(), (char*)data, (char*)data + size);

	return true;
}


} // namespace iser


