#ifndef iqt_TGuiComponentBase_included
#define iqt_TGuiComponentBase_included


#include <QWidget>

#include "iqt/CGuiComponentBase.h"


namespace iqt
{


/**	Base class for all Qt GUI componentes.
 */
template <class WidgetType>
class TGuiComponentBase: public CGuiComponentBase
{
public:
	typedef CGuiComponentBase BaseClass;
	I_COMPONENT(TGuiComponentBase);

	TGuiComponentBase(const icomp::IComponentContext* contextPtr);

	// reimplemented (iqt::CGuiComponentBase)
	virtual QWidget* InitWidgetToParent(QWidget* parentPtr);
};


// public methods

template <class WidgetType>
TGuiComponentBase<WidgetType>::TGuiComponentBase(const icomp::IComponentContext* contextPtr)
:	BaseClass(contextPtr)
{
	I_REGISTER_INTERFACE(IGuiObject);
}


// reimplemented (iqt::CGuiComponentBase)

template <class WidgetType>
QWidget* TGuiComponentBase<WidgetType>::InitWidgetToParent(QWidget* parentPtr)
{
	I_ASSERT(!IsGuiCreated());

	WidgetType* widgetPtr = new WidgetType(parentPtr);

	return widgetPtr;
}


} // namespace iqt


#endif //!iqt_TGuiComponentBase_included


