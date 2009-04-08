#ifndef CmpstrPck_included
#define CmpstrPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "icmpstr/CAttributeEditorComp.h"
#include "icmpstr/CPackageOverviewComp.h"
#include "icmpstr/CRegistryLoaderComp.h"
#include "icmpstr/CRegistryModelComp.h"
#include "icmpstr/CRegistryPreviewComp.h"
#include "icmpstr/CRegistryViewComp.h"
#include "icmpstr/CComponentHelpFileProviderComp.h"


/**
	Component manipulation package.
	This package is used to contruct Compositor tool.
*/
namespace CmpstrPck
{


typedef icmpstr::CAttributeEditorComp AttributeEditor;
typedef icmpstr::CPackageOverviewComp PackageOverview;
typedef icmpstr::CRegistryLoaderComp RegistryLoader;
typedef icomp::TModelCompWrap<icmpstr::CRegistryModelComp> RegistryModel;
typedef icmpstr::CRegistryPreviewComp RegistryPreview;
typedef icmpstr::CRegistryViewComp RegistryView;
typedef icmpstr::CComponentHelpFileProviderComp ComponentHelpFileProvider;


} // namespace CmpstrPck


#endif // !CmpstrPck_included


