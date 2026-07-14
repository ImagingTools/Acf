// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CSimpleXmlFileReadArchive.h>


namespace ifile
{


CSimpleXmlFileReadArchive::CSimpleXmlFileReadArchive(const QString& filePath, bool serializeHeader, const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag),
	BaseClass2(filePath)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath, serializeHeader);
	}
}


bool CSimpleXmlFileReadArchive::OpenFile(const QString& filePath, bool serializeHeader)
{
	if (m_file.isOpen()){
		m_stream.setDevice(NULL);
		m_file.close();
	}

	m_filePath = filePath;
	m_file.setFileName(filePath);
	if (!m_file.open(QIODevice::ReadOnly | QIODevice::Text)){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_FILE_OPEN_ERROR,
						QString("Cannot open file: %1").arg(m_file.errorString()),
						"SimpleXmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	m_stream.setDevice(&m_file);
	ReadXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}

	return true;
}


bool CSimpleXmlFileReadArchive::IsOpen() const
{
	return m_file.isOpen();
}


CSimpleXmlFileReadArchive::~CSimpleXmlFileReadArchive()
{
	if (m_file.isOpen()){
		ReadXmlFooter();

		m_stream.setDevice(NULL);

		m_file.close();
	}
}


// protected methods

// reimplemented (istd::ILogger)

void CSimpleXmlFileReadArchive::DecorateMessage(
			istd::IInformationProvider::InformationCategory /*category*/,
			int /*id*/,
			int /*flags*/,
			QString& message,
			QString& /*messageSource*/) const
{
	message = m_filePath + "(" + QString("%1").arg(GetLastReadLine()) + ") : " + message;
}


} // namespace ifile

