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
		SerializeAcfHeader();
	}
}


const void* CMemoryWriteArchive::GetBuffer() const
{
	return &m_dataBuffer[0];
}


int CMemoryWriteArchive::GetBufferSize() const
{
	return int(m_dataBuffer.size());
}


// reimplemented (iser::IArchive)

bool CMemoryWriteArchive::ProcessData(void* data, int size)
{
	if (data == NULL || size <= 0){
		return false;
	}

	DataBuffer::size_type previousSize = m_dataBuffer.size();

	m_dataBuffer.resize(previousSize + size);

	::memcpy(&m_dataBuffer[previousSize], data, size);

	return true;
}


} // namespace iser


