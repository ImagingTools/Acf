// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtgui/CCalendarGuiComp.h>


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CCalendarGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CalendarWidget->setCurrentPage(QDate::currentDate().year(), QDate::currentDate().month());
	CalendarWidget->setSelectedDate(QDate::currentDate());
}


void CCalendarGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();
}


} // namespace iqtgui


