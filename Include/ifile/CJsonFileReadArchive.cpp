// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CJsonFileReadArchive.h>


namespace ifile
{


// public methods

CJsonFileReadArchive::CJsonFileReadArchive(const QString& filePath, bool serializeHeader)
	:BaseClass(serializeHeader)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath);
	}
}


bool CJsonFileReadArchive::OpenFile(const QString &filePath)
{
	if (m_file.isOpen()){
		m_file.close();
	}

	m_file.setFileName(filePath);
	if (!m_file.open(QIODevice::ReadOnly | QIODevice::Text)){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_FILE_OPEN_ERROR,
						QString("Cannot open file: %1").arg(m_file.errorString()),
						"JsonReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	if (!BaseClass::SetContent(&m_file)){
		m_file.close();

		return false;
	}

	return true;
}


bool CJsonFileReadArchive::IsOpen() const
{
	return m_file.isOpen();
}


// protected methods

// reimplemented (istd::ILogger)

void CJsonFileReadArchive::DecorateMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			int flags,
			QString& message,
			QString& messageSource) const
{
	BaseClass::DecorateMessage(category, id, flags, message, messageSource);

	message = m_file.fileName() + " : " + message;
}


} // namespace ifile
