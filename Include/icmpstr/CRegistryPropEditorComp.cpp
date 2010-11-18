#include "icmpstr/CRegistryPropEditorComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


// public methods
	
namespace icmpstr
{


// reimplmented (imod::IModelEditor)

void CRegistryPropEditorComp::UpdateEditor(int /*updateFlags*/)
{
	if (!IsGuiCreated()){
		return;
	}

	const icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr == NULL){
		return;
	}

	DescriptionEdit->setText(iqt::GetQString(registryPtr->GetDescription()));
	KeywordsEdit->setText(iqt::GetQString(registryPtr->GetKeywords()));
	/*
	CComponentCategoryEncoder encoder;
	m_categoryComboBox->SetItemsChecked(encoder.GetCategoryNames(registryPtr->GetCategory()));
*/
	CreateOverview();
}


void CRegistryPropEditorComp::UpdateModel() const
{
}


// reimplemented (imod::TGuiObserverWrap)

void CRegistryPropEditorComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	DescriptionEdit->setEnabled(true);
	KeywordsEdit->setEnabled(true);
}


void CRegistryPropEditorComp::OnGuiModelDetached()
{
	DescriptionEdit->clear();
	KeywordsEdit->clear();

	DescriptionEdit->setDisabled(true);
	KeywordsEdit->setDisabled(true);

	OverviewTree->clear();

	ErrorsLabel->clear();

	BaseClass::OnGuiModelDetached();
}


// protected methods

void CRegistryPropEditorComp::CreateOverview()
{
	// reset view:
	OverviewTree->clear();

	// setup colors:
	QFont boldFont = qApp->font();
	boldFont.setBold(true);

	// create overview infos:
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		const icomp::IRegistry::ExportedInterfacesMap& exportedInterfaces = registryPtr->GetExportedInterfacesMap();
		if (!exportedInterfaces.empty()){
			QTreeWidgetItem* exportedInterfacesItemPtr = new QTreeWidgetItem();
			exportedInterfacesItemPtr->setText(0, tr("Exported Interfaces"));
			exportedInterfacesItemPtr->setFont(0, boldFont);
			
			OverviewTree->addTopLevelItem(exportedInterfacesItemPtr);

			for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator index = exportedInterfaces.begin();
						index != exportedInterfaces.end();
						index++){
				QTreeWidgetItem* exportedInterfaceItemPtr = new QTreeWidgetItem();
				exportedInterfaceItemPtr->setText(0, QString(index->second.c_str()));
				exportedInterfaceItemPtr->setText(1, QString(index->first.GetName().c_str()));
				exportedInterfacesItemPtr->addChild(exportedInterfaceItemPtr);		
			}

			exportedInterfacesItemPtr->setExpanded(true);
		}

		const icomp::IRegistry::ExportedComponentsMap& exportedComponents = registryPtr->GetExportedComponentsMap();
		if (!exportedComponents.empty()){
			QTreeWidgetItem* exportedComponentsItemPtr = new QTreeWidgetItem();
			exportedComponentsItemPtr->setText(0, tr("Exported Components"));
			exportedComponentsItemPtr->setFont(0, boldFont);

			OverviewTree->addTopLevelItem(exportedComponentsItemPtr);

			for (		icomp::IRegistry::ExportedComponentsMap::const_iterator index = exportedComponents.begin();
						index != exportedComponents.end();
						index++){
				QTreeWidgetItem* exportedComponentItemPtr = new QTreeWidgetItem();
				exportedComponentItemPtr->setText(0, QString(index->second.c_str()));
				exportedComponentItemPtr->setText(1, QString(index->first.c_str()));
				exportedComponentsItemPtr->addChild(exportedComponentItemPtr);		
			}

			exportedComponentsItemPtr->setExpanded(true);
		}

		if (m_consistInfoCompPtr.IsValid()){
			TextLog textLog;
			if (!m_consistInfoCompPtr->IsRegistryValid(*registryPtr, false, true, &textLog)){
				ErrorsLabel->setText(iqt::GetQString(textLog));
				ErrorsFrame->setVisible(true);
			}
			else{
				ErrorsFrame->setVisible(false);
			}
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CRegistryPropEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	OverviewTree->header()->setResizeMode(QHeaderView::ResizeToContents);
	OverviewTree->setStyleSheet("QTreeView {background: palette(window)} QTreeView::branch {background: palette(window);} QTreeView::item {min-height: 25px}");
/*
	m_categoryComboBox = new iqtgui::CCheckableComboBox(CategoryGroupBox);
	QLayout* groubBoxLayout = CategoryGroupBox->layout();
	if (groubBoxLayout != NULL){
		groubBoxLayout->addWidget(m_categoryComboBox);
	}

	CComponentCategoryEncoder encoder;
	const CComponentCategoryEncoder::CategoriesMap& categoriesMap = encoder.GetCategoriesMap();

	for (		CComponentCategoryEncoder::CategoriesMap::const_iterator iterator = categoriesMap.begin();
				iterator != categoriesMap.end();
				iterator++){		
		m_categoryComboBox->addItem(iterator.value(), false);
	}

	connect(m_categoryComboBox, SIGNAL(EmitActivatedItems(const QStringList&)), this, SLOT(OnCategoriesChanged(const QStringList&)));

	*/
}


// protected slots:

void CRegistryPropEditorComp::on_DescriptionEdit_editingFinished()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		istd::CString description = iqt::GetCString(DescriptionEdit->text());

		if (description != registryPtr->GetDescription()){
			istd::CChangeNotifier notifier(registryPtr);

			registryPtr->SetDescription(description);
		}
	}
}


void CRegistryPropEditorComp::on_KeywordsEdit_editingFinished()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		istd::CString keywords = iqt::GetCString(KeywordsEdit->text());

		if (keywords != registryPtr->GetKeywords()){
			istd::CChangeNotifier notifier(registryPtr);

			registryPtr->SetKeywords(keywords);
		}
	}
}


void CRegistryPropEditorComp::OnCategoriesChanged(const QStringList& /*categories*/)
{/*
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		CComponentCategoryEncoder encoder;
		int category = encoder.GetCategoryFromNames(categories);

		if (category != registryPtr->GetCategory()){
			istd::CChangeNotifier notifier(registryPtr);

			registryPtr->SetCategory(category);
		}
	}
	*/
}




// public methods of embedded class TextLog

// reimplemented (ibase::IMessageConsumer)

bool CRegistryPropEditorComp::TextLog::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const ibase::IMessage* /*messagePtr*/) const
{
	return true;
}


void CRegistryPropEditorComp::TextLog::AddMessage(const MessagePtr& messagePtr)
{
	if (messagePtr.IsValid()){
		if (!IsEmpty()){
			operator+=("\n");
		}

		operator+=(messagePtr->GetText());
	}
}


} // namespace icmpstr


