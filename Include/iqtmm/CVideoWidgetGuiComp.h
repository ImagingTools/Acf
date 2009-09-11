#ifndef iqtmm_CVideoWidgetGuiComp_included
#define iqtmm_CVideoWidgetGuiComp_included


// Qt includes
#include <Phonon>


// ACF includes
#include "iqtmm/IMediaObject.h"

#include "imod/TSingleModelObserverBase.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"


namespace iqtmm
{


class CVideoWidgetGuiComp:
	public iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<Phonon::VideoWidget>, 
				imod::TSingleModelObserverBase<iqtmm::IMediaObject> >

{
	Q_OBJECT
public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<Phonon::VideoWidget>, 
				imod::TSingleModelObserverBase<iqtmm::IMediaObject> > BaseClass;

	I_BEGIN_COMPONENT(CVideoWidgetGuiComp);
		I_REGISTER_INTERFACE(imod::IObserver);
	I_END_COMPONENT();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
};


} // namespace iqtmm


#endif // !iqtmm_CVideoWidgetGuiComp_included

