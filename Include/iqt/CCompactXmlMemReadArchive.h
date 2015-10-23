#ifndef iqt_CCompactXmlMemReadArchive_included
#define iqt_CCompactXmlMemReadArchive_included


// Qt includes
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

// ACF includes
#include "iqt/CCompactXmlReadArchiveBase.h"


namespace iqt
{


/**
	Qt-based implementation of archive reading from XML file.

	\ingroup Persistence
*/
class CCompactXmlMemReadArchive: public CCompactXmlReadArchiveBase
{
public:
	typedef CCompactXmlReadArchiveBase BaseClass;

	/**
		Constructor initializing archive from memory.
	*/
	CCompactXmlMemReadArchive(
				const QByteArray& inputString,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

protected:
	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;
};


} // namespace iqt


#endif // !iqt_CCompactXmlMemReadArchive_included


