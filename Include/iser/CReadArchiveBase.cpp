// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/CReadArchiveBase.h>


namespace iser
{


// reimplemented (iser::IArchive)

bool CReadArchiveBase::IsStoring() const
{
	return false;
}


const IVersionInfo& CReadArchiveBase::GetVersionInfo() const
{
	return m_versionInfo;
}


bool CReadArchiveBase::ProcessBits(void* dataPtr, int bitsCount, int bytesCount)
{
	if (bytesCount == int(sizeof(quint32))){
		quint32 mask = (1 << bitsCount) - 1;
		quint32 word = *((quint32*)dataPtr);
		quint32 result = word & mask;

		return Process(result);
	}

	return false;
}


// protected methods

bool CReadArchiveBase::SerializeAcfHeader()
{
	return m_versionInfo.SerializeArchiveHeader(*this);
}


} // namespace iser


