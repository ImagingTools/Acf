#include "iview/CInteractiveShapeBase.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "istd/IChangeable.h"
#include "istd/TChangeNotifier.h"
#include "iqt/iqt.h"

#include "iview/ISelectable.h"
#include "iview/IViewLayer.h"


namespace iview
{


CInteractiveShapeBase::CInteractiveShapeBase()
{
	m_isSelected = false;
	m_isEditablePosition = true;

	AssignToLayer(IViewLayer::LT_ACTIVE);
}


CInteractiveShapeBase::CInteractiveShapeBase(const CInteractiveShapeBase& /*shape*/)
{
}


CInteractiveShapeBase::~CInteractiveShapeBase()
{
	DisconnectDisplay();
}


// reimplemented (iview::IInteractiveShape)

bool CInteractiveShapeBase::IsSelected() const
{
	return m_isSelected;
}


void CInteractiveShapeBase::SetSelected(bool selectFlag)
{
	if (m_isSelected != selectFlag){
		m_isSelected = selectFlag;

		InvalidateBoundingBox();

		ISelectable* observerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
		if (observerPtr != NULL){
			observerPtr->OnShapeSelected(*this, selectFlag);
		}
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveShapeBase::OnMouseButton(istd::CIndex2d /*position*/, Qt::MouseButton /*buttonType*/, bool /*downFlag*/)
{
	return false;
}


bool CInteractiveShapeBase::OnMouseMove(istd::CIndex2d)
{
	return false;
}


// reimplemented (imod::IObserver)

bool CInteractiveShapeBase::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass3::OnAttached(modelPtr)){
		Invalidate(CS_CONSOLE);

		return true;
	}

	return false;
}


bool CInteractiveShapeBase::OnDetached(imod::IModel* modelPtr)
{
	Invalidate(CS_CONSOLE);

	return BaseClass3::OnDetached(modelPtr);
}


// protected methods

void CInteractiveShapeBase::BeginModelChanges()
{
	ISelectable* controllerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
	if (controllerPtr != NULL){
		controllerPtr->OnShapeFocused(this);
	}
}


void CInteractiveShapeBase::EndModelChanges()
{
	UpdateModelChanges();

	ISelectable* controllerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
	if (controllerPtr != NULL){
		controllerPtr->OnShapeDefocused(this);
	}
}


void CInteractiveShapeBase::UpdateModelChanges()
{
	istd::IChangeable* objectPtr = dynamic_cast<istd::IChangeable*>(GetModelPtr());

	istd::CChangeNotifier changePtr(objectPtr, istd::IChangeable::CF_MODEL);
}


void CInteractiveShapeBase::DrawText(QPainter& drawContext, istd::CIndex2d point, const QString& text) const
{
	QFontMetrics metrics(drawContext.font());

	QRect textRect(iqt::GetQPoint(point), metrics.size(0, text));

	drawContext.drawText(textRect, Qt::AlignLeft | Qt::AlignBottom, text);
}

void CInteractiveShapeBase::DrawOrientationMarker(
			QPainter& drawContext,
			const QPen& rightPen,
			const QBrush& rightBrush,
			const QPen& leftPen,
			const QBrush& leftBrush,
			const i2d::CLine2d& segmentLine,
			double scale) const
{
	double lineLength = segmentLine.GetLength();
	if (lineLength < I_BIG_EPSILON){
		return;
	}

	scale = sqrt(scale);

	// arbitrary marker dimensions in pixels, multiplied by scale
	double markerSizeX = 8 * scale;
	double markerSizeY = 30 * scale;

	double maxMarkerLength = qMin(20.0, lineLength * 0.2);

	// limit marker size to half line length
	if (markerSizeY > maxMarkerLength){
		markerSizeX *= maxMarkerLength / markerSizeY;
		markerSizeY = maxMarkerLength;
	}

	i2d::CVector2d lineCenter = segmentLine.GetCenter();

	i2d::CVector2d segmentDelta = segmentLine.GetDiffVector();
	i2d::CVector2d segmentBaseX = segmentDelta.GetNormalized(markerSizeX);
	i2d::CVector2d segmentBaseY = segmentDelta.GetOrthogonal().GetNormalized(markerSizeY);
	// two points common to both markers
	i2d::CVector2d markerPoint1 = lineCenter - segmentBaseX;
	i2d::CVector2d markerPoint2 = lineCenter + segmentBaseX;
	i2d::CVector2d markerPoint3 = lineCenter - segmentBaseY;
	i2d::CVector2d markerPoint4 = lineCenter + segmentBaseY;
 
	drawContext.save();

	// draw the markers
	QPolygonF markerPolygon(3);
	markerPolygon[0] = markerPoint1;
	markerPolygon[1] = markerPoint2;
	markerPolygon[2] = markerPoint3;
	drawContext.setPen(leftPen);
	drawContext.setBrush(leftBrush);
	drawContext.drawPolygon(markerPolygon);

	markerPolygon[2] = markerPoint4;

	drawContext.setPen(rightPen);
	drawContext.setBrush(rightBrush);
	drawContext.drawPolygon(markerPolygon);

	drawContext.restore();
}



} // namespace iview


