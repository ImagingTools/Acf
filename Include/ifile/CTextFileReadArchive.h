#ifndef ifile_CTextFileReadArchive_included
#define ifile_CTextFileReadArchive_included


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include "iser/CTextReadArchiveBase.h"
#include "ifile/CFileArchiveInfo.h"


namespace ifile
{


/**
	Simple implementation of archive reading from XML file.
	This implementation doesn't use any external parser.
	It uses only single pass and is very fast, but it doesn't support of tags skipping and
	it needs \c counter attribute indicating number of subtags for each mutli tag node.
	XML file in this format can be created using \c ifile::CTextFileWriteArchive.

\ingroup Persistence
*/
class CTextFileReadArchive:
	public iser::CTextReadArchiveBase,
	public CFileArchiveInfo
{
public:
	typedef iser::CTextReadArchiveBase BaseClass;
	typedef CFileArchiveInfo BaseClass2;

	explicit CTextFileReadArchive(const QString& filePath, bool serializeHeader = true, const iser::CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CTextFileReadArchive();

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);

	virtual bool Process(QString& value);
	virtual bool Process(QByteArray& value);

protected:
	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
		istd::IInformationProvider::InformationCategory category,
		int id,
		int flags,
		QString& message,
		QString& messageSource) const;

private:
	QFile m_file;
	QTextStream m_stream;
};


} // namespace ifile


#endif // !ifile_CTextFileReadArchive_included


