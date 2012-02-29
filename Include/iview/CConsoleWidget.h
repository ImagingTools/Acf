#ifndef iview_CConsoleWidget_included
#define iview_CConsoleWidget_included


// Qt includes
#include <QtGui/QPainter>
#include <QtGui/QWidget>
#include <QtGui/QCursor>
#include <QtGui/QPainter>


// ACF includes
#include "iview/TViewImpl.h"
#include "iview/CDrawBuffer.h"
#include "iview/CColorShema.h"
#include "iview/CCalibratedViewBase.h"


namespace iview
{


class CConsoleWidget:
			public QWidget,
			public iview::TViewImpl<iview::CCalibratedViewBase>
{
public:
	typedef iview::TViewImpl<iview::CCalibratedViewBase> BaseClass;
	typedef QWidget BaseClass2;

	CConsoleWidget(QWidget* parent = NULL);

	// reimplemented (iview::IShapeView)
	virtual i2d::CRect GetClientRect() const;

protected:
	// reimplemented (QWidget)
	virtual void paintEvent(QPaintEvent* event);
	virtual void resizeEvent (QResizeEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);

	// reimplemented (iview::CViewBase)
	virtual void SetMousePointer(MousePointerMode mode);
	void UpdateRectArea(const i2d::CRect& rect);

	// static methods
	int GetKeysState(const QMouseEvent& event);

private:
	QCursor m_mousePointerModes[MPM_LAST + 1];
};


} // namespace iview


#endif // !iview_CConsoleWidget_included


