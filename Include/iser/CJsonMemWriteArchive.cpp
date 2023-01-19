#include <iser/CJsonMemWriteArchive.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>


namespace iser
{


// public methods


CJsonMemWriteArchive::CJsonMemWriteArchive(
			QByteArray &inputString,
			const IVersionInfo* versionInfoPtr,
			QJsonDocument::JsonFormat jsonFormat)
	:BaseClass(versionInfoPtr)
{
	InitArchive(inputString);
}


} // namespace iser


