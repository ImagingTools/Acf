// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CCompactXmlFileWriteArchive.h>


// Qt includes
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


namespace ifile
{


// public methods

CCompactXmlFileWriteArchive::CCompactXmlFileWriteArchive(
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


CCompactXmlFileWriteArchive::~CCompactXmlFileWriteArchive()
{
	Flush();
}


bool CCompactXmlFileWriteArchive::OpenFile(const QString& filePath)
{
	m_file.setFileName(filePath);

	if (m_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Unbuffered)){
		return InitArchive(&m_file);
	}

	return false;
}


bool CCompactXmlFileWriteArchive::Flush()
{
	BaseClass::Flush();

	if (m_file.isOpen()){
		m_file.close();

		return true;
	}

	return false;
}


// protected methods

// reimplemented (iser::CTextWriteArchiveBase)

bool CCompactXmlFileWriteArchive::WriteTextNode(const QByteArray& text)
{
	if (!m_file.isOpen()){
		return false;
	}

	return BaseClass::WriteTextNode(text);
}


} // namespace ifile


