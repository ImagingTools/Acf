#ifndef iqt_CXmlFileWriteArchive_included
#define iqt_CXmlFileWriteArchive_included


#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QFile>

#include "iser/CTextWriteArchiveBase.h"
#include "ifile/CFileArchiveInfo.h"
#include "iser/CXmlDocumentInfoBase.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Qt-based implementation of archive for writing in XML format.
	Please note that it doesn't create \c counter attribute needed by \c ifile::CXmlFileReadArchive.

	\ingroup Persistence
*/
class CXmlFileWriteArchive:
			public iser::CTextWriteArchiveBase,
			public ifile::CFileArchiveInfo,
			public iser::CXmlDocumentInfoBase
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;
	typedef ifile::CFileArchiveInfo BaseClass2;

	CXmlFileWriteArchive(
				const QString& filePath = "",
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	~CXmlFileWriteArchive();

	bool Flush();

	bool OpenFile(const QString& filePath);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(QString& value);
	using BaseClass::Process;

protected:
	bool WriteStringNode(const QString& text);

	// reimplemented (iser::CTextWriteArchiveBase)
	bool WriteTextNode(const QByteArray& text);

private:
	QDomDocument m_document;
	QDomElement m_currentParent;

	QFile m_file;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;

	bool m_isSeparatorNeeded;
};


} // namespace iqt


#endif // !iqt_CXmlFileWriteArchive_included


