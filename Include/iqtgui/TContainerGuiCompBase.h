#ifndef iqtgui_TContainerGuiCompBase_included
#define iqtgui_TContainerGuiCompBase_included


// Qt includes
#include <QtCore/QMap>
#include <QtGui/QToolBox>
#include <QtGui/QGroupBox>
#include <QtGui/QSplitter>

// ACF includes
#include "imod/CMultiModelDispatcherBase.h"
#include "imod/CMultiModelBridgeBase.h"
#include "iprm/IEnableableParam.h"
#include "iprm/CSelectionParam.h"
#include "iprm/COptionsManager.h"
#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


template <class GuiCompBase>
class TContainerGuiCompBase:
			public GuiCompBase,
			protected imod::CMultiModelDispatcherBase
{
public:
	typedef GuiCompBase BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	enum DesignType
	{
		DT_SIMPLE,
		DT_TOOL_BOX,
		DT_TAB_WIDGET,
		DT_STACK,
		DT_SPLITTER
	};

	I_BEGIN_BASE_COMPONENT(TContainerGuiCompBase);
		I_REGISTER_SUBELEMENT(PageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, iprm::ISelectionParam, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, iprm::IOptionsList, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, iser::ISerializable, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, imod::IModel, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, istd::IChangeable, ExtractPageModel);
		I_ASSIGN_MULTI_0(m_slaveWidgetsVisualCompPtr, "GuiVisualInfos", "Provide visual information for each GUI", false);
		I_ASSIGN_TO(m_slaveWidgetsModelCompPtr, m_slaveWidgetsVisualCompPtr, false);
		I_ASSIGN_MULTI_0(m_pageActivatorsCompPtr, "PageActivators", "Optional activators for each page", false);
		I_ASSIGN_TO(m_pageActivatorsModelCompPtr, m_pageActivatorsCompPtr, false);
		I_ASSIGN_MULTI_0(m_pageNamesAttrPtr, "Names", "List of the page titles", true);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for page icons", false, 16);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Use horizontal layout", true, false);
		I_ASSIGN(m_designTypeAttrPtr, "DesignType", "Type of design:\n* 0 - simple\n* 1 - tool box\n* 2 - tab\n* 3 - widget stack\n* 4 - splitter", true, DT_SIMPLE);
		I_ASSIGN(m_useSameStretchingFactorAttrPtr, "UseSameStretchingFactor", "Set the same stretching factor for all widgets. Only for group box mode", false, false);
		I_ASSIGN(m_insertSpacerAttrPtr, "InsertSpacer", "If enabled, spacer will be added to fill whole available area", true, false);
	I_END_COMPONENT;

	TContainerGuiCompBase();

protected:
	/**
		Get the number of the pages in the UI container.
	*/
	virtual int GetPagesCount() const = 0;

	/**
		Get the UI component for a given page.
	*/
	virtual iqtgui::IGuiObject* GetPageGuiComponent(int pageIndex) const = 0;

protected:
	/**
		Get the index of the currently active page in the container widget.
	*/
	int GetCurrentPageIndex() const;

	/**
		Get page container widget.
	*/
	virtual QWidget* GetGuiContainerPtr() const;

	/**
		Create container GUI in some parent widget.
	*/
	virtual bool CreateContainerGui(QWidget& parentWidget);

	/**
		Get currently active page in the container.
	*/
	virtual int GetCurrentPage(QWidget& containerWidget) const;

	/**
		Set active page in the container.
	*/
	virtual bool SetCurrentPage(QWidget& containerWidget, int pageIndex) const;

	/**
		Set the page title.
	*/
	virtual QString GetPageTitle(QWidget& containerWidget, int pageIndex) const;

	/**
		Set the page title.
	*/
	virtual void SetPageTitle(QWidget& containerWidget, int pageIndex, const QString& pageTitle) const;

	/**
		Set the page icon.
	*/
	virtual void SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon) const;

	/**
		Set the page tool tip.
	*/
	virtual void SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip) const;

	/**
		Add a new page to the widget container.
	*/
	virtual int AddPageToContainerWidget(QWidget& containerWidget, iqtgui::IGuiObject& pageGui, const QString& pageTitle) const;

	/**
		Set page enabled/disabled.
	*/
	virtual bool SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled = true) const;

	/**
		Set the size page icon.
	*/
	virtual bool SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize) const;

	/**
		Get the name corresponding to a page GUI element.
	*/
	QString GetPageGuiName(const iqtgui::IGuiObject& pageGui) const;

	/**
		Create a page in the page container widget.
	*/
	bool CreatePage(QWidget* containerWidgetPtr, int guiIndex);

	/**
		Update visual status of the page elements.
	*/
	void UpdateVisualElements();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

private:
	class PageModel:
				public iprm::CSelectionParam,
				public imod::CMultiModelBridgeBase,
				public iprm::IOptionsList
	{
	public:
		typedef iprm::CSelectionParam BaseClass;

		PageModel();

		void SetParent(TContainerGuiCompBase* parentPtr);
		void UpdatePageState();

		// reimplemented (iprm::ISelectionParam)
		virtual bool SetSelectedOptionIndex(int index);

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;

	protected:
		// reimplemented (istd::IChangeable)
		virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	private:
		TContainerGuiCompBase* m_parentPtr;
	};

	template <class InterfaceType>
	static InterfaceType* ExtractPageModel(TContainerGuiCompBase& component)
	{
		return &component.m_pageModel;
	}

protected:
	I_MULTIREF(IVisualStatusProvider, m_slaveWidgetsVisualCompPtr);
	I_MULTIREF(imod::IModel, m_slaveWidgetsModelCompPtr);
	I_MULTIREF(iprm::IEnableableParam, m_pageActivatorsCompPtr);
	I_MULTIREF(imod::IModel, m_pageActivatorsModelCompPtr);
	I_MULTIATTR(QString, m_pageNamesAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
	I_ATTR(int, m_designTypeAttrPtr);
	I_ATTR(bool, m_useSameStretchingFactorAttrPtr);
	I_ATTR(bool, m_insertSpacerAttrPtr);

private:
	typedef QMap<int /*page index in the widget container*/, int /*logical GUI element index*/> PageToGuiIndexMap;
	PageToGuiIndexMap m_pageToGuiIndexMap;

	QMap<const iqtgui::IGuiObject*, QString> m_guiNamesMap;

	imod::TModelWrap<PageModel> m_pageModel;

	/**
		A container that depends on \c DesignType, i.e. QWidget, QToolBox or QTabWidget
	*/
	QWidget* m_guiContainerPtr;
};


// public methods

template <class GuiCompBase>
TContainerGuiCompBase<GuiCompBase>::TContainerGuiCompBase()
	:m_guiContainerPtr(NULL)
{
}


// protected methods

template <class GuiCompBase>
int TContainerGuiCompBase<GuiCompBase>::GetCurrentPageIndex() const
{
	QWidget* containerWidgetPtr = GetGuiContainerPtr();
	if (containerWidgetPtr != NULL){
		return GetCurrentPage(*containerWidgetPtr);
	}

	return -1;
}


template <class GuiCompBase>
QWidget* TContainerGuiCompBase<GuiCompBase>::GetGuiContainerPtr() const
{
	return m_guiContainerPtr;
}


template <class GuiCompBase>
bool TContainerGuiCompBase<GuiCompBase>::CreateContainerGui(QWidget& parentWidget)
{
	// initialize the GUI container
	QLayout* layoutPtr = parentWidget.layout();
	if (layoutPtr == NULL){
		if (*m_useHorizontalLayoutAttrPtr){
			layoutPtr = new QHBoxLayout(&parentWidget);
		}
		else{
			layoutPtr = new QVBoxLayout(&parentWidget);
		}
	}
	layoutPtr->setMargin(0);

	QToolBox* toolBoxPtr;
	QTabWidget* tabWidgetPtr;
	QSplitter* splitterPtr;
	int guiMode = *m_designTypeAttrPtr;

	switch (guiMode){
		case DT_TAB_WIDGET:
			parentWidget.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
			m_guiContainerPtr = tabWidgetPtr = new QTabWidget(&parentWidget);
			layoutPtr->addWidget(tabWidgetPtr);
			break;

		case DT_TOOL_BOX:
			parentWidget.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
			m_guiContainerPtr = toolBoxPtr = new QToolBox(&parentWidget);
			toolBoxPtr->setBackgroundRole(QPalette::Window);
			layoutPtr->addWidget(toolBoxPtr);
			break;
			
		case DT_SPLITTER:
			m_guiContainerPtr = splitterPtr = new QSplitter(&parentWidget);
			if (*m_useHorizontalLayoutAttrPtr){
				splitterPtr->setOrientation(Qt::Horizontal);
			}
			else{
				splitterPtr->setOrientation(Qt::Vertical);
			}
			layoutPtr->addWidget(splitterPtr);
			break;

		default:
			m_guiContainerPtr = &parentWidget;
	}

	if (m_iconSizeAttrPtr.IsValid()){
		QSize iconSize = QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue());
		if (!iconSize.isNull() && iconSize.isValid() && !iconSize.isEmpty()){
			SetPageIconSize(*m_guiContainerPtr, iconSize);
		}
	}

	if (*m_insertSpacerAttrPtr){
		QSpacerItem* spacerPtr = NULL;
		if (*m_useHorizontalLayoutAttrPtr){
			spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
		}
		else{
			spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
		}

		layoutPtr->addItem(spacerPtr);
	}

	return (m_guiContainerPtr != NULL);
}


template <class GuiCompBase>
int TContainerGuiCompBase<GuiCompBase>::GetCurrentPage(QWidget& containerWidget) const
{
	return containerWidget.property("currentIndex").toInt();
}


template <class GuiCompBase>
bool TContainerGuiCompBase<GuiCompBase>::SetCurrentPage(QWidget& containerWidget, int pageIndex) const
{
	return containerWidget.setProperty("currentIndex", pageIndex);
}


template <class GuiCompBase>
QString TContainerGuiCompBase<GuiCompBase>::GetPageTitle(QWidget& containerWidget, int pageIndex) const
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		return tabWidgetPtr->tabText(pageIndex);
	}

	return QString();
}


template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::SetPageTitle(QWidget& containerWidget, int pageIndex, const QString& pageTitle) const
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->setTabText(pageIndex, pageTitle);
	}
}


template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon) const
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->setTabIcon(pageIndex, pageIcon);
	}
}


template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip) const
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->setTabToolTip(pageIndex, pageToolTip);
	}
}


template <class GuiCompBase>
int TContainerGuiCompBase<GuiCompBase>::AddPageToContainerWidget(
			QWidget& containerWidget,
			iqtgui::IGuiObject& pageGui,
			const QString& pageTitle) const
{
	int guiMode = *m_designTypeAttrPtr;
	bool addSpacer = false;
	int pageIndex = -1;

	QWidget* panelPtr = NULL;

	if (guiMode == DT_TAB_WIDGET){
		panelPtr = new QWidget(&containerWidget);
		new QVBoxLayout(panelPtr);
		QTabWidget* tabWidgetPtr = static_cast<QTabWidget*>(&containerWidget);
		pageIndex = tabWidgetPtr->addTab(panelPtr, pageTitle);

		addSpacer = true;
	}
	else if (guiMode == DT_TOOL_BOX){
		panelPtr = new QWidget(&containerWidget);
		QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
		panelLayoutPtr->setContentsMargins(6, 0, 6, 0);
		QToolBox* toolBoxPtr = static_cast<QToolBox*>(&containerWidget);
		pageIndex = toolBoxPtr->addItem(panelPtr, pageTitle);

		addSpacer = true;
	}
	else if (guiMode == DT_SPLITTER){
		QSplitter* splitterPtr = static_cast<QSplitter*>(&containerWidget);
		panelPtr = new QWidget(&containerWidget);
		QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
		panelLayoutPtr->setMargin(0);
		splitterPtr->addWidget(panelPtr);
	}
	else{
		if (!pageTitle.isEmpty()){
			panelPtr = new QGroupBox(pageTitle, &containerWidget);
			new QVBoxLayout(panelPtr);
		}
		else{
			panelPtr = new QWidget(&containerWidget);
			QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
			panelLayoutPtr->setContentsMargins(0, 0, 0, 0);
		}
		QLayout* parentLayoutPtr = containerWidget.layout();
		if (parentLayoutPtr != NULL){
			parentLayoutPtr->addWidget(panelPtr);
		}
	}

	if (pageGui.GetWidget() != NULL){
		QLayout* panelLayoutPtr = panelPtr->layout();
		if (panelLayoutPtr != NULL){
			panelLayoutPtr->addWidget(pageGui.GetWidget());
		}
		else{
			pageGui.GetWidget()->setParent(panelPtr);
		}
	}
	else{
		pageGui.CreateGui(panelPtr);
	}

	if (addSpacer){
		QLayout* panelLayoutPtr = panelPtr->layout();
		if (panelLayoutPtr != NULL){
			QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
			panelLayoutPtr->addItem(spacerPtr);
		}
	}

	return pageIndex;
}


template <class GuiCompBase>
bool TContainerGuiCompBase<GuiCompBase>::SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled) const
{
	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(&containerWidget);
	if (tabWidgetPtr != NULL){
		tabWidgetPtr->setTabEnabled(pageIndex, isPageEnabled);

		return true;
	}

	QToolBox* toolBoxPtr = dynamic_cast<QToolBox*>(&containerWidget);
	if (toolBoxPtr != NULL){
		toolBoxPtr->setItemEnabled(pageIndex, isPageEnabled);

		return true;
	}

	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(&containerWidget);
	if (splitterPtr != NULL){
		QWidget* pageWidgetPtr = splitterPtr->widget(pageIndex);
		Q_ASSERT(pageWidgetPtr != NULL);

		pageWidgetPtr->setEnabled(isPageEnabled);

		return true;
	}

	return false;
}


template <class GuiCompBase>
bool TContainerGuiCompBase<GuiCompBase>::SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize) const
{
	return containerWidget.setProperty("iconSize", pageIconSize);
}


template <class GuiCompBase>
QString TContainerGuiCompBase<GuiCompBase>::GetPageGuiName(const iqtgui::IGuiObject& pageGui) const
{
	if (m_guiNamesMap.contains(&pageGui)){
		return m_guiNamesMap[&pageGui];
	}

	return QString();
}


template <class GuiCompBase>
bool TContainerGuiCompBase<GuiCompBase>::CreatePage(QWidget* containerWidgetPtr, int guiIndex)
{
	iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(guiIndex);
	if (guiPtr != NULL){
		QString pageTitle = GetPageGuiName(*guiPtr);

		int pageIndex = AddPageToContainerWidget(*containerWidgetPtr, *guiPtr, pageTitle);
		if (pageIndex < 0){
			pageIndex = guiIndex;
		}

		m_pageToGuiIndexMap[pageIndex] = guiIndex;

		if (		(guiIndex < m_slaveWidgetsVisualCompPtr.GetCount()) &&
					(guiIndex < m_slaveWidgetsModelCompPtr.GetCount())){
			imod::IModel* modelPtr = m_slaveWidgetsModelCompPtr[guiIndex];
			if ((m_slaveWidgetsVisualCompPtr[guiIndex] != NULL) && (modelPtr != NULL)){
				RegisterModel(modelPtr, guiIndex);
			}
		}

		return true;
	}

	return false;
}


template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::UpdateVisualElements()
{
	QWidget* widgetPtr = GetQtWidget();
	Q_ASSERT(widgetPtr != NULL);

	int visualProvidersCount = m_slaveWidgetsVisualCompPtr.GetCount();

	for (		PageToGuiIndexMap::ConstIterator iter = m_pageToGuiIndexMap.constBegin();
				iter != m_pageToGuiIndexMap.constEnd();
				++iter){
		int pageIndex = iter.key();
		Q_ASSERT(pageIndex >= 0);
		Q_ASSERT(pageIndex < GetPagesCount());

		int guiIndex = iter.value();
		Q_ASSERT(guiIndex >= 0);
		Q_ASSERT(guiIndex < GetPagesCount());

		QIcon pageIcon;
		QString pageToolTip;

		if (guiIndex < visualProvidersCount){
			const IVisualStatusProvider* visualProviderPtr = m_slaveWidgetsVisualCompPtr[guiIndex];
			if (visualProviderPtr != NULL){
				pageIcon = visualProviderPtr->GetStatusIcon();
				pageToolTip = visualProviderPtr->GetStatusText();
			}
		}

		SetPageIcon(*widgetPtr, pageIndex, pageIcon);
		SetPageToolTip(*widgetPtr, pageIndex, pageToolTip);
	}

	m_pageModel.UpdatePageState();
}


// reimplemented (iqtgui::CGuiComponentBase)

template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* guiContainerPtr = GetGuiContainerPtr();

	int pagesCount = GetPagesCount();
	for (int pageIndex = 0; pageIndex < pagesCount; pageIndex++){
		CreatePage(guiContainerPtr, pageIndex);
	}
}


template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::OnGuiDestroyed()
{
	m_pageToGuiIndexMap.clear();

	int pagesCount = GetPagesCount();
	for (int pageIndex = 0; pageIndex < pagesCount; pageIndex++){
		iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(pageIndex);
		if (guiPtr != NULL){
			guiPtr->DestroyGui();
		}
	}

	BaseClass::OnGuiDestroyed();
}


// reimplemented (icomp::CComponentBase)

template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// Map page GUIs with their names:
	int pagesCount = GetPagesCount();
	for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
		iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(pageIndex);
		if (guiPtr == NULL){
			continue;
		}

		QString name;
		if (pageIndex < m_pageNamesAttrPtr.GetCount()){
			name = m_pageNamesAttrPtr[pageIndex];
		}
		else{
			continue;
		}

		m_guiNamesMap[guiPtr] = name;
	}

	m_pageModel.SetParent(this);
}


template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::OnComponentDestroyed()
{
	m_guiNamesMap.clear();

	m_pageModel.SetParent(NULL);

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CMultiModelDispatcherBase)

template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (IsGuiCreated()){
		UpdateVisualElements();
	}
}


// public methods of the embedded class PageModel

template <class GuiCompBase>
TContainerGuiCompBase<GuiCompBase>::PageModel::PageModel()
:	m_parentPtr(NULL)
{
	m_selectedOptionIndex = 0;

	BaseClass::SetSelectionConstraints(this);
}


template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::PageModel::SetParent(TContainerGuiCompBase* parentPtr)
{
	if (parentPtr != m_parentPtr){
		if (m_parentPtr != NULL){
			EnsureModelsDetached();
		}

		if (parentPtr != NULL){
			int activatorsCount = qMin(parentPtr->m_pageNamesAttrPtr.GetCount(), parentPtr->m_pageActivatorsModelCompPtr.GetCount());
			for (int pageIndex = 0; pageIndex < activatorsCount; ++pageIndex){
				imod::IModel* modelPtr = parentPtr->m_pageActivatorsModelCompPtr[pageIndex];

				if (!modelPtr->IsAttached(this)){
					modelPtr->AttachObserver(this);
				}
			}
		}

		m_parentPtr = parentPtr;
	}
}


template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::PageModel::UpdatePageState()
{
	Q_ASSERT(m_parentPtr != NULL);

	QWidget* containerWidgetPtr = m_parentPtr->GetGuiContainerPtr();
	Q_ASSERT(containerWidgetPtr != NULL);

	int activatorsCount = qMin(m_parentPtr->m_pageActivatorsCompPtr.GetCount(), m_parentPtr->GetPagesCount());
	for (int i = 0; i < activatorsCount; ++i){
		const iprm::IEnableableParam* paramPtr = m_parentPtr->m_pageActivatorsCompPtr[i];
		if (paramPtr != NULL){
			m_parentPtr->SetPageEnabled(*containerWidgetPtr, i, paramPtr->IsEnabled());
		}
	}
}


// reimplemented (iprm::ISelectionParam)

template <class GuiCompBase>
bool TContainerGuiCompBase<GuiCompBase>::PageModel::SetSelectedOptionIndex(int index)
{
	Q_ASSERT(m_parentPtr != NULL);

	if (BaseClass::SetSelectedOptionIndex(index)){
		QWidget* containerWidgetPtr = m_parentPtr->GetGuiContainerPtr();
		if (containerWidgetPtr != NULL){
			m_parentPtr->SetCurrentPage(*containerWidgetPtr, index);
		}
	}

	return false;
}


// reimplemented (iprm::IOptionsList)

template <class GuiCompBase>
int TContainerGuiCompBase<GuiCompBase>::PageModel::GetOptionsFlags() const
{
	return SCF_SUPPORT_DISABLED | SCF_SUPPORT_UNIQUE_ID;
}


template <class GuiCompBase>
int TContainerGuiCompBase<GuiCompBase>::PageModel::GetOptionsCount() const
{
	Q_ASSERT(m_parentPtr != NULL);

	return m_parentPtr->m_pageNamesAttrPtr.GetCount();
}


template <class GuiCompBase>
QString TContainerGuiCompBase<GuiCompBase>::PageModel::GetOptionName(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index < m_parentPtr->m_pageNamesAttrPtr.GetCount());

	return m_parentPtr->m_pageNamesAttrPtr[index];
}


template <class GuiCompBase>
QString TContainerGuiCompBase<GuiCompBase>::PageModel::GetOptionDescription(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	QWidget* containerWidgetPtr = m_parentPtr->GetGuiContainerPtr();
	if (containerWidgetPtr != NULL){
		return m_parentPtr->GetPageTitle(*containerWidgetPtr, index);
	}

	return QString();
}


template <class GuiCompBase>
QByteArray TContainerGuiCompBase<GuiCompBase>::PageModel::GetOptionId(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index < m_parentPtr->m_pageNamesAttrPtr.GetCount());

	return m_parentPtr->m_pageNamesAttrPtr[index].toUtf8();
}


template <class GuiCompBase>
bool TContainerGuiCompBase<GuiCompBase>::PageModel::IsOptionEnabled(int index) const
{
	Q_ASSERT(m_parentPtr != NULL);
	Q_ASSERT(index >= 0);

	if (index >= m_parentPtr->GetPagesCount()){
		return false;
	}

	if (index < m_parentPtr->m_pageActivatorsCompPtr.GetCount()){
		const iprm::IEnableableParam* paramPtr = m_parentPtr->m_pageActivatorsCompPtr[index];
		if (paramPtr != NULL){
			return paramPtr->IsEnabled();
		}
	}

	return true;
}


// protected methods of the embedded class PageModel

// reimplemented (istd::IChangeable)

template <class GuiCompBase>
void TContainerGuiCompBase<GuiCompBase>::PageModel::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	Q_ASSERT(m_parentPtr != NULL);

	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	if (m_parentPtr->IsGuiCreated()){
		UpdatePageState();
	}
}


} // namespace iqtgui


#endif // !iqtgui_TContainerGuiCompBase_included


