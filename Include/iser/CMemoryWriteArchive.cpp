// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/CMemoryWriteArchive.h>


// STL includes
#include <cstring>


namespace iser
{


CMemoryWriteArchive::CMemoryWriteArchive(
			const IVersionInfo* versionInfoPtr,
			bool serializeHeader)
:	BaseClass(versionInfoPtr), m_serializeHeader(serializeHeader)
{
	Reset();
}


const void* CMemoryWriteArchive::GetBuffer() const
{
	return m_dataBuffer.constData();
}


int CMemoryWriteArchive::GetBufferSize() const
{
	return int(m_dataBuffer.size());
}


void CMemoryWriteArchive::Reset()
{
	m_dataBuffer.clear();

	if (m_serializeHeader){
		SerializeAcfHeader();
	}
}


// reimplemented (iser::IArchive)

bool CMemoryWriteArchive::ProcessData(void* data, int size)
{
	if (size <= 0){
		return true;
	}

	if (data == NULL){
		return false;
	}

	m_dataBuffer.append((const char*)data, size);

	return true;
}


} // namespace iser


