#ifndef iqtgui_CImagePropertiesFrameComp_included
#define iqtgui_CImagePropertiesFrameComp_included


#include "iimg/IBitmap.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/Generated/ui_CImagePropertiesFrameComp.h"


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

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;
};


} // namespace iqtgui


#endif // !iqtgui_CImagePropertiesFrameComp_included


