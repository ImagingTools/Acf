#include <iser/CJsonMemWriteArchive.h>


// Qt includes
#include <QtCore/QJsonDocument>


namespace iser
{


// public methods

CJsonMemWriteArchive::CJsonMemWriteArchive(
			QByteArray &inputString,
			const IVersionInfo* versionInfoPtr,
			QJsonDocument::JsonFormat /*jsonFormat*/)
	:BaseClass(versionInfoPtr)
{
	bool serializeHeader = versionInfoPtr != nullptr;

	InitArchive(inputString, serializeHeader);
}


CJsonMemWriteArchive::~CJsonMemWriteArchive()
{
	Flush();
}


} // namespace iser


