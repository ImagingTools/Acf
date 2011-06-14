#ifndef iqtgui_CImagePropertyGuiComp_included
#define iqtgui_CImagePropertyGuiComp_included


#include "iimg/IBitmap.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/Generated/ui_CImagePropertyGuiComp.h"


namespace iqtgui
{


class CImagePropertyGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CImagePropertyGuiComp, iimg::IBitmap>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CImagePropertyGuiComp, iimg::IBitmap> BaseClass;

	I_BEGIN_COMPONENT(CImagePropertyGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);
};


} // namespace iqtgui


#endif // !iqtgui_CImagePropertyGuiComp_included


