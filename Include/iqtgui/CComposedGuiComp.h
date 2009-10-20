#ifndef iqtgui_CComposedGuiComp_included
#define iqtgui_CComposedGuiComp_included


// Qt includes
#include <QWidget>


// ACF includes
#include "istd/CString.h"

#include "iser/IFileLoader.h"

#include "iprm/IParamsSet.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"


namespace iqtgui
{


class CComposedGuiComp: public iqtgui::TGuiComponentBase<QWidget>
{
public:
	typedef iqtgui::TGuiComponentBase<QWidget> BaseClass;

	I_BEGIN_COMPONENT(CComposedGuiComp);
		I_ASSIGN_MULTI_0(m_guisCompPtr, "Guis", "List of GUI's", true);
		I_ASSIGN_MULTI_0(m_namesAttrPtr, "Names", "List of of gui names", false);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Use horizontal layout", true, false);
		I_ASSIGN(m_designTypeAttrPtr, "DesignType", "Type of design:\n* 0 - simple\n* 1 - tool box\n* 2 - tab", true, false);
	I_END_COMPONENT;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_MULTIREF(iqtgui::IGuiObject, m_guisCompPtr);
	I_MULTIATTR(istd::CString, m_namesAttrPtr);
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
	I_ATTR(int, m_designTypeAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CComposedGuiComp_included


