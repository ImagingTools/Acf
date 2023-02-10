#pragma once


// ACF includes
#include <iser/CJsonWriteArchiveBase.h>


namespace iser
{


/**
	Implementation of an ACF Archive serializing to JSON string
*/
class CJsonMemWriteArchive: public CJsonWriteArchiveBase
{
public:
	typedef CJsonWriteArchiveBase BaseClass;

	CJsonMemWriteArchive(
				QByteArray& inputString,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				QJsonDocument::JsonFormat jsonFormat = QJsonDocument::Compact);
	~CJsonMemWriteArchive();
};


} // namespace iser


