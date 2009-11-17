#ifndef iqtgui_CFilePersistenceGuiComp_included
#define iqtgui_CFilePersistenceGuiComp_included


#include "iser/IFileLoader.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "iqtgui/Generated/ui_CFilePersistenceGuiComp.h"


namespace iqtgui
{


class CFilePersistenceGuiComp: public iqtgui::TDesignerGuiCompBase<Ui::CFilePersistenceGuiComp>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CFilePersistenceGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CFilePersistenceGuiComp);
		I_ASSIGN(m_objectCompPtr, "Object", "Object will be restored and stored", true, "Object");
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "File loader used to store and restore object", true, "FileLoader");
		I_ASSIGN(m_showLoadAttrPtr, "ShowLoad", "Show load button", true, true);
		I_ASSIGN(m_showSaveAttrPtr, "ShowSave", "Show save button", true, true);
	I_END_COMPONENT;

protected Q_SLOTS:
	void on_LoadButton_clicked();
	void on_SaveButton_clicked();

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	void OnGuiCreated();

private:
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_ATTR(bool, m_showLoadAttrPtr);
	I_ATTR(bool, m_showSaveAttrPtr);
};

} // namespace iqtgui


#endif // !iqtgui_CFilePersistenceGuiComp_included


