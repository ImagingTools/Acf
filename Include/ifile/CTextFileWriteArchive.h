#ifndef ifile_CTextFileWriteArchive_included
#define ifile_CTextFileWriteArchive_included


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include "iser/CTextWriteArchiveBase.h"
#include "ifile/CFileArchiveInfo.h"


namespace ifile
{


/**
	Simple implementation of archive for writing in XML format.
	This implementation doesn't use any external parser.
	It creates \c counter attribute indicating number of subtags for each mutli tag node.
	XML file in this format are needed by \c ifile::CTextFileReadArchive.

	\ingroup Persistence
*/
class CTextFileWriteArchive:
	public iser::CTextWriteArchiveBase,
	public CFileArchiveInfo
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;
	typedef CFileArchiveInfo BaseClass2;

	CTextFileWriteArchive(
		const QString& filePath,
		const iser::IVersionInfo* versionInfoPtr = NULL,
		bool serializeHeader = true,
		const iser::CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CTextFileWriteArchive();

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);

	virtual bool Process(QString& value);
	virtual bool Process(QByteArray& value);

protected:
	// reimplemented (iser::CTextWriteArchiveBase)
	virtual bool WriteString(const QByteArray& value);

private:
	QTextStream m_stream;
	QFile m_file;
};


} // namespace ifile


#endif // !ifile_CTextFileWriteArchive_included


