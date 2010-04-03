#ifndef iqtgui_CTabbedMultiViewGuiComp_included
#define iqtgui_CTabbedMultiViewGuiComp_included


// Qt includes
#include <QTabWidget>


// ACF includes
#include "imod/IModelEditor.h"
#include "imod/CSingleModelObserverBase.h"

#include "ibase/ICommandsProvider.h"

#include "iqtgui/CHierarchicalCommand.h"
#include "iqtgui/CTabContainerGuiComp.h"


namespace iqtgui
{


/**
	A multi view observer implementation.
*/
class CTabbedMultiViewGuiComp:
			public iqtgui::CTabContainerGuiComp,
			public imod::CSingleModelObserverBase,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::CTabContainerGuiComp BaseClass;
	typedef imod::CSingleModelObserverBase BaseClass2;

	I_BEGIN_COMPONENT(CTabbedMultiViewGuiComp)
		I_REGISTER_INTERFACE(imod::IObserver)
		I_REGISTER_INTERFACE(ibase::ICommandsProvider)
		I_ASSIGN_MULTI_0(m_observersCompPtr, "Editors", "Editors", true)
	I_END_COMPONENT

protected:
	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

private:
	I_MULTIREF(imod::IObserver, m_observersCompPtr);

	mutable iqtgui::CHierarchicalCommand m_commands;
};


} // namespace iqtgui


#endif // !iqtgui_CTabbedMultiViewGuiComp_included


