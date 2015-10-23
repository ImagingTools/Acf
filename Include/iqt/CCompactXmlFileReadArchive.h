#ifndef iqt_CCompactXmlFileReadArchive_included
#define iqt_CCompactXmlFileReadArchive_included


// Qt includes
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

// ACF includes
#include "iser/CXmlDocumentInfoBase.h"
#include "iser/CTextReadArchiveBase.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Qt-based implementation of archive reading from XML file.

	\ingroup Persistence
*/
class CCompactXmlFileReadArchive: public iser::CTextReadArchiveBase, public iser::CXmlDocumentInfoBase
{
public:
	CCompactXmlFileReadArchive(
				const QString& filePath = "",
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	bool OpenFile(const QString& filePath);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(QString& value);
	using BaseClass::Process;

protected:
	bool ReadStringNode(QString& text);

	// reimplemented (iser::CTextReadArchiveBase)
	virtual bool ReadTextNode(QByteArray& text);

	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;

private:
	QDomDocument m_document;
	QDomElement m_currentParent;

	QByteArray m_currentAttribute;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;

	typedef QList<const iser::CArchiveTag*> TagsList;
	TagsList m_tagsStack;

	QString m_openFileName;
};


} // namespace iqt


#endif // !iqt_CCompactXmlFileReadArchive_included


