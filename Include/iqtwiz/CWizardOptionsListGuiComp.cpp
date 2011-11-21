#include "iqtwiz/CWizardOptionsListGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtwiz
{


// reimplemented (imod::IModelEditor)

void CWizardOptionsListGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	iprm::ISelectionParam* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	QList<QTreeWidgetItem*> selectedItems = OptionsList->selectedItems();
	if (selectedItems.isEmpty()){
		objectPtr->SetSelectedOptionIndex(iprm::ISelectionParam::NO_SELECTION);
	}
	else{
		QTreeWidgetItem* selectedItemPtr = selectedItems.first();

		int selectedIndex = selectedItemPtr->data(0, DR_SELECTION_INDEX).toInt();

		objectPtr->SetSelectedOptionIndex(selectedIndex);
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CWizardOptionsListGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	OptionsList->clear();
	
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		CreateOptionsTree(objectPtr);
	}
}


// protected slots

void CWizardOptionsListGuiComp::on_OptionsList_itemSelectionChanged()
{
	if (!IsUpdateBlocked() && IsModelAttached()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


// private methods

void CWizardOptionsListGuiComp::CreateOptionsTree(const iprm::IParamsManager* paramsManagerPtr, QTreeWidgetItem* parentItemPtr)
{
	if (paramsManagerPtr != NULL){
		int selectedOptionIndex = paramsManagerPtr->GetSelectedOptionIndex();

		int pagesCount = paramsManagerPtr->GetParamsSetsCount();
		for (int optionIndex = 0; optionIndex < pagesCount; optionIndex++){
			QString optionName = iqt::GetQString(paramsManagerPtr->GetParamsSetName(optionIndex));

			QTreeWidgetItem* itemPtr = new QTreeWidgetItem(OptionsList);
			itemPtr->setText(0, optionName);
			itemPtr->setData(0, DR_SELECTION_INDEX, optionIndex);

			if (parentItemPtr != NULL){
				parentItemPtr->addChild(itemPtr);
			}
			else{
				OptionsList->addTopLevelItem(itemPtr);
			}

			if (selectedOptionIndex == optionIndex){
				itemPtr->setSelected(true);
			}
		}
	}
}


} // namespace iqtwiz


