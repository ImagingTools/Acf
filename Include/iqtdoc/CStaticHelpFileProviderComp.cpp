#include "iqtdoc/CStaticHelpFileProviderComp.h"


// QT includes
#include <QFileInfo>
#include <QDir>

// ACF includes
#include "iqt/iqt.h"


namespace iqtdoc
{


// reimplemented (idoc::IHelpFileProvider)

double CStaticHelpFileProviderComp::GetHelpQuality(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	istd::CString filePath = GetHelpFilePath(contextText, contextObjectPtr);
	if (filePath.IsEmpty()){
		return 0;
	}

	QFileInfo fileInfo(iqt::GetQString(filePath));
	if (fileInfo.isFile()){
		return 1;
	}
	else{
		return 0;
	}
}


istd::CString CStaticHelpFileProviderComp::GetHelpFilePath(const istd::CString& contextText, const istd::IPolymorphic* /*contextObjectPtr*/) const
{
	if (!m_helpFileDirCompPtr.IsValid()){
		return istd::CString::GetEmpty();
	}

	QDir helpDir(iqt::GetQString(m_helpFileDirCompPtr->GetPath()));
	QString fileName;
	if (contextText.IsEmpty()){
		fileName = iqt::GetQString(*m_defaultFileNameAttrPtr);
	}
	else{
		fileName = iqt::GetQString(*m_contextPrefixAttrPtr + contextText + *m_contextSuffixAttrPtr);
	}

	return iqt::GetCString(helpDir.absoluteFilePath(fileName));
}


} // namespace iqtdoc


