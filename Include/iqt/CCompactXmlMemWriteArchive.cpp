#include "iqt/CCompactXmlMemWriteArchive.h"


// Qt includes
#include <QtXml/QDomNodeList>
#include <QtCore/QTextStream>


namespace iqt
{


CCompactXmlMemWriteArchive::CCompactXmlMemWriteArchive(
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, serializeHeader, rootTag)
{
}


QByteArray CCompactXmlMemWriteArchive::GetString() const
{
	return m_document.toByteArray(4);
}


} // namespace iqt


