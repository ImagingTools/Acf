#ifndef iqtgui_TControlledWidgetWrap_included
#define iqtgui_TControlledWidgetWrap_included


// Qt includes
#include <QWidget>
#include <QCloseEvent>


// ACF includes
#include "iqtgui/CGuiComponentBase.h"


namespace iqtgui
{


/**
	Wrapper for controlling the "close"-behaviour of the widget.
	\sa TryClose()
 */
template <class WidgetType>
class TControlledWidgetWrap: public WidgetType
{
public:
	typedef WidgetType BaseClass;

	TControlledWidgetWrap(CGuiComponentBase& ownerComponent, QWidget* parent = NULL);

protected:
	// reimplemented (QWidget)
	virtual void closeEvent(QCloseEvent* eventPtr);

private:
	CGuiComponentBase& m_ownerComponent;
};


// public methods

template <class WidgetType>
TControlledWidgetWrap<WidgetType>::TControlledWidgetWrap(CGuiComponentBase& ownerComponent, QWidget* parent)
:	BaseClass(parent),
	m_ownerComponent(ownerComponent)
{
}


// protected methods

// reimplemented (QWidget)

template <class WidgetType>
void TControlledWidgetWrap<WidgetType>::closeEvent(QCloseEvent* eventPtr)
{
	bool ignoreClose;
	m_ownerComponent.OnTryClose(&ignoreClose);
	if (ignoreClose){
		eventPtr->ignore();
	}
	else{
		eventPtr->accept();
	}
}


} // namespace iqtgui


#endif // !iqtgui_TControlledWidgetWrap_included


