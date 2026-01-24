// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <iimg/IBitmap.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <GeneratedFiles/iqtgui/ui_CImagePropertiesFrameComp.h>


namespace iqtgui
{


class CImagePropertiesFrameComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CImagePropertiesFrameComp, iimg::IBitmap>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CImagePropertiesFrameComp, iimg::IBitmap> BaseClass;

	I_BEGIN_COMPONENT(CImagePropertiesFrameComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual void OnGuiModelDetached() override;
};


} // namespace iqtgui




