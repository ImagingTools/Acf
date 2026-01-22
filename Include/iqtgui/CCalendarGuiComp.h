// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iqtgui/TDesignerGuiCompBase.h>

#include <GeneratedFiles/iqtgui/ui_CCalendarGuiComp.h>


namespace iqtgui
{


/**
	Component for displaying a calendar.
*/
class CCalendarGuiComp: public iqtgui::TDesignerGuiCompBase<Ui::CCalendarGuiComp>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CCalendarGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CCalendarGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiRetranslate() override;
};


} // namespace iqtgui




