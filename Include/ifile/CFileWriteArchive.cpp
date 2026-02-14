// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CFileWriteArchive.h>


// Qt includes
#include <QtCore/QString>


namespace ifile
{


CFileWriteArchive::CFileWriteArchive(
			const QString& filePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool supportTagSkipping,
			bool serializeHeader)
:	BaseClass(versionInfoPtr),
	BaseClass2(filePath),
	m_file(filePath),
	m_devicePtr(nullptr),
	m_supportTagSkipping(supportTagSkipping),
	m_isValid(false)
{
	m_isValid = m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


CFileWriteArchive::CFileWriteArchive(
			QIODevice& device,
			const iser::IVersionInfo* versionInfoPtr,
			bool supportTagSkipping,
			bool serializeHeader)
:	BaseClass(versionInfoPtr),
	BaseClass2(QString()),
	m_devicePtr(&device),
	m_supportTagSkipping(supportTagSkipping),
	m_isValid(true)
{
	if (serializeHeader){
		SerializeAcfHeader();
	}
}


void CFileWriteArchive::Flush()
{
	GetDevice()->flush();
}


// reimplemented (iser::IArchive)

bool CFileWriteArchive::IsTagSkippingSupported() const
{
	return m_supportTagSkipping;
}


bool CFileWriteArchive::BeginTag(const iser::CArchiveTag& tag)
{
	bool retVal = m_isValid && BaseClass::BeginTag(tag);

	if (!retVal){
		return false;
	}

	QIODevice* device = GetDevice();
	m_tagStack.push_back(TagStackElement());
	TagStackElement& element = m_tagStack.back();

	element.tagBinaryId = tag.GetBinaryId();
	element.endFieldPosition = (tag.IsTagSkippingUsed() && m_supportTagSkipping)?
				quint32(device->pos()):
				quint32(0);

	quint32 dummyPos = 0;
	retVal = retVal && Process(dummyPos);

	return retVal;
}


bool CFileWriteArchive::EndTag(const iser::CArchiveTag& tag)
{
	TagStackElement& element = m_tagStack.back();

	bool retVal = (element.tagBinaryId == tag.GetBinaryId());

	if (!retVal){
		qFatal("BeginTag and EndTag have to use the same tag");

		return false;
	}

	if (element.endFieldPosition != 0){	// add position of the file tag end to the tag begin
		QIODevice* device = GetDevice();
		quint32 endPosition = device->pos();

		retVal = retVal && device->seek(element.endFieldPosition);

		retVal = retVal && Process(endPosition);

		retVal = retVal && device->seek(endPosition);
	}

	m_tagStack.pop_back();

	return retVal && BaseClass::EndTag(tag);
}


bool CFileWriteArchive::ProcessData(void* data, int size)
{
	if (size <= 0) {
		return true; // Nothing to write, not an error
	}

	if (data == nullptr) {
		return false; // Invalid data pointer
	}

	QIODevice* device = GetDevice();
	const char* buffer = static_cast<const char*>(data);
	int totalWritten = 0;

	while (totalWritten < size) {
		qint64 bytesWritten = device->write(buffer + totalWritten, size - totalWritten);
		if (bytesWritten <= 0) {
			// Write failed
			return false;
		}
		totalWritten += bytesWritten;
	}

	return device->error() == QIODevice::NoError;
}


// private methods

QIODevice* CFileWriteArchive::GetDevice()
{
	return m_devicePtr ? m_devicePtr : &m_file;
}


const QIODevice* CFileWriteArchive::GetDevice() const
{
	return m_devicePtr ? m_devicePtr : &m_file;
}


} // namespace ifile

