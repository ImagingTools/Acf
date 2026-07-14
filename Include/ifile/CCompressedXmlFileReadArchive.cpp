// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CCompressedXmlFileReadArchive.h>


// include STL
#include <cstring>

// Qt includes
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>


namespace ifile
{


CCompressedXmlFileReadArchive::CCompressedXmlFileReadArchive(
			const QString& filePath,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(serializeHeader, rootTag)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath);
	}
}


bool CCompressedXmlFileReadArchive::OpenFile(const QString& filePath)
{
	m_openFileName = filePath;

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly)){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_FILE_OPEN_ERROR,
						QString("Cannot open file: %1").arg(file.errorString()),
						"CompressedXmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		m_openFileName = "";

		return false;
	}

	QByteArray uncompressedData = qUncompress(file.readAll());

	m_buffer.setData(uncompressedData);

	if (!BaseClass::SetContent(&m_buffer)){
		file.close();
		m_openFileName = "";

		return false;
	}

	return true;
}


bool CCompressedXmlFileReadArchive::IsOpen() const
{
	return !m_openFileName.isEmpty();
}


// protected methods

// reimplemented (istd::ILogger)

void CCompressedXmlFileReadArchive::DecorateMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			int flags,
			QString& message,
			QString& messageSource) const
{
	BaseClass::DecorateMessage(category, id, flags, message, messageSource);

	message = QObject::tr("%2 : %1").arg(message).arg(m_openFileName);
}


} // namespace ifile

