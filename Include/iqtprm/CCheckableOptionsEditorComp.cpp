#include "iqtprm/CCheckableOptionsEditorComp.h"


// Qt includes
#include "QtGui/qcheckbox.h"
#include "QtGui/qboxlayout.h"

// ACF includes
#include "iprm/IOptionsList.h"


namespace iqtprm
{


// public methods

// reimplemented (imod::IModelEditor)

void CCheckableOptionsEditorComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	iprm::IOptionsManager* managerPtr = GetObjectPtr();
	if (managerPtr != NULL){
		return;
	}

	istd::TChangeNotifier<iprm::IOptionsManager> updatePtr(managerPtr);

	int optionsCount = managerPtr->GetOptionsCount();
	QWidget* widget = GetQtWidget();
	if (widget != NULL){
		for (int optionsIndex = 0; optionsIndex < optionsCount; optionsIndex++){
			QCheckBox* box = widget->findChild<QCheckBox *>(managerPtr->GetOptionName(optionsIndex));
			if (box != NULL){
				managerPtr->SetOptionEnabled(optionsIndex, box->isChecked());
			}
		}
	}
}


// protected slots

void CCheckableOptionsEditorComp::OnParameterChanged()
{
	DoUpdateModel();
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CCheckableOptionsEditorComp::UpdateGui(int /* updateFlags = 0 */)
{
	Q_ASSERT(IsGuiCreated());

	iprm::IOptionsManager* managerPtr = GetObjectPtr();
	if (managerPtr != NULL){
		return;
	}

	int optionsCount = managerPtr->GetOptionsCount();
	QWidget* widget = GetQtWidget();
	if (widget != NULL){
		for (int optionsIndex = 0; optionsIndex < optionsCount; optionsIndex++){
			QCheckBox* box = (QCheckBox*)widget->findChild<QCheckBox *>(managerPtr->GetOptionName(optionsIndex));
			if (box != NULL){
				box->setChecked(managerPtr->IsOptionEnabled(optionsIndex));
			}
		}
	}
}


// reimplemented (iqt::CGuiObjectBase)

void CCheckableOptionsEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetQtWidget();
	if (widgetPtr != NULL){
		iprm::IOptionsManager* managerPtr = GetObjectPtr();
		if (managerPtr != NULL){
			return;
		}

		int optionCount = managerPtr->GetOptionsCount();

		QBoxLayout* layoutPtr = NULL;

		if (*m_useHorizontalLayoutAttrPtr){
			layoutPtr = new QHBoxLayout(GetWidget());
		}
		else{
			layoutPtr = new QVBoxLayout(GetWidget());
		}

		for (int optionIndex = 0; optionIndex < optionCount; optionIndex++){
			QCheckBox* checkBoxPtr = new QCheckBox(widgetPtr);
			
			checkBoxPtr->setText(managerPtr->GetOptionName(optionIndex));
			connect(checkBoxPtr, SIGNAL(stateChanged(int state)), this, SLOT(OnParameterChanged()));

			layoutPtr->addWidget(checkBoxPtr);
		}
	}
}


} // namespace iqtprm


