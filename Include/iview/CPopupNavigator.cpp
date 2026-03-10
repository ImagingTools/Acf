#include "CPopupNavigator.h"

#include <QMouseEvent>
#include <QPainter>
#include <QWidgetAction>
#include <QMenu>
#include <QGuiApplication>
#include <QScreen>


namespace iview
{


// constructor

CPopupNavigator::CPopupNavigator(QWidget *parent) :
    QWidget(parent),
    m_panning(true),
    m_sliderX(0),
    m_sliderY(0)
{
    m_valueXpan = m_valueYpan = 0;
    m_sizeXpan = m_sizeYpan = 1;

    m_minX = m_maxX = m_valueX = 0;
    m_minY = m_maxY = m_valueY = 0;

    m_sizeX = m_sizeY = 1;
    m_viewSize = QSize(1,1);

    m_opacity = 1;
    m_panOpacity = 0.5;

    m_oldX = m_oldY = 0;
}

CPopupNavigator::~CPopupNavigator()
{
}


// properties

void CPopupNavigator::setBackgroundPen(const QPen& pen)
{
    m_pen = pen;
    update();
}

void CPopupNavigator::setBackgroundBrush(const QBrush& brush)
{
    m_brush = brush;
    update();
}

void CPopupNavigator::setSliderPen(const QPen& pen)
{
    m_panPen = pen;
    update();
}

void CPopupNavigator::setSliderBrush(const QBrush& brush)
{
    m_panBrush = brush;
    update();
}

void CPopupNavigator::setOpacity(double opacity)
{
    m_opacity = opacity;
    update();
}

void CPopupNavigator::setSliderOpacity(double opacity)
{
    m_panOpacity = opacity;
    update();
}

// source connection

void CPopupNavigator::connectSource(QAbstractSlider *horizontal, QAbstractSlider *vertical)
{
    disconnectSource();

    connectH(horizontal);
    connectV(vertical);
}

void CPopupNavigator::connectSource(QAbstractScrollArea *area)
{
    connectSource(area->horizontalScrollBar(), area->verticalScrollBar());
}

void CPopupNavigator::connectH(QAbstractSlider *sl)
{
    m_sliderX = sl;

    if (m_sliderX)
    {
        connect(m_sliderX, SIGNAL(rangeChanged(int,int)),
                this, SLOT(setHorizontalRange(int,int)));

        connect(m_sliderX, SIGNAL(valueChanged(int)),
                this, SLOT(setHorizontalValue(int)));

        connect(this, SIGNAL(scrollHorizontal(int)),
                m_sliderX, SLOT(setValue(int)));

        m_sliderX->installEventFilter(this);
    }
}

void CPopupNavigator::connectV(QAbstractSlider *sl)
{
    m_sliderY = sl;

    if (m_sliderY)
    {
        connect(m_sliderY, SIGNAL(rangeChanged(int,int)),
                this, SLOT(setVerticalRange(int,int)));

        connect(m_sliderY, SIGNAL(valueChanged(int)),
                this, SLOT(setVerticalValue(int)));

        connect(this, SIGNAL(scrollVertical(int)),
                m_sliderY, SLOT(setValue(int)));

        m_sliderY->installEventFilter(this);
    }
}

void CPopupNavigator::disconnectSource()
{
    if (m_sliderX)
    {
        disconnect(m_sliderX, SIGNAL(rangeChanged(int,int)),
                this, SLOT(setHorizontalRange(int,int)));

        disconnect(m_sliderX, SIGNAL(valueChanged(int)),
                this, SLOT(setHorizontalValue(int)));

        disconnect(this, SIGNAL(scrollHorizontal(int)),
                m_sliderX, SLOT(setValue(int)));

        m_sliderX->removeEventFilter(this);

        m_sliderX = 0;
    }

    if (m_sliderY)
    {
        disconnect(m_sliderY, SIGNAL(rangeChanged(int,int)),
                this, SLOT(setVerticalRange(int,int)));

        disconnect(m_sliderY, SIGNAL(valueChanged(int)),
                this, SLOT(setVerticalValue(int)));

        disconnect(this, SIGNAL(scrollVertical(int)),
                m_sliderY, SLOT(setValue(int)));

        m_sliderY->removeEventFilter(this);

        m_sliderY = 0;
    }
}

void CPopupNavigator::setHorizontalRange(int min, int max)
{
    m_minX = min;
    m_maxX = max;
    m_sizeX = max - min + m_viewSize.width();

    update();
}

void CPopupNavigator::setHorizontalValue(int value)
{
    double d = double(width()) / double(m_sizeX);
    int v = value - m_minX;
    double dv = v * d;
    m_valueXpan = dv;

    update();
}

void CPopupNavigator::setHorizontalViewSize(int size)
{
    m_viewSize.setWidth(size);

    calculateXsize(size);

    update();
}

void CPopupNavigator::setVerticalRange(int min, int max)
{
    m_minY = min;
    m_maxY = max;
    m_sizeY = max - min + m_viewSize.height();

    update();
}

void CPopupNavigator::setVerticalValue(int value)
{
    double d = double(height()) / double(m_sizeY);
    int v = value - m_minY;
    double dv = v * d;
    m_valueYpan = dv;

    update();
}

void CPopupNavigator::setVerticalViewSize(int size)
{
    m_viewSize.setHeight(size);

    calculateYsize(size);

    update();
}

void CPopupNavigator::setViewSize(const QSize &size)
{
    m_viewSize = size;

    calculateXsize(m_viewSize.width());
    calculateYsize(m_viewSize.height());

    update();
}


// panning methods

void CPopupNavigator::enablePanning(bool on)
{
    m_panning = on;
}

void CPopupNavigator::mousePressEvent(QMouseEvent *event)
{
    if (m_panning && event->buttons() & Qt::LeftButton)
    {
        m_oldX = m_valueXpan - event->pos().x();
        m_oldY = m_valueYpan - event->pos().y();

        // if clicked outside of the rect
        if (!QRect(m_valueXpan, m_valueYpan, m_sizeXpan, m_sizeYpan).contains(event->pos()))
        {
            m_oldX = -m_sizeXpan/2;
            m_oldY = -m_sizeYpan/2;
            m_valueXpan = event->pos().x() + m_oldX;
            m_valueYpan = event->pos().y() + m_oldY;

            mouseMoveEvent(event);
        }
    }
}

void CPopupNavigator::mouseMoveEvent(QMouseEvent *event)
{
    if (m_panning && event->buttons() & Qt::LeftButton)
    {
        m_valueXpan = event->pos().x() + m_oldX;
        if (m_valueXpan < 0)
            m_valueXpan = 0;
        else
        if (m_valueXpan >= (width()-m_sizeXpan))
            m_valueXpan = width()-m_sizeXpan;

        double dx = double(width()) / double(m_sizeX);
        int viewX = double(m_valueXpan) / dx + m_minX;

        Q_EMIT scrollHorizontal(viewX);

        m_valueYpan = event->pos().y() + m_oldY;
        if (m_valueYpan < 0)
            m_valueYpan = 0;
        else
        if (m_valueYpan >= (height()-m_sizeYpan))
            m_valueYpan = height()-m_sizeYpan;

        double dy = double(height()) / double(m_sizeY);
        int viewY = double(m_valueYpan) / dy + m_minY;

        Q_EMIT scrollVertical(viewY);
    }
}


// painting methods

void CPopupNavigator::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    drawBackground(p, rect().adjusted(0,0,-1,-1));

    drawSlider(p, QRect(m_valueXpan, m_valueYpan,
                        m_sizeXpan, m_sizeYpan).adjusted(0,0,-1,-1));
}

void CPopupNavigator::drawBackground(QPainter& p, QRect r)
{
    if (!m_pixmap.isNull())
    {
        p.drawPixmap(r, m_pixmap);
        p.setPen(m_pen);
		p.setBrush(Qt::NoBrush);
        p.drawRect(r);
    }
    else
    {
        p.setPen(m_pen);
        p.setBrush(m_brush);
        p.setOpacity(m_opacity);
        p.drawRect(r);
    }
}

void CPopupNavigator::drawSlider(QPainter& p, QRect r)
{
    p.setPen(m_panPen);
    p.setBrush(m_panBrush);
    p.setOpacity(m_panOpacity);

    p.drawRect(r);
}


// helper methods

void CPopupNavigator::connectButton(QToolButton* sliderButton)
{
    sliderButton->setPopupMode(QToolButton::InstantPopup);
    QWidgetAction* sliderAction = new QWidgetAction(sliderButton);
    sliderAction->setDefaultWidget(this);

    QMenu* sliderMenu = new QMenu(parentWidget());
    sliderButton->setMenu(sliderMenu);
    sliderMenu->addAction(sliderAction);
    sliderMenu->setDefaultAction(sliderAction);
    sliderMenu->installEventFilter(this);

    connect(sliderMenu, &QMenu::aboutToShow, this, [=]() 
        {
			// this will change the size of the parent widget to fit the navigator
            Q_EMIT aboutToShow();

            parentWidget()->setFixedSize(m_viewSize);

			// reposition the menu under the button
            QPoint desired = sliderButton->mapToGlobal(QPoint(sliderButton->width(), sliderButton->height())) - QPoint(m_viewSize.width(), 0);

            // Clamp to the relevant screen
            QScreen* screen = QGuiApplication::screenAt(desired);
            if (!screen) screen = QGuiApplication::primaryScreen();
            const QRect avail = screen->availableGeometry();

            int x = std::clamp(desired.x(), avail.left(), avail.right() - m_viewSize.width());
            int y = std::clamp(desired.y(), avail.top(), avail.bottom() - m_viewSize.height());

			// actual position will be set in the menu show event
            m_viewPos = QPoint(x, y);
        }
    );
}

void CPopupNavigator::resetSize(const QSize& size)
{
	m_pixmap = QPixmap();
	setFixedSize(size); 
}

void CPopupNavigator::setFromPixmap(const QPixmap& pixmap)
{
    if (pixmap.isNull())
    {
        resetSize(minimumSizeHint());
        return;
    }

	m_pixmap = pixmap;
    auto psize = pixmap.size();
	setFixedSize(psize);
	setViewSize(psize);
    update();
}

void CPopupNavigator::calculateXsize(int size)
{
    m_sizeX = m_maxX - m_minX + m_viewSize.width();
    double d = double(width()) / double(m_sizeX);
    m_sizeXpan = qMax(int(size * d), 1);
}

void CPopupNavigator::calculateYsize(int size)
{
    m_sizeY = m_maxY - m_minY + m_viewSize.height();
    double d = double(height()) / double(m_sizeY);
    m_sizeYpan = qMax(int(size * d), 1);
}


// event filter

bool CPopupNavigator::eventFilter(QObject *obj, QEvent *event)
{
    bool b = QObject::eventFilter(obj, event);

    QAbstractSlider *sl = dynamic_cast<QAbstractSlider*>(obj);
    if (sl && event->type() == QEvent::Resize)
    {
        if (sl == m_sliderX)
            setHorizontalViewSize(sl->pageStep());
		else
        if (sl == m_sliderY)
            setVerticalViewSize(sl->pageStep());

		return b;
	}

	QMenu *tb = dynamic_cast<QMenu*>(obj);
	if (tb && event->type() == QEvent::MouseMove)
	{
		this->mouseMoveEvent((QMouseEvent*)event);
		return b;
	}

    if (tb && event->type() == QEvent::Show)
    {
		// refine the menu position
		tb->move(m_viewPos);
    }

    return b;
}

bool CPopupNavigator::event(QEvent *e)
{
    if (e->type() == QEvent::Paint)
    {
        if (m_sliderX)
        {
            setHorizontalRange(m_sliderX->minimum(), m_sliderX->maximum());
            setHorizontalViewSize(m_sliderX->pageStep());
            setHorizontalValue(m_sliderX->value());
        }

        if (m_sliderY)
        {
            setVerticalRange(m_sliderY->minimum(), m_sliderY->maximum());
            setVerticalViewSize(m_sliderY->pageStep());
            setVerticalValue(m_sliderY->value());
        }
    }

    if (e->type() == QEvent::Wheel)
    {
		Q_EMIT wheeled(static_cast<QWheelEvent*>(e));
    }

    return QWidget::event(e);
}


} // namespace QSint
