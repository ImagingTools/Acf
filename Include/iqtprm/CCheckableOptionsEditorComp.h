// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <iprm/IOptionsManager.h>

#include <GeneratedFiles/iqtprm/ui_CCheckableOptionsEditorComp.h>


namespace iqtprm
{


class CCheckableOptionsEditorComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CCheckableOptionsEditorComp,
						iprm::IOptionsManager>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CCheckableOptionsEditorComp,
				iprm::IOptionsManager> BaseClass;

	I_BEGIN_COMPONENT(CCheckableOptionsEditorComp);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Horizontal mode for GUI", true, true);
	I_END_COMPONENT;

protected Q_SLOTS:
	void OnParameterChanged();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

private:
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
};


} // namespace iqtprm




