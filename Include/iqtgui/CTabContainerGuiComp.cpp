#include "iqtgui/CTabContainerGuiComp.h"


// Qt includes
#include <QtGui/QTabWidget>


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::CMultiPageGuiCompBase)

int CTabContainerGuiComp::GetPagesCount() const
{
	return m_slaveWidgetsCompPtr.GetCount();
}


iqtgui::IGuiObject* CTabContainerGuiComp::GetPageGuiComponent(int pageIndex) const
{
	Q_ASSERT(pageIndex >= 0);
	Q_ASSERT(pageIndex < m_slaveWidgetsCompPtr.GetCount());

	return m_slaveWidgetsCompPtr[pageIndex];
}


int CTabContainerGuiComp::GetDesignType() const
{
	return CMultiPageWidget::DT_TAB_WIDGET;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CTabContainerGuiComp::OnGuiCreated()
{
	Q_ASSERT(IsGuiCreated());

	BaseClass::OnGuiCreated();

	CMultiPageWidget* widgetPtr = dynamic_cast<CMultiPageWidget*>(GetWidget());
	Q_ASSERT(widgetPtr != NULL);

	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(widgetPtr->GetContainerWidgetPtr());
	if (tabWidgetPtr != NULL){
		if (*m_useTriangularTabsAttrPtr){
			tabWidgetPtr->setTabShape(QTabWidget::Triangular);
		}

		tabWidgetPtr->setTabPosition(QTabWidget::TabPosition(*m_tabOrientationAttrPtr));

		// setup the corner widget:
		if (m_cornerGuiCompPtr.IsValid()){
			if (m_cornerGuiCompPtr->CreateGui(NULL)){
				tabWidgetPtr->setCornerWidget(m_cornerGuiCompPtr->GetWidget());
			}
		}
	}
}


} // namespace iqtgui


