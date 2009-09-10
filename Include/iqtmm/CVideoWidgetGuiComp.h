#ifndef iqtmm_CVideoWidgetGuiComp_included
#define iqtmm_CVideoWidgetGuiComp_included


// Qt includes
#include <Phonon>


// ACF includes
#include "iqtgui/TGuiComponentBase.h"


namespace iqtmm
{


class CVideoWidgetGuiComp: public iqtgui::TGuiComponentBase<Phonon::VideoWidget>
{
	Q_OBJECT
public:
	typedef iqtgui::TGuiComponentBase<Phonon::VideoWidget> BaseClass;

	I_BEGIN_COMPONENT(CVideoWidgetGuiComp)
		I_ASSIGN(m_videoPathAttrPtr, "VideoPath", "VideoPath", true, "");
	I_END_COMPONENT

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_ATTR(istd::CString, m_videoPathAttrPtr);
};


} // namespace iqtmm


#endif // !iqtmm_CVideoWidgetGuiComp_included

