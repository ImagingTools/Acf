#ifndef QtPck_included
#define QtPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqt/CSettingsSerializerComp.h"
#include "iqt/CApplicationSettingsProviderComp.h"
#include "iqt/CIniSettingsProviderComp.h"
#include "iqt/CClipboardSerializerComp.h"
#include "iqt/CTranslationManagerComp.h"
#include "iqt/CXmlFileReadArchive.h"
#include "iqt/CXmlFileWriteArchive.h"
#include "iqt/CCompactXmlFileReadArchive.h"
#include "iqt/CCompactXmlFileWriteArchive.h"

#include "iqtdoc/CExtendedDocumentTemplateComp.h"
#include "iqtdoc/CStaticHelpFileProviderComp.h"
#include "iqtdoc/CDoxygenHelpFileProviderComp.h"

#include "ifile/TXmlFileSerializerComp.h"


/**
	Standard Qt package.
*/
namespace QtPck
{



typedef iqt::CSettingsSerializerComp SettingsSerializer;
typedef iqt::CApplicationSettingsProviderComp ApplicationSettingsProvider;
typedef iqt::CIniSettingsProviderComp IniSettingsProvider;
typedef iqt::CClipboardSerializerComp ClipboardSerializer;
typedef icomp::TModelCompWrap<iqt::CTranslationManagerComp> TranslationManager;
typedef ifile::TXmlFileSerializerComp<iqt::CXmlFileReadArchive, iqt::CXmlFileWriteArchive> QtXmlFileSerializer;
typedef ifile::TXmlFileSerializerComp<iqt::CCompactXmlFileReadArchive, iqt::CCompactXmlFileWriteArchive> CompactXmlFileSerializer;

typedef iqtdoc::CExtendedDocumentTemplateComp ExtendedDocumentTemplate;
typedef iqtdoc::CStaticHelpFileProviderComp StaticHelpFileProvider;
typedef iqtdoc::CDoxygenHelpFileProviderComp DoxygenHelpFileProvider;


} // namespace QtPck


#endif // !QtPck_included


