#include <ifile/CJsonFileReadArchive.h>


// Qt inclides
#include <QtCore/QFile>


namespace ifile
{


// public methods

CJsonFileReadArchive::CJsonFileReadArchive(const QString& filePath, const iser::IVersionInfo* infoPtr)
	:BaseClass()
{
	OpenFile(filePath);
}


bool CJsonFileReadArchive::OpenFile(const QString &filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly)){
		return false;
	}

	QByteArray inputString = file.readAll();

	return InitArchive(inputString);
}


} // namespace iser


