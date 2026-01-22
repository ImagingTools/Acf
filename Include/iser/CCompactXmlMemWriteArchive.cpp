// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/CCompactXmlMemWriteArchive.h>


// Qt includes
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


namespace iser
{


CCompactXmlMemWriteArchive::CCompactXmlMemWriteArchive(
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, serializeHeader, rootTag)
{
	if (m_textBuffer.open(QIODevice::WriteOnly | QIODevice::Text)){
		InitArchive(&m_textBuffer);
	}
}


const QByteArray& CCompactXmlMemWriteArchive::GetString() const
{
	const_cast<CCompactXmlMemWriteArchive*>(this)->Flush();

	return m_textBuffer.buffer();
}


} // namespace iser


