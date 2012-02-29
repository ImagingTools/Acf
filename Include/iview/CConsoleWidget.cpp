#include "iview/CConsoleWidget.h"


// Qt includes
#include <QtGui/QPaintEvent>
#include <QtGui/QMouseEvent>


namespace iview
{


// public methods

CConsoleWidget::CConsoleWidget(QWidget* parent)
	:BaseClass2(parent)
{
	m_mousePointerModes[MPM_NONE] = QCursor(Qt::ArrowCursor);
	m_mousePointerModes[MPM_DEFAULT] = QCursor(Qt::ArrowCursor);
	m_mousePointerModes[MPM_DESELECT] = QCursor(Qt::UpArrowCursor);
	m_mousePointerModes[MPM_POINTER] = QCursor(Qt::ArrowCursor);
	m_mousePointerModes[MPM_WAIT] = QCursor(Qt::WaitCursor);
	m_mousePointerModes[MPM_CROSS] = QCursor(Qt::CrossCursor);
	m_mousePointerModes[MPM_HAND] = QCursor(Qt::PointingHandCursor);
	m_mousePointerModes[MPM_DRAG] = QCursor(Qt::SizeAllCursor);
	m_mousePointerModes[MPM_SCREEN_MOVE] = QCursor(Qt::PointingHandCursor);

	setAttribute(Qt::WA_NoBackground);
	setAttribute(Qt::WA_PaintOnScreen);

	setMouseTracking(true);
}


// reimplemented (iview::IShapeView)

i2d::CRect CConsoleWidget::GetClientRect() const
{
	return i2d::CRect(iqt::GetCIndex2d(size()));
}


// protected methods

// reimplemented Qt (QWidget)

void CConsoleWidget::paintEvent(QPaintEvent* eventPtr)
{
	I_ASSERT(eventPtr != NULL);

	DrawBuffers(*this, iqt::GetCRect(eventPtr->rect()));
}


void CConsoleWidget::resizeEvent(QResizeEvent* /*eventPtr*/)
{
	CheckResize();
}


void CConsoleWidget::mousePressEvent(QMouseEvent* eventPtr)
{
	I_ASSERT(eventPtr != NULL);

	SetKeysState(GetKeysState(*eventPtr));

	istd::CIndex2d pos = iqt::GetCIndex2d(eventPtr->pos());
	Qt::MouseButton buttonState = eventPtr->button();

	bool isConsumed = OnMouseButton(pos, buttonState, true);
	if (isConsumed){
		Update();
	}
	else{
		BaseClass2::mousePressEvent(eventPtr);
	}
}


void CConsoleWidget::mouseReleaseEvent(QMouseEvent* eventPtr)
{
	I_ASSERT(eventPtr != NULL);

	SetKeysState(GetKeysState(*eventPtr));

	istd::CIndex2d pos = iqt::GetCIndex2d(eventPtr->pos());

	Qt::MouseButton buttonState = eventPtr->button();

	bool isConsumed = OnMouseButton(pos, buttonState, false);
	if (isConsumed){
		Update();
	}
	else{
		BaseClass2::mousePressEvent(eventPtr);
	}
}


void CConsoleWidget::mouseMoveEvent(QMouseEvent* eventPtr)
{
	I_ASSERT(eventPtr != NULL);

	SetKeysState(GetKeysState(*eventPtr));

	istd::CIndex2d pos = iqt::GetCIndex2d(eventPtr->pos());

	OnMouseMove(pos);

	BaseClass2::mouseMoveEvent(eventPtr);
}


// reimplemented (iview::CViewBase)

void CConsoleWidget::SetMousePointer(MousePointerMode mode)
{
	I_ASSERT(mode >= 0);
	I_ASSERT(mode < MPM_LAST);

	BaseClass2::setCursor(m_mousePointerModes[mode]);
}


void CConsoleWidget::UpdateRectArea(const i2d::CRect& rect)
{
	QRect qrect(iqt::GetQRect(rect));
	emit update(qrect);
}


// static protected methods

int CConsoleWidget::GetKeysState(const QMouseEvent& mouseEvent)
{
	Qt::MouseButtons buttons = mouseEvent.buttons();
	Qt::KeyboardModifiers modifiers = mouseEvent.modifiers();

	int retVal = 0;

	if ((buttons & Qt::LeftButton) != 0){
		retVal |= Qt::LeftButton;
	}
	if ((buttons & Qt::MidButton) != 0){
		retVal |= Qt::MidButton;
	}
	if ((buttons & Qt::RightButton) != 0){
		retVal |= Qt::RightButton;
	}
	if ((modifiers & Qt::ShiftModifier) != 0){
		retVal |= Qt::ShiftModifier;
	}
	if ((modifiers & Qt::ControlModifier) != 0){
		retVal |= Qt::ControlModifier;
	}

	return retVal;
}


} // namespace iview


