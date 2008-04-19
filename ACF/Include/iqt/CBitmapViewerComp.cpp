#include "iqt/CBitmapViewerComp.h"


#include <QBrush>

#include "iimg/IBitmap.h"

#include "iqt/CBitmap.h"


namespace iqt
{


// reimplemented (iqt::TGuiObserverWrap)

void CBitmapViewerComp::UpdateModel() const
{
}


void CBitmapViewerComp::UpdateEditor()
{
	iqt::CBitmap* bitmapPtr = dynamic_cast<iqt::CBitmap*>(GetObjectPtr());

	if (bitmapPtr != NULL){
		m_scene.setBackgroundBrush(QBrush(*bitmapPtr));
	}
}


// protected methods

// reimplemented (iqt::CGuiComponentBase)

void CBitmapViewerComp::OnGuiCreated()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);

	viewPtr->setScene(&m_scene);
}


} // namespace iqt


