#ifndef BasePck_included
#define BasePck_included


#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "isvn/TApplicationInfoWrap.h"

#include "idoc/CSingleDocumentTemplateComp.h"
#include "idoc/CCompositeDocumentTemplateComp.h"

#include "istdc/TFileSerializerComp.h"
#include "istdc/CApplicationInfoComp.h"
#include "istdc/CComposedParamsSetComp.h"
#include "istdc/CRegistryCodeSaverComp.h"

#include "icam/CFileBitmapAcquisitionParams.h"


/**
	Base system-undependent general package.
*/
namespace BasePck
{


typedef istdc::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> XmlFileSerializer;
typedef isvn::TApplicationInfoWrap<istdc::CApplicationInfoComp> ApplicationInfo;
typedef icomp::TModelCompWrap<istdc::CComposedParamsSetComp> ComposedParamsSet;
typedef istdc::CRegistryCodeSaverComp RegistryCodeSaver;

typedef idoc::CSingleDocumentTemplateComp SingleDocumentTemplate;
typedef idoc::CCompositeDocumentTemplateComp CompositeDocumentTemplate;

typedef icomp::TModelCompWrap<icomp::TMakeComponentWrap<icam::IFileBitmapAcquisitionParams, icam::CFileBitmapAcquisitionParams> > FileBitmapAcquisitionParams;


} // namespace BasePck


#endif // !BasePck_included


