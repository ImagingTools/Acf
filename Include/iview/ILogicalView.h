#ifndef iview_ILogicalView_included
#define iview_ILogicalView_included



#include "iview/IShapeView.h"
#include "iview/ILogicalCoords.h"



namespace iview{



/**	Comon interface for all views with logical transform support.
*/
class  ILogicalView: public IShapeView, public ILogicalCoords
{
public:
	/**	Set logical to view units transformation.
	*/
	virtual void SetLogToViewTransform(const i2d::CAffine2d& transform) = 0;
};



} // namespace iview



#endif // !iview_ILogicalView_included



