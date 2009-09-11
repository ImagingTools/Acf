#ifndef QtMmPck_included
#define QtMmPck_included


#include "icomp/TModelCompWrap.h"

#include "iqtmm/CVideoWidgetGuiComp.h"
#include "iqtmm/CMediaObjectComp.h"
#include "iqtmm/CMediaObjectLoaderComp.h"


/**
	Qt based multimedia package.
*/
namespace QtMmPck
{


typedef iqtmm::CVideoWidgetGuiComp VideoView;
typedef icomp::TModelCompWrap<iqtmm::CMediaObjectComp> MediaObject;
typedef iqtmm::CMediaObjectLoaderComp MediaLoader;


} // namespace QtMmPck


#endif // !QtMmPck_included


