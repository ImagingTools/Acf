// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <iview/IShapeStatusInfo.h>
#include <GeneratedFiles/iview/ui_CShapeInfoGuiComp.h>


namespace iview
{


class CShapeInfoGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CShapeInfoGuiComp, IShapeStatusInfo>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CShapeInfoGuiComp, IShapeStatusInfo> BaseClass;

	I_BEGIN_COMPONENT(CShapeInfoGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;
};


} // namespace iview




