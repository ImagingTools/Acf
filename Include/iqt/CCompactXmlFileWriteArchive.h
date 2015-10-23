#ifndef iqt_CCompactXmlFileWriteArchive_included
#define iqt_CCompactXmlFileWriteArchive_included


#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QFile>

// ACF includes
#include "ifile/CFileArchiveInfo.h"
#include "iqt/CCompactXmlWriteArchiveBase.h"


namespace iqt
{


/**
	Qt-based implementation of archive for writing in compact XML format.

	\ingroup Persistence
*/
class CCompactXmlFileWriteArchive:
			public CCompactXmlWriteArchiveBase,
			public ifile::CFileArchiveInfo
{
public:
	typedef CCompactXmlWriteArchiveBase BaseClass;
	typedef ifile::CFileArchiveInfo BaseClass2;

	/**
		Constructor initializing archive to open file immediatelly.
	*/
	CCompactXmlFileWriteArchive(
				const QString& filePath,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	~CCompactXmlFileWriteArchive();

	bool OpenFile(const QString& filePath);

	bool Flush();

private:
	QFile m_file;
};


} // namespace iqt


#endif // !iqt_CCompactXmlFileWriteArchive_included


