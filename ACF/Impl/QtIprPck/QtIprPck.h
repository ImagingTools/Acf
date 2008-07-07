#ifndef QtIprPck_included
#define QtIprPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtipr/CRectangularFilterParamsGuiComp.h"
#include "iqtipr/CIterativeProcessorParamsGuiComp.h"


/**
	Standard Qt image processing package.
*/
namespace QtIprPck
{


typedef iqtipr::CRectangularFilterParamsGuiComp RectangularFilterParamsGui;
typedef iqtipr::CIterativeProcessorParamsGuiComp IterationParamsGui;


} // namespace QtIprPck


#endif // !QtIprPck_included

