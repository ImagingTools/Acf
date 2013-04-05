#ifndef iqtgui_CSettingsContainerGuiComp_included
#define iqtgui_CSettingsContainerGuiComp_included


// Qt includes
#include <QtCore/QMap>
#include <QtGui/QStackedWidget>
#include <QtGui/QListWidget>

// ACF includes
#include "imod/CMultiModelDispatcherBase.h"
#include "iprm/CSelectionParam.h"
#include "iprm/COptionsManager.h"

#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


class CSettingsContainerGuiComp:
				public iqtgui::TGuiComponentBase<QWidget>,
				protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT
public:
	typedef iqtgui::TGuiComponentBase<QWidget> BaseClass;

	I_BEGIN_COMPONENT(CSettingsContainerGuiComp);
		I_REGISTER_SUBELEMENT(TabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iprm::ISelectionParam, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iprm::IOptionsList, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iprm::IOptionsManager, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iser::ISerializable, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, imod::IModel, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, istd::IChangeable, ExtractTabModel);

		I_ASSIGN_MULTI_0(m_slaveWidgetsCompPtr, "Guis", "Slave widgets for tab window", true);
		I_ASSIGN_MULTI_0(m_slaveWidgetsVisualCompPtr, "GuiVisualInfos", "Provide visual information for each GUI", false);
		I_ASSIGN_TO(m_slaveWidgetsModelCompPtr, m_slaveWidgetsVisualCompPtr, false);
		I_ASSIGN_MULTI_0(m_tabNamesAttrPtr, "Names", "Titles for the buttons", true);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for button icons", true, 16);
		I_ASSIGN(m_fixedWidthAttrPtr, "FixedWidth", "Fixed width of the buttons", false, 100);
		I_ASSIGN(m_alternateColorsAttrPtr, "AlternateColors", "Alternate colors for each button", true, true);
		I_ASSIGN(m_menuPositionAttrPtr, "SelectorPosition", "0 - Left\n1 - Right", true, 0);
	I_END_COMPONENT;

protected:
	void UpdateVisualElements();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	class TabModel:
		public iprm::CSelectionParam,
		public iprm::COptionsManager
	{
	public:
		typedef iprm::CSelectionParam BaseClass;
		typedef iprm::COptionsManager BaseClass2;

		TabModel();

		void SetParent(CSettingsContainerGuiComp* parentPtr);

		// reimplemented (iprm::ISelectionParam)
		virtual bool SetSelectedOptionIndex(int index);

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

		// reimplemented (iprm::IOptionsManager)
		virtual bool SetOptionEnabled(int index, bool isEnabled = true);
		virtual bool RemoveOption(int index);
		virtual bool InsertOption(
			QString& optionName,
			const QByteArray& optionId,
			const QString& optionDescription = QString(), 
			int index = -1);

	private:
		CSettingsContainerGuiComp* m_parentPtr;
	};

	template <class InterfaceType>
	static InterfaceType* ExtractTabModel(CSettingsContainerGuiComp& component)
	{
		return &component.m_tabModel;
	}

private:
	I_MULTIREF(IGuiObject, m_slaveWidgetsCompPtr);
	I_MULTIREF(IVisualStatusProvider, m_slaveWidgetsVisualCompPtr);
	I_MULTIREF(imod::IModel, m_slaveWidgetsModelCompPtr);
	I_MULTIATTR(QString, m_tabNamesAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(int, m_fixedWidthAttrPtr);
	I_ATTR(bool, m_alternateColorsAttrPtr);
	I_ATTR(int, m_menuPositionAttrPtr);

	typedef QMap<int, int> TabToGuiIndexMap;
	TabToGuiIndexMap m_tabToGuiIndexMap;

	imod::TModelWrap<TabModel> m_tabModel;

	QStackedWidget* m_stackedWidgetPtr;
	QListWidget* m_menuListPtr;
};


} // namespace iqtgui


#endif // !iqtgui_CSettingsContainerGuiComp_included
