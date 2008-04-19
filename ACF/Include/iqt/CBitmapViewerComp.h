#ifndef iqt_CBitmapViewerComp_included
#define iqt_CBitmapViewerComp_included


#include <QGraphicsView>
#include <QGraphicsScene>

#include "imod/TSingleModelObserverBase.h"

#include "iimg/IBitmap.h"

#include "iqt/TGuiComponentBase.h"
#include "iqt/TGuiObserverWrap.h"


namespace iqt
{


class CBitmapViewerComp:
			public iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QGraphicsView>, imod::TSingleModelObserverBase<iimg::IBitmap> >
{
public:
	typedef iqt::TGuiObserverWrap<
				iqt::TGuiComponentBase<QGraphicsView>,
				imod::TSingleModelObserverBase<iimg::IBitmap> > BaseClass;

	I_BEGIN_COMPONENT(CBitmapViewerComp)
		I_REGISTER_INTERFACE(imod::IObserver)
	I_END_COMPONENT

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	QGraphicsScene m_scene;
};


} // namespace iqt


#endif // !iqt_CBitmapViewerComp_included


