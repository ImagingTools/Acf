#ifndef iqtgui_CFileTreeProviderGuiComp_included
#define iqtgui_CFileTreeProviderGuiComp_included


// ACF includes
#include "ifile/IFileNameParam.h"
#include "ibase/IQtItemModelProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtgui/Generated/ui_CFileTreeProviderGuiComp.h"


namespace iqtgui
{


class CFileTreeProviderGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CFileTreeProviderGuiComp, ibase::IQtItemModelProvider>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileTreeProviderGuiComp, ibase::IQtItemModelProvider> BaseClass;

	I_BEGIN_COMPONENT(CFileTreeProviderGuiComp);
		I_ASSIGN(m_currentFileCompPtr, "CurrentFile", "Write name of the currently selected file to", false, "CurrentFile");
	I_END_COMPONENT;

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

private Q_SLOTS:
	void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
	I_REF(ifile::IFileNameParam, m_currentFileCompPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CFileTreeProviderGuiComp_included


