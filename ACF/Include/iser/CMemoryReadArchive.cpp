#include "iser/CMemoryReadArchive.h"


#include "istd/CString.h"

#include "iser/CMemoryWriteArchive.h"


namespace iser
{


CMemoryReadArchive::CMemoryReadArchive(
			const void* bufferPtr,
			int bufferSize,
			bool serializeHeader)
:	m_bufferPtr((const I_BYTE*)bufferPtr),
	m_bufferSize(bufferSize),
	m_readPosition(0)
{
	if (serializeHeader){
		SerializeHeader();
	}
}


CMemoryReadArchive::CMemoryReadArchive(
			const CMemoryWriteArchive& writeArchive,
			bool serializeHeader)
:	m_bufferPtr((const I_BYTE*)writeArchive.GetBuffer()),
	m_bufferSize(writeArchive.GetBufferSize()),
	m_readPosition(0)
{
	if (serializeHeader){
		SerializeHeader();
	}
}


// reimplemented (iser::IArchive)

bool CMemoryReadArchive::ProcessData(void* data, int size)
{
	if (m_readPosition + size > m_bufferSize){
		return false;
	}

	::memcpy(data, m_bufferPtr + m_readPosition, size);
	m_readPosition += size;

	return true;
}


} // namespace iser


