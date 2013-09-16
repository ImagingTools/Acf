#include "iview/CPerspectiveCalibrationBoundsShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "iqt/iqt.h"

#include "iview/CCalibratedViewBase.h"
#include "iview/CPerspectiveCalibration.h"


namespace iview
{


// reimplemented (iview::IInteractiveShape)

iview::ITouchable::TouchState CPerspectiveCalibrationBoundsShape::IsTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const CPerspectiveCalibration* calibPtr = dynamic_cast<const CPerspectiveCalibration*>(GetModelPtr());
	if (calibPtr != NULL){
		const i2d::CRectangle& bounds = calibPtr->GetBounds();
		i2d::CVector2d viewLeftTop, viewRightTop, viewLeftBottom, viewRightBottom;

		calibPtr->GetInvPositionAt(bounds.GetLeftTop(), viewLeftTop);
		calibPtr->GetInvPositionAt(bounds.GetRightTop(), viewRightTop);
		calibPtr->GetInvPositionAt(bounds.GetLeftBottom(), viewLeftBottom);
		calibPtr->GetInvPositionAt(bounds.GetRightBottom(), viewRightBottom);

		i2d::CVector2d viewPos = GetLogPosition(i2d::CVector2d(position));

		i2d::CLine2d viewLeft(viewLeftTop, viewLeftBottom);
		i2d::CLine2d viewRight(viewRightTop, viewRightBottom);
		i2d::CLine2d viewTop(viewLeftTop, viewRightTop);
		i2d::CLine2d viewBottom(viewLeftBottom, viewRightBottom);

		double viewScale = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

		const iview::IColorSchema& colorSchema = GetColorSchema();

		double viewportLineWidth = colorSchema.GetLogicalLineWidth();

		if (			(viewScale * viewLeft.GetDistance(viewPos) < viewportLineWidth) ||
						(viewScale * viewRight.GetDistance(viewPos) < viewportLineWidth) ||
						(viewScale * viewTop.GetDistance(viewPos) < viewportLineWidth) ||
						(viewScale * viewBottom.GetDistance(viewPos) < viewportLineWidth)){
			return TS_TICKER;
		}
	}

	return TS_NONE;
}


// reimplemented (iview::IVisualizable)

void CPerspectiveCalibrationBoundsShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const CPerspectiveCalibration* calibPtr = dynamic_cast<const CPerspectiveCalibration*>(GetModelPtr());
	if (calibPtr != NULL){
		const i2d::CRectangle& bounds = calibPtr->GetBounds();
		i2d::CVector2d viewLeftTop, viewRightTop, viewLeftBottom, viewRightBottom;

		calibPtr->GetInvPositionAt(bounds.GetLeftTop(), viewLeftTop);
		calibPtr->GetInvPositionAt(bounds.GetRightTop(), viewRightTop);
		calibPtr->GetInvPositionAt(bounds.GetLeftBottom(), viewLeftBottom);
		calibPtr->GetInvPositionAt(bounds.GetRightBottom(), viewRightBottom);

		const iview::IColorSchema& colorSchema = GetColorSchema();

		drawContext.save();
		drawContext.setPen(colorSchema.GetPen(IsSelected()? iview::IColorSchema::SP_SELECTED: iview::IColorSchema::SP_NORMAL));

		QPoint screenLeftTop = iqt::GetQPoint(GetScreenPosition(viewLeftTop).ToIndex2d());
		QPoint screenLeftBottom = iqt::GetQPoint(GetScreenPosition(viewLeftBottom).ToIndex2d());
		QPoint screenRightTop = iqt::GetQPoint(GetScreenPosition(viewRightTop).ToIndex2d());
		QPoint screenRightBottom = iqt::GetQPoint(GetScreenPosition(viewRightBottom).ToIndex2d());

		drawContext.drawLine(screenLeftTop, screenRightTop);
		drawContext.drawLine(screenRightTop, screenRightBottom);
		drawContext.drawLine(screenRightBottom, screenLeftBottom);
		drawContext.drawLine(screenLeftBottom, screenLeftTop);

		drawContext.restore();
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CPerspectiveCalibrationBoundsShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	if (!IsEditablePosition()){
		return false;
	}

	if (downFlag){
		const CPerspectiveCalibration* calibPtr = dynamic_cast<const CPerspectiveCalibration*>(GetModelPtr());
		if (calibPtr != NULL){
			const i2d::CRectangle& bounds = calibPtr->GetBounds();
			i2d::CVector2d viewLeftTop, viewRightTop, viewLeftBottom, viewRightBottom;

			calibPtr->GetInvPositionAt(bounds.GetLeftTop(), viewLeftTop);
			calibPtr->GetInvPositionAt(bounds.GetRightTop(), viewRightTop);
			calibPtr->GetInvPositionAt(bounds.GetLeftBottom(), viewLeftBottom);
			calibPtr->GetInvPositionAt(bounds.GetRightBottom(), viewRightBottom);

			const iview::IColorSchema& colorSchema = GetColorSchema();

			i2d::CVector2d viewPos = GetLogPosition(i2d::CVector2d(position));

			i2d::CLine2d viewLeft(viewLeftTop, viewLeftBottom);
			i2d::CLine2d viewRight(viewRightTop, viewRightBottom);
			i2d::CLine2d viewTop(viewLeftTop, viewRightTop);
			i2d::CLine2d viewBottom(viewLeftBottom, viewRightBottom);

			double viewScale = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

			double viewportLineWidth = colorSchema.GetLogicalLineWidth();

			if (viewScale * viewLeft.GetDistance(viewPos) < viewportLineWidth){
				m_editBound = EB_LEFT;
				BeginModelChanges();
				return true;
			}
			if (viewScale * viewRight.GetDistance(viewPos) < viewportLineWidth){
				m_editBound = EB_RIGHT;
				BeginModelChanges();
				return true;
			}
			if (viewScale * viewTop.GetDistance(viewPos) < viewportLineWidth){
				m_editBound = EB_TOP;
				BeginModelChanges();
				return true;
			}
			if (viewScale * viewBottom.GetDistance(viewPos) < viewportLineWidth){
				m_editBound = EB_BOTTOM;
				BeginModelChanges();
				return true;
			}
		}
	}

	EndModelChanges();

	return false;
}


bool CPerspectiveCalibrationBoundsShape::OnMouseMove(istd::CIndex2d position)
{
	if (!IsEditablePosition()){
		return false;
	}

	imod::IModel* modelPtr = GetModelPtr();
	CPerspectiveCalibration* calibPtr = dynamic_cast<CPerspectiveCalibration*>(modelPtr);
	if (calibPtr != NULL){
		i2d::CRectangle bounds = calibPtr->GetBounds();

		i2d::CVector2d viewPos = GetLogPosition(i2d::CVector2d(position));

		i2d::CVector2d logPos;
		if (calibPtr->GetPositionAt(viewPos, logPos)){
			bool isChanged = false;

			switch (m_editBound){
			case EB_LEFT:
				if (logPos.GetX() < bounds.GetRight()){
					bounds.SetLeft(logPos.GetX());
					isChanged = true;
				}
				break;

			case EB_RIGHT:
				if (logPos.GetX() > bounds.GetLeft()){
					bounds.SetRight(logPos.GetX());
					isChanged = true;
				}
				break;

			case EB_TOP:
				if (logPos.GetY() < bounds.GetBottom()){
					bounds.SetTop(logPos.GetY());
					isChanged = true;
				}
				break;

			case EB_BOTTOM:
				if (logPos.GetY() > bounds.GetTop()){
					bounds.SetBottom(logPos.GetY());
					isChanged = true;
				}
				break;
			}
			if (isChanged){
				calibPtr->SetBounds(bounds);
				UpdateModelChanges();
			}
		}
		return true;
	}
	else{
		return false;
	}
}


// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CPerspectiveCalibrationBoundsShape::CalcBoundingBox() const
{
	iview::IDisplay* displayPtr = GetDisplayPtr();
	if (displayPtr != NULL){
		return displayPtr->GetClientRect();
	}

	return i2d::CRect::GetEmpty();
}


// reimplemented (iview::CInteractiveShapeBase)

void CPerspectiveCalibrationBoundsShape::BeginLogDrag(const i2d::CVector2d& /*reference*/)
{
}


void CPerspectiveCalibrationBoundsShape::SetLogDragPosition(const i2d::CVector2d& /*position*/)
{
}


} // namespace iview



