#ifndef BasePck_included
#define BasePck_included


#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "isvn/TApplicationInfoWrap.h"

#include "idoc/CSingleDocumentTemplateComp.h"
#include "idoc/CCompositeDocumentTemplateComp.h"

#include "istdc/TFileSerializerComp.h"
#include "istdc/CApplicationInfoComp.h"


/**
	Base system-undependent general package.
*/
namespace BasePck
{


typedef istdc::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> XmlFileSerializer;
typedef idoc::CSingleDocumentTemplateComp SingleDocumentTemplate;
typedef idoc::CCompositeDocumentTemplateComp CompositeDocumentTemplate;
typedef isvn::TApplicationInfoWrap<istdc::CApplicationInfoComp> ApplicationInfo;


} // namespace BasePck


#endif // !BasePck_included


