// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CFileReadArchive.h>


// Qt includes
#include <QtCore/QString>


// ACF includes
#include <iser/CArchiveTag.h>


namespace ifile
{


CFileReadArchive::CFileReadArchive(const QString& filePath, bool supportTagSkipping, bool serializeHeader)
:	BaseClass2(filePath),
	m_supportTagSkipping(supportTagSkipping)
{
	if (!filePath.isEmpty() && OpenFile(filePath) && serializeHeader){
		SerializeAcfHeader();
	}
}


bool CFileReadArchive::OpenFile(const QString& filePath)
{
	if (m_file.isOpen()){
		m_file.close();
	}

	m_filePath = filePath;
	m_file.setFileName(filePath);
	if (!m_file.open(QIODevice::ReadOnly)){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_FILE_OPEN_ERROR,
						QString("Cannot open file: %1").arg(m_file.errorString()),
						"BinaryReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	return true;
}


bool CFileReadArchive::IsOpen() const
{
	return m_file.isOpen();
}


bool CFileReadArchive::IsTagSkippingSupported() const
{
	return m_supportTagSkipping;
}


bool CFileReadArchive::BeginTag(const iser::CArchiveTag& tag)
{
	bool retVal = BaseClass::BeginTag(tag);

	if (!retVal){
		return false;
	}

	m_tagStack.push_back(TagStackElement());
	TagStackElement& element = m_tagStack.back();

	element.tagBinaryId = tag.GetBinaryId();

	retVal = retVal && Process(element.endPosition);
	element.useTagSkipping = tag.IsTagSkippingUsed() && m_supportTagSkipping;

	return retVal;
}


bool CFileReadArchive::EndTag(const iser::CArchiveTag& tag)
{
	TagStackElement& element = m_tagStack.back();

	bool retVal = (element.tagBinaryId == tag.GetBinaryId());

	if (!retVal){
		qFatal("BeginTag and EndTag have to use the same tag");

		return false;
	}

	if (element.useTagSkipping && (element.endPosition != 0)){
		retVal = retVal && m_file.seek(element.endPosition);
	}

	m_tagStack.pop_back();

	retVal = retVal && BaseClass::EndTag(tag);

	return retVal;
}


bool CFileReadArchive::ProcessData(void* data, int size)
{
	if (size <= 0){
		return true;
	}

	if (data == nullptr) {
		return false;
	}

	char* buffer = static_cast<char*>(data);
	int totalRead = 0;

	while (totalRead < size) {
		qint64 bytesRead = m_file.read(buffer + totalRead, size - totalRead);
		if (bytesRead <= 0) {
			// Read failed or EOF reached before reading all data
			return false;
		}
		totalRead += bytesRead;
	}

	return m_file.error() == QFile::NoError;
}


// protected methods

// reimplemented (istd::ILogger)

void CFileReadArchive::DecorateMessage(
			istd::IInformationProvider::InformationCategory /*category*/,
			int /*id*/,
			int /*flags*/,
			QString& message,
			QString& /*messageSource*/) const
{
	message = m_filePath + " : " + message;
}


// reimplemented (iser::CArchiveBase)

int CFileReadArchive::GetMaxStringLength() const
{
	return int(m_file.size() - m_file.pos());
}


} // namespace ifile

