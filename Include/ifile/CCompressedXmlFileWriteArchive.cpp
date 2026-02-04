// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CCompressedXmlFileWriteArchive.h>


// Qt includes
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


namespace ifile
{


CCompressedXmlFileWriteArchive::CCompressedXmlFileWriteArchive(
			const QString& filePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, serializeHeader, rootTag),
	BaseClass2(filePath)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath);
	}
}


CCompressedXmlFileWriteArchive::~CCompressedXmlFileWriteArchive()
{
	Flush();
}


bool CCompressedXmlFileWriteArchive::OpenFile(const QString& filePath)
{
	m_file.setFileName(filePath);

	if (m_file.open(QIODevice::WriteOnly)){
		if (m_buffer.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Unbuffered)){
			return InitArchive(&m_buffer);
		}
	}

	return false;
}


bool CCompressedXmlFileWriteArchive::Flush()
{
	BaseClass::Flush();

	if (m_file.isOpen()){
		QByteArray data = qCompress(m_buffer.buffer());

		bool retVal  = (m_file.write(data) > 0);

		m_file.close();

		return retVal;
	}

	return false;
}


} // namespace ifile


