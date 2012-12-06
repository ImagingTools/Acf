#ifndef iqtgui_CStatusGuiComp_included
#define iqtgui_CStatusGuiComp_included


// Qt includes
#include <QtGui/QLabel>

// ACF includes
#include "istd/IInformationProvider.h"
#include "ibase/TModelObserverCompWrap.h"
#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"


namespace iqtgui
{


class CStatusGuiComp: 
			public ibase::TModelObserverCompWrap<
						iqtgui::TGuiObserverWrap<
									iqtgui::TGuiComponentBase<QLabel>,
									imod::TSingleModelObserverBase<istd::IInformationProvider> > >
{
public:
	typedef ibase::TModelObserverCompWrap<
				iqtgui::TGuiObserverWrap<
							iqtgui::TGuiComponentBase<QLabel>,
							imod::TSingleModelObserverBase<istd::IInformationProvider> > > BaseClass;

	I_BEGIN_COMPONENT(CStatusGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
};


} // namespace iqtgui


#endif // !iqtgui_CStatusGuiComp_included

