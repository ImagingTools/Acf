// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/CJsonMemWriteArchive.h>


namespace iser
{


// public methods

CJsonMemWriteArchive::CJsonMemWriteArchive(
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
	:BaseClass(versionInfoPtr, serializeHeader, rootTag)
{
	if (m_textBuffer.open(QIODevice::WriteOnly | QIODevice::Text)){
		InitArchive(&m_textBuffer);
	}
}


const QByteArray& CJsonMemWriteArchive::GetData() const
{
	const_cast<CJsonMemWriteArchive*>(this)->Flush();

	return m_textBuffer.buffer();
}


} // namespace iser


