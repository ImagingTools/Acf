#ifndef iqtgui_TDesignerGuiObserverCompBase_included
#define iqtgui_TDesignerGuiObserverCompBase_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include "imod/TSingleModelObserverBase.h"
#include "ibase/TModelObserverCompWrap.h"
#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/TDesignerGuiCompBase.h"


namespace iqtgui
{


/**
	Base class for all Qt GUI components.

	\ingroup ModelObserver
 */
template <class UI, class Model, class WidgetType = QWidget>
class TDesignerGuiObserverCompBase:
			public ibase::TModelObserverCompWrap<
						iqtgui::TGuiObserverWrap< iqtgui::TDesignerGuiCompBase<UI, WidgetType>, imod::TSingleModelObserverBase<Model> > >
{
public:
	typedef ibase::TModelObserverCompWrap<
				iqtgui::TGuiObserverWrap< iqtgui::TDesignerGuiCompBase<UI, WidgetType>, imod::TSingleModelObserverBase<Model> > > BaseClass;

	I_BEGIN_BASE_COMPONENT(TDesignerGuiObserverCompBase);
		I_REGISTER_INTERFACE(imod::IModelEditor);
	I_END_COMPONENT;
};


} // namespace iqtgui


#endif // !iqtgui_TDesignerGuiObserverCompBase_included


