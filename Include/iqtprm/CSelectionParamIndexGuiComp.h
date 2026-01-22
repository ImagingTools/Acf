// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <iprm/ISelectionParam.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/iqtprm/ui_CSelectionParamIndexGuiComp.h>


namespace iqtprm
{


class CSelectionParamIndexGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSelectionParamIndexGuiComp,
			iprm::ISelectionParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSelectionParamIndexGuiComp,
				iprm::ISelectionParam> BaseClass;

	I_BEGIN_COMPONENT(CSelectionParamIndexGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

protected Q_SLOTS:
	void on_OptionIndexSlider_valueChanged(int value);
};


} // namespace iqtprm




