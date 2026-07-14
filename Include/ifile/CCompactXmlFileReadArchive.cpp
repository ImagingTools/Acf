// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CCompactXmlFileReadArchive.h>


// include STL
#include <cstring>

// Qt includes
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>


namespace ifile
{


CCompactXmlFileReadArchive::CCompactXmlFileReadArchive(
			const QString& filePath,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(serializeHeader, rootTag),
	BaseClass2(filePath)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath);
	}
}


bool CCompactXmlFileReadArchive::OpenFile(const QString& filePath)
{
	m_openFileName = filePath;

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_FILE_OPEN_ERROR,
						QString("Cannot open file: %1").arg(file.errorString()),
						"CompactXmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		m_openFileName = "";

		return false;
	}

	if (!BaseClass::SetContent(&file)){
		file.close();
		m_openFileName = "";

		return false;
	}

	return true;
}


bool CCompactXmlFileReadArchive::IsOpen() const
{
	return !m_openFileName.isEmpty();
}


// protected methods

// reimplemented (istd::ILogger)

void CCompactXmlFileReadArchive::DecorateMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			int flags,
			QString& message,
			QString& messageSource) const
{
	BaseClass::DecorateMessage(category, id, flags, message, messageSource);

	int lineNumber = m_currentParent.lineNumber();
	if (lineNumber >= 0){
		message = QObject::tr("%2(%3) : %1").arg(message).arg(m_openFileName).arg(lineNumber);
	}
	else{
		message = QObject::tr("%2 : %1").arg(message).arg(m_openFileName);
	}
}


} // namespace ifile
