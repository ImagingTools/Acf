#ifndef icmpstr_CAttributeEditorComp_included
#define icmpstr_CAttributeEditorComp_included


// Qt includes
#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QtGui/QItemDelegate>

// ACF includes
#include "istd/TPointerBase.h"
#include "istd/TDelPtr.h"
#include "istd/TOptDelPtr.h"
#include "istd/CClassInfo.h"

#include "imod/CSingleModelObserverBase.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/IRegistryElement.h"

#include "idoc/IHelpViewer.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CTreeWidgetFilter.h"
#include "iqtgui/CItemDelegate.h"

#include "icmpstr/IAttributeSelectionObserver.h"
#include "icmpstr/IElementSelectionInfo.h"
#include "icmpstr/IRegistryConsistInfo.h"

#include "icmpstr/Generated/ui_CAttributeEditorComp.h"


namespace icmpstr
{


class CAttributeEditorComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CAttributeEditorComp, IElementSelectionInfo>
{
    Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CAttributeEditorComp, IElementSelectionInfo> BaseClass;

	I_BEGIN_COMPONENT(CAttributeEditorComp);
		I_ASSIGN(m_metaInfoManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN(m_attributeSelectionObserverCompPtr, "AttributeSelectionObserver", "AM_ATTRIBUTE selection observer", false, "AttributeSelectionObserver");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpViewer", "Shows object info during selection using its type", false, "QuickHelpViewer");
		I_ASSIGN(m_consistInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and attributes", false, "ConsistencyInfo");
		I_ASSIGN(m_registryPropGuiCompPtr, "RegistryPropGui", "Display and edit registry properties if no element is selected", false, "RegistryPropGui");
		I_ASSIGN_TO(m_registryPropObserverCompPtr, m_registryPropGuiCompPtr, false);
	I_END_COMPONENT;

	enum TabIndex
	{
		TI_GENERAL,
		TI_ATTRIBUTES,
		TI_INTERFACES,
		TI_FLAGS,
		TI_EXPORTS
	};

	enum AttrMeaning
	{
		AM_NONE = 0,
		AM_REFERENCE,
		AM_MULTI_REFERENCE,
		AM_ATTRIBUTE,
		AM_SELECTABLE_ATTRIBUTE,
		AM_MULTI_ATTRIBUTE,
		AM_EXPORTED_ATTR,
		AM_EXPORTED_COMP,
		AM_MULTI
	};

	enum AttributeColumns
	{
		NameColumn = 0,
		ValueColumn = 1
	};

	enum
	{
		AttributeMining = Qt::UserRole + 1,
		AttributeId,
		AttributeValue,
		AttributeType,
		ElementId,
		InterfaceName,
		ExportId
	};

public:
	CAttributeEditorComp();

	icomp::IRegistry* GetRegistry() const;
	const icomp::IAttributeStaticInfo* GetStaticAttributeInfo(const std::string& id) const;
	QStringList GetExportAliases(const std::string& attributeName) const;

protected Q_SLOTS:
	void on_AttributeTree_itemSelectionChanged();
	void on_AttributeTree_itemChanged(QTreeWidgetItem* item, int column);
	void on_InterfacesTree_itemSelectionChanged();
	void on_InterfacesTree_itemChanged(QTreeWidgetItem* item, int column);
	void on_AutoInstanceCB_toggled(bool checked);

protected:
	struct AttrInfo
	{
		istd::TPointerBase<const icomp::IRegistryElement::AttributeInfo> infoPtr;
		istd::TPointerBase<const icomp::IAttributeStaticInfo> staticInfoPtr;
	};
	typedef std::map<std::string, AttrInfo> ElementIdToAttrInfoMap;

	void UpdateSelectedAttr();

	bool AddAttributeToView(
				const icomp::IRegistry& registry,
				const std::string& attributeId,
				const ElementIdToAttrInfoMap& infos,
				bool* hasErrorPtr,
				bool* hasWarningPtr,
				bool* hasExportPtr);
	bool DecodeAttribute(
				const iser::ISerializable& attribute,
				QString& text,
				int& meaning);

	void CreateExportedComponentsTree(
				const std::string& elementId,
				const icomp::IComponentStaticInfo& elementStaticInfo,
				QTreeWidgetItem& rootItem) const;

	void UpdateExportIcon();

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// static methods
	static QString DecodeFromEdit(const QString& text);
	static QString EncodeToEdit(const QString& text);

private:
	class AttributeItemDelegate: public iqtgui::CItemDelegate
	{
	public:
		typedef iqtgui::CItemDelegate BaseClass;

		AttributeItemDelegate(CAttributeEditorComp* parentPtr);
		
		// reimplemented (QItemDelegate)
		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index ) const;
		virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

	protected:
		bool SetComponentExportEditor(const std::string& attributeId, QWidget& editor) const;
		bool SetAttributeExportEditor(const std::string& id, const std::string& exportId, QWidget& editor) const;
		bool SetAttributeValueEditor(const std::string& id, int propertyMining, QWidget& editor) const;

		bool SetComponentExportData(const std::string& attributeId, const QWidget& editor) const;
		bool SetAttributeExportData(
					const std::string& id,
					const std::string& typeName,
					const QWidget& editor) const;
		bool SetAttributeValueData(
					const std::string& id,
					const std::string& typeName,
					int propertyMining,
					const QWidget& editor) const;

	private:
		CAttributeEditorComp& m_parent;
	};

	class RegistryObserver: public imod::CSingleModelObserverBase
	{
	public:
		RegistryObserver(CAttributeEditorComp* parentPtr);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CAttributeEditorComp& m_parent;
	};

	AttributeItemDelegate m_attributeItemDelegate;
	RegistryObserver m_registryObserver;

	typedef std::map<std::string, QString> AttributeTypesMap;
	AttributeTypesMap m_attributeTypesMap;
	istd::TDelPtr<iqtgui::CTreeWidgetFilter> m_treeWidgetFilter;

	imod::IModel* m_lastRegistryModelPtr;

	QIcon m_exportIcon;

	I_REF(icomp::IComponentEnvironmentManager, m_metaInfoManagerCompPtr);
	I_REF(IAttributeSelectionObserver, m_attributeSelectionObserverCompPtr);
	I_REF(idoc::IHelpViewer, m_quickHelpViewerCompPtr);
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);
	I_REF(iqtgui::IGuiObject, m_registryPropGuiCompPtr);
	I_REF(imod::IObserver, m_registryPropObserverCompPtr);

	typedef std::map<icomp::CComponentAddress, istd::TOptDelPtr<const icomp::IComponentStaticInfo> > AddressToInfoMap;
};


} // namespace icmpstr


#endif // !icmpstr_CAttributeEditorComp_included

