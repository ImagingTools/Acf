#ifndef TCalibratedViewBase_included
#define TCalibratedViewBase_included


#include "iview/TFramedViewBase.h"


namespace iview
{


template <class ViewBase>
class TCalibratedViewBase: public iview::TFramedViewBase<ViewBase>
{
public:
	typedef iview::TFramedViewBase<ViewBase> BaseClass;

	TCalibratedViewBase(Frame* framePtr)
		:BaseClass(framePtr)
	{
	}
	template <typename ConstructorParam>
	TCalibratedViewBase(Frame* framePtr, ConstructorParam* param)
		:BaseClass(framePtr, param)
	{
	}

	// reimplemented (iview::CCalibratedViewBase)
	virtual void ConnectCalibrationShape(iview::IShape* shapePtr);
};


// public methods

template <class ViewBase>
void TCalibratedViewBase<ViewBase>::ConnectCalibrationShape(iview::IShape* shapePtr)
{
	BaseClass::ConnectCalibrationShape(shapePtr);

	Frame* framePtr = GetFramePtr();
	if (framePtr != NULL){
		framePtr->UpdateButtonsState();
	}
}


} // namespace iview


#endif // !TCalibratedViewBase_included


