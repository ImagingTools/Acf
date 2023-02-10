#include <ifile/CJsonFileWriteArchive.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>


namespace ifile
{


// public methods

CJsonFileWriteArchive::CJsonFileWriteArchive(const QString& filePath, const iser::IVersionInfo* infoPtr)
		: CJsonWriteArchiveBase()
{
	OpenFile(filePath);
}

CJsonFileWriteArchive::~CJsonFileWriteArchive()
{
	if (m_file.isOpen()){
		m_file.close();
	}
}


bool CJsonFileWriteArchive::OpenFile(const QString &filePath)
{
	m_file.setFileName(filePath);

	if (m_file.open(QIODevice::WriteOnly)){
		InitArchive(&m_file);
		return true;
	}

	return false;
}


} // namespace ifile


