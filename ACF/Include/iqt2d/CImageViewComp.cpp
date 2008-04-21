#include "iqt2d/CImageViewComp.h"


#include <QApplication>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMenu>


#include "iqt/CBitmap.h"


namespace iqt2d
{


CImageViewComp::CImageViewComp()
	:m_isFullScreenMode(true),
	m_isZoomIgnored(false)
{
	m_scenePtr = new QGraphicsScene;
}


CImageViewComp::~CImageViewComp() 
{
}


void CImageViewComp::CreateContextMenu()
{
	m_contextMenu = new QMenu(GetWidget());
	m_contextMenu->setTitle(tr("View Properties"));

	QAction* actiOnFitInView = new QAction(tr("&Fit Image In View"), m_contextMenu);
	connect(actiOnFitInView, SIGNAL( activated()), this, SLOT(OnFitInView()));
	m_contextMenu->addAction(actiOnFitInView);

	QAction* actiOnFitToImage = new QAction(tr("&Fit View To Image"), m_contextMenu);
	connect(actiOnFitToImage, SIGNAL(activated()), this, SLOT(OnFitToImage()));
	m_contextMenu->addAction(actiOnFitToImage);
}


// reimplemented (QObject)

bool CImageViewComp::eventFilter(QObject* obj, QEvent* event)
{
	if (!IsGuiCreated()){
		return false;
	}

	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return false;
	}

	if (obj != viewPtr){
		return false;
	}

	switch(event->type()){
		case QEvent::MouseButtonDblClick:
		case QEvent::GraphicsSceneMouseDoubleClick:
			OnMouseDoubleClickEvent(dynamic_cast<QMouseEvent*>(event));
			break;
	}

	return false;
}


// reimplemented (iqt::TGuiObserverWrap)

void CImageViewComp::UpdateModel() const
{
}


void CImageViewComp::UpdateEditor()
{
	if (IsGuiCreated()){
		iqt::CBitmap* bitmapPtr = dynamic_cast<iqt::CBitmap*>(GetObjectPtr());

		if (bitmapPtr != NULL){
			m_scenePtr->setSceneRect(0, 0, bitmapPtr->width(), bitmapPtr->height());

			m_imageItem.SetImage(*bitmapPtr);
		}
	}
}


// protected methods

// reimplemented (iqt::CGuiComponentBase)

void CImageViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}
	
	viewPtr->setScene(m_scenePtr);
	viewPtr->setMouseTracking(true);
	viewPtr->setDragMode(QGraphicsView::ScrollHandDrag);

	m_scenePtr->addItem(&m_imageItem);

	viewPtr->installEventFilter(this);

	CreateContextMenu();
	CreateBackgroundPixmap();

	viewPtr->setBackgroundBrush(QBrush(m_backgroundPixmap));
}


void CImageViewComp::OnGuiDestroyed()
{
	m_scenePtr->removeItem(&m_imageItem);

	BaseClass::OnGuiDestroyed();
}



void CImageViewComp::SetFitMode(FitMode mode)
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	m_fitMode = mode;
	if (m_fitMode == ScaleToFit){
		viewPtr->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		viewPtr->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		OnFitInView();
	}
	else{
		viewPtr->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		viewPtr->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
}


void CImageViewComp::SetFullScreenMode(bool fullScreenMode)
{
	m_isFullScreenMode = fullScreenMode;
}


void CImageViewComp::ScaleView(double scaleFactor)
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	QMatrix sceneMatrix = viewPtr->matrix();
	QMatrix scaleMatrix;
	scaleMatrix.scale(scaleFactor,scaleFactor);

	sceneMatrix *= scaleMatrix;
	
	SetZoom(sceneMatrix.m11());
}


void CImageViewComp::OnZoomIncrement()
{
	 ScaleView(pow((double)2, 0.5));  
}


void CImageViewComp::OnZoomDecrement()
{
	ScaleView(pow((double)2, -0.5));  
}


void CImageViewComp::SetZoom(double scaleFactor)
{
	if (m_isZoomIgnored){
		return;
	}

	if (m_fitMode == ScaleToFit){
		return;
	}

	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	QMatrix scaleMatrix;
	scaleMatrix.scale(scaleFactor, scaleFactor);

	viewPtr->setMatrix(scaleMatrix);

	m_isZoomIgnored = true;

	emit zoomChanged(scaleFactor);

	m_isZoomIgnored = false;

}


void CImageViewComp::SetZoom(const QString& zoomString)
{
	SetZoom(zoomString.toInt() / 100.0);
}


void CImageViewComp::SwitchFullScreen()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	if (viewPtr->isFullScreen()){
		viewPtr->showNormal();
	}

	else{
		viewPtr->showFullScreen();
	}
}


void CImageViewComp::OnFitInView()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	double scaleX = viewPtr->width()  / (double)(m_imageItem.GetWidth());
	double scaleY = viewPtr->height() / (double)(m_imageItem.GetHeight());
	
	double newScale = (scaleX >=scaleY) ? scaleY : scaleX;

	QMatrix scaleMatrix;
	scaleMatrix.scale(newScale, newScale);	
		
	viewPtr->setMatrix(scaleMatrix);
}


void CImageViewComp::OnFitToImage()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	viewPtr->showNormal();
	double r = viewPtr->matrix().m11();
	viewPtr->resize(m_imageItem.GetWidth() * r, m_imageItem.GetHeight() * r);
}


void CImageViewComp::OnResize(QResizeEvent* event)
{
	if (m_fitMode == ScaleToFit){
		OnFitInView();
	}
}


void CImageViewComp::OnWheelEvent(QWheelEvent* event)
{
	ScaleView(pow((double)2, -event->delta() / 240.0));
}


void CImageViewComp::OnMouseDoubleClickEvent(QMouseEvent* event)
{
	if (m_isFullScreenMode){
		SwitchFullScreen();
	}
}


void CImageViewComp::OnMouseMoveEvent(QMouseEvent* event)
{
}


void CImageViewComp::OnKeyReleaseEvent(QKeyEvent* event)
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	switch(event->key()){
		case Qt::Key_Plus:
			OnZoomIncrement();
			break;
		case Qt::Key_Minus:
			OnZoomDecrement();
			break;
		case Qt::Key_Escape:
			if (viewPtr->isFullScreen()){
				viewPtr->showNormal();
			}
			break;
	}
}


void CImageViewComp::OnContextMenuEvent(QContextMenuEvent* event)
{
	m_contextMenu->exec(QCursor::pos());
}


// private methods

void CImageViewComp::InvalidateScene()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	m_scenePtr->setSceneRect(0,0, m_imageItem.GetWidth(), m_imageItem.GetHeight());
	
	viewPtr->centerOn(&m_imageItem);
}


void CImageViewComp::CreateBackgroundPixmap()
{
	m_backgroundPixmap = QPixmap(16,16);
		
	QPainter p(&m_backgroundPixmap);
	p.fillRect(0, 0, 8, 8, QBrush(qRgb(200,200,200)));
	p.fillRect(0, 8, 8, 8, QBrush(Qt::white));
	p.fillRect(8, 0, 8, 8, QBrush(Qt::white));
	p.fillRect(8, 8, 8, 8, QBrush(qRgb(200,200,200)));
}


} // namespace iqt2d