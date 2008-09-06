#ifndef AvtPck_included
#define AvtPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "icam/CTriggerParamsComp.h"

#include "iavt/CFireGrabAcquisitionComp.h"



/**
	AVT-specific package.
*/
namespace AvtPck
{


typedef iavt::CFireGrabAcquisitionComp FireGrabAcquisition;
typedef icomp::TModelCompWrap<icam::CTriggerParamsComp> TriggerParams;


} // namespace AvtPck


#endif // !AvtPck_included


