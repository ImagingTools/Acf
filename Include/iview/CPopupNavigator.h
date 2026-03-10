#pragma once

#include <QPen>
#include <QPixmap>
#include <QScrollBar>
#include <QScrollArea>
#include <QToolButton>


namespace iview
{


/**
    \brief 2-dimensional scrolling widget.
    \since 0.2

    \image html CPopupNavigator.png An example of CPopupNavigator

    CPopupNavigator is designed for scrolling in both direction (horizontal and vertical)
    at the same time. Main frame represents the scrollable area, and the slider inside the frame
    corresponds to the visible part.

    Slider is draggable within the frame (panning mode on). This behavior is enabled by default.
    If you want to disable the panning, use enablePanning(false).

    To use CPopupNavigator, attach it to the source widget (which content is to be scrolled) using one of connectSource() methods.
    Currently \a QAbstractScrollArea and \a QAbstractSlider descedants can be handled by CPopupNavigator.

    \code
    // a QGraphicsView is a descedant of the QAbstractScrollArea
    QGraphicsView *myView;

    ...

    // create CPopupNavigator and connect it to the QGraphicsView
    CPopupNavigator *myPanner = new CPopupNavigator(this);
    myPanner->connectSource(myView);
    \endcode
*/
class CPopupNavigator : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(double opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(double sliderOpacity READ sliderOpacity WRITE setSliderOpacity)
    Q_PROPERTY(bool panning READ isPanningEnabled WRITE enablePanning)
    Q_PROPERTY(QPen backgroundPen READ backgroundPen WRITE setBackgroundPen)
    Q_PROPERTY(QBrush backgroundBrush READ backgroundBrush WRITE setBackgroundBrush)
    Q_PROPERTY(QPen sliderPen READ sliderPen WRITE setSliderPen)
    Q_PROPERTY(QBrush sliderBrush READ sliderBrush WRITE setSliderBrush)

public:
    /** Constructor */
    explicit CPopupNavigator(QWidget *parent = 0);
    /** Destructor */
    virtual ~CPopupNavigator();

    void setBackgroundPen(const QPen& pen);
    inline QPen backgroundPen() const { return m_pen; }

    void setBackgroundBrush(const QBrush& brush);
    inline QBrush backgroundBrush() const { return m_brush; }

    void setSliderPen(const QPen& pen);
    inline QPen sliderPen() const { return m_panPen; }

    void setSliderBrush(const QBrush& brush);
    inline QBrush sliderBrush() const { return m_panBrush; }

    void setOpacity(double opacity);
    inline double opacity() const { return m_opacity; }

    void setSliderOpacity(double opacity);
    inline double sliderOpacity() const { return m_panOpacity; }

    /** Returns true if mouse panning is enabled (default), false otherwise.
      \sa enablePanning()
      */
    inline bool isPanningEnabled() const { return m_panning; }

    void connectSource(QAbstractScrollArea *area);
    void connectSource(QAbstractSlider *horizontal, QAbstractSlider *vertical);
    void disconnectSource();

    /** Connects to a tool button which will show this CPopupNavigator as a popup menu.
    */
    void connectButton(QToolButton*);

    /** Removes background pixmap (if any) and resets the size of the navigator to \a size.
     */
	void resetSize(const QSize& size);
	void resetSize(int width, int height) { resetSize(QSize(width, height)); }

    /** Sets the pixmap to be shown as the background of the navigator.
        If not set, a default rectangle is drawn.
      */
	void setFromPixmap(const QPixmap& pixmap);

    // reimp
    virtual QSize minimumSizeHint() const    {   return QSize(64,64);    }

Q_SIGNALS:
    void scrollHorizontal(int value);
    void scrollVertical(int value);

    void aboutToShow();
	void wheeled(QWheelEvent *we);

public Q_SLOTS:
    /** Sets panning mode to \a on (true by default).
        When panning is enabled, you can move the slider by the mouse.
      */
    void enablePanning(bool on = true);

protected:
    virtual void paintEvent(QPaintEvent *) override;
    virtual bool eventFilter(QObject *, QEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent *) override;
	virtual bool event(QEvent *) override;

    virtual void drawBackground(QPainter& p, QRect r);
    virtual void drawSlider(QPainter& p, QRect r);

private:
    void setHorizontalRange(int min, int max);
    void setHorizontalValue(int value);
    void setHorizontalViewSize(int size);

    void setVerticalRange(int min, int max);
    void setVerticalValue(int value);
    void setVerticalViewSize(int size);

    void setViewSize(const QSize& size);

    void calculateXsize(int size);
    void calculateYsize(int size);

    void connectH(QAbstractSlider *sl);
    void connectV(QAbstractSlider *sl);

    int m_minX, m_maxX, m_sizeX, m_valueX;
    int m_valueXpan, m_sizeXpan;

    int m_minY, m_maxY, m_sizeY, m_valueY;
    int m_valueYpan, m_sizeYpan;

    QSize m_viewSize;
	QPoint m_viewPos;

    QPen m_pen, m_panPen;
    QBrush m_brush, m_panBrush;
	QPixmap m_pixmap;

    double m_opacity, m_panOpacity;

    bool m_panning;
    int m_oldX, m_oldY;

    QAbstractSlider *m_sliderX, *m_sliderY;
};


} // namespace 

