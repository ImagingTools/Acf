#pragma once


// ACF includes
#include <iser/CJsonReadArchiveBase.h>


namespace ifile
{


class CJsonFileReadArchive: public iser::CJsonReadArchiveBase
{
public:
	typedef iser::CJsonReadArchiveBase BaseClass;

	CJsonFileReadArchive(const QString& filePath = "", const iser::IVersionInfo* infoPtr = nullptr);

	bool OpenFile(const QString& filePath);
};


} // namespace ifile


