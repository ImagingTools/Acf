// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtprm/CGenericParamsSetGuiComp.h>


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QVariant>
#include <QtCore/QtGlobal>
#include <QtUiTools/QUiLoader>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QDoubleSpinBox>
#else
#include <QtGui/QBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QTextEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#endif

// ACF includes
#include <iprm/INameParam.h>
#include <iprm/ISelectionParam.h>
#include <iprm/IOptionsList.h>
#include <iprm/IEnableableParam.h>
#include <iprm/ITextParam.h>
#include <ifile/IFileNameParam.h>


namespace iqtprm
{


// public methods

CGenericParamsSetGuiComp::CGenericParamsSetGuiComp()
:	m_loadedUiWidgetPtr(NULL)
{
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

QWidget* CGenericParamsSetGuiComp::CreateQtWidget(QWidget* parentPtr)
{
	QWidget* containerPtr = new QWidget(parentPtr);

	if (m_uiFilePathAttrPtr.IsValid()){
		QString uiFilePath = *m_uiFilePathAttrPtr;

		if (!uiFilePath.isEmpty()){
			QFile uiFile(uiFilePath);

			if (uiFile.open(QFile::ReadOnly)){
				QUiLoader loader;
				m_loadedUiWidgetPtr = loader.load(&uiFile, containerPtr);
				uiFile.close();

				if (m_loadedUiWidgetPtr != NULL){
					QVBoxLayout* layoutPtr = new QVBoxLayout(containerPtr);
					layoutPtr->setContentsMargins(0, 0, 0, 0);
					layoutPtr->addWidget(m_loadedUiWidgetPtr);
				}
				else{
					qWarning("CGenericParamsSetGuiComp: Failed to load UI from file: %s", qPrintable(uiFilePath));
				}
			}
			else{
				qWarning("CGenericParamsSetGuiComp: Cannot open UI file: %s", qPrintable(uiFilePath));
			}
		}
	}

	return containerPtr;
}


void CGenericParamsSetGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_isReadOnlyAttrPtr.IsValid() && *m_isReadOnlyAttrPtr){
		BaseClass::SetReadOnly(true);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CGenericParamsSetGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	CreateBindings();
}


void CGenericParamsSetGuiComp::OnGuiModelDetached()
{
	ClearBindings();

	BaseClass::OnGuiModelDetached();
}


void CGenericParamsSetGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	for (int i = 0; i < m_bindings.size(); ++i){
		UpdateWidgetFromParam(m_bindings[i]);
	}
}


void CGenericParamsSetGuiComp::UpdateModel() const
{
	for (int i = 0; i < m_bindings.size(); ++i){
		UpdateParamFromWidget(m_bindings[i]);
	}
}


// reimplemented (icomp::CComponentBase)

void CGenericParamsSetGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_isReadOnlyAttrPtr.IsValid()){
		m_isReadOnly = *m_isReadOnlyAttrPtr;
	}
}


// reimplemented (imod::IModelEditor)

void CGenericParamsSetGuiComp::SetReadOnly(bool state)
{
	if (m_isReadOnlyAttrPtr.IsValid() && *m_isReadOnlyAttrPtr){
		return;
	}

	BaseClass::SetReadOnly(state);
}


// private slots

void CGenericParamsSetGuiComp::OnWidgetValueChanged()
{
	DoUpdateModel();
}


// private methods

void CGenericParamsSetGuiComp::CreateBindings()
{
	ClearBindings();

	if (m_loadedUiWidgetPtr == NULL){
		return;
	}

	iprm::IParamsSet* paramsSetPtr = GetObservedObject();
	if (paramsSetPtr == NULL){
		return;
	}

	QList<QWidget*> allWidgets = m_loadedUiWidgetPtr->findChildren<QWidget*>();

	for (int i = 0; i < allWidgets.size(); ++i){
		QWidget* widgetPtr = allWidgets[i];

		QVariant paramIdVariant = widgetPtr->property("ACF_PARAM_ID");
		if (!paramIdVariant.isValid() || paramIdVariant.isNull()){
			continue;
		}

		QByteArray paramId = paramIdVariant.toByteArray();
		if (paramId.isEmpty()){
			continue;
		}

		iser::ISerializable* paramPtr = paramsSetPtr->GetEditableParameter(paramId);
		if (paramPtr == NULL){
			qWarning("CGenericParamsSetGuiComp: Parameter '%s' not found or not editable", paramId.constData());
			continue;
		}

		WidgetBinding binding;
		binding.widgetPtr = widgetPtr;
		binding.paramId = paramId;
		binding.paramPtr = paramPtr;
		binding.connection = QMetaObject::Connection();

		// Try to determine binding type and connect signals
		bool bound = false;

		// INameParam ↔ QLineEdit
		iprm::INameParam* nameParamPtr = dynamic_cast<iprm::INameParam*>(paramPtr);
		if (nameParamPtr != NULL){
			QLineEdit* lineEditPtr = qobject_cast<QLineEdit*>(widgetPtr);
			if (lineEditPtr != NULL){
				binding.type = BT_NAME;
				binding.connection = connect(lineEditPtr, SIGNAL(textChanged(QString)), this, SLOT(OnWidgetValueChanged()));
				lineEditPtr->setReadOnly(nameParamPtr->IsNameFixed() || m_isReadOnly);
				bound = true;
			}
		}

		// ISelectionParam ↔ QComboBox
		if (!bound){
			iprm::ISelectionParam* selectionParamPtr = dynamic_cast<iprm::ISelectionParam*>(paramPtr);
			if (selectionParamPtr != NULL){
				QComboBox* comboBoxPtr = qobject_cast<QComboBox*>(widgetPtr);
				if (comboBoxPtr != NULL){
					binding.type = BT_SELECTION;
					PopulateComboBox(comboBoxPtr, selectionParamPtr);
					binding.connection = connect(comboBoxPtr, SIGNAL(currentIndexChanged(int)), this, SLOT(OnWidgetValueChanged()));
					comboBoxPtr->setEnabled(!m_isReadOnly);
					bound = true;
				}
			}
		}

		// IFileNameParam ↔ QLineEdit / QComboBox
		if (!bound){
			ifile::IFileNameParam* fileParamPtr = dynamic_cast<ifile::IFileNameParam*>(paramPtr);
			if (fileParamPtr != NULL){
				QLineEdit* lineEditPtr = qobject_cast<QLineEdit*>(widgetPtr);
				if (lineEditPtr != NULL){
					binding.type = BT_FILENAME;
					binding.connection = connect(lineEditPtr, SIGNAL(textChanged(QString)), this, SLOT(OnWidgetValueChanged()));
					lineEditPtr->setReadOnly(m_isReadOnly);
					bound = true;
				}

				if (!bound){
					QComboBox* comboBoxPtr = qobject_cast<QComboBox*>(widgetPtr);
					if (comboBoxPtr != NULL){
						binding.type = BT_FILENAME;
						comboBoxPtr->setEditable(true);
						binding.connection = connect(comboBoxPtr, SIGNAL(editTextChanged(QString)), this, SLOT(OnWidgetValueChanged()));
						comboBoxPtr->setEnabled(!m_isReadOnly);
						bound = true;
					}
				}
			}
		}

		// IEnableableParam ↔ QCheckBox
		if (!bound){
			iprm::IEnableableParam* enableableParamPtr = dynamic_cast<iprm::IEnableableParam*>(paramPtr);
			if (enableableParamPtr != NULL){
				QCheckBox* checkBoxPtr = qobject_cast<QCheckBox*>(widgetPtr);
				if (checkBoxPtr != NULL){
					binding.type = BT_ENABLEABLE;
					binding.connection = connect(checkBoxPtr, SIGNAL(stateChanged(int)), this, SLOT(OnWidgetValueChanged()));
					checkBoxPtr->setEnabled(enableableParamPtr->IsEnablingAllowed() && !m_isReadOnly);
					bound = true;
				}
			}
		}

		// ITextParam ↔ QLineEdit / QTextEdit / QPlainTextEdit
		if (!bound){
			iprm::ITextParam* textParamPtr = dynamic_cast<iprm::ITextParam*>(paramPtr);
			if (textParamPtr != NULL){
				QLineEdit* lineEditPtr = qobject_cast<QLineEdit*>(widgetPtr);
				if (lineEditPtr != NULL){
					binding.type = BT_TEXT;
					binding.connection = connect(lineEditPtr, SIGNAL(textChanged(QString)), this, SLOT(OnWidgetValueChanged()));
					lineEditPtr->setReadOnly(textParamPtr->IsReadOnly() || m_isReadOnly);
					bound = true;
				}

				if (!bound){
					QTextEdit* textEditPtr = qobject_cast<QTextEdit*>(widgetPtr);
					if (textEditPtr != NULL){
						binding.type = BT_TEXT;
						binding.connection = connect(textEditPtr, SIGNAL(textChanged()), this, SLOT(OnWidgetValueChanged()));
						textEditPtr->setReadOnly(textParamPtr->IsReadOnly() || m_isReadOnly);
						bound = true;
					}
				}

				if (!bound){
					QPlainTextEdit* plainTextEditPtr = qobject_cast<QPlainTextEdit*>(widgetPtr);
					if (plainTextEditPtr != NULL){
						binding.type = BT_TEXT;
						binding.connection = connect(plainTextEditPtr, SIGNAL(textChanged()), this, SLOT(OnWidgetValueChanged()));
						plainTextEditPtr->setReadOnly(textParamPtr->IsReadOnly() || m_isReadOnly);
						bound = true;
					}
				}
			}
		}

		if (bound){
			m_bindings.append(binding);

			// Set initial widget value from parameter
			UpdateWidgetFromParam(m_bindings.last());
		}
		else{
			qWarning("CGenericParamsSetGuiComp: Could not bind parameter '%s' - unsupported parameter/widget combination (widget type: %s)",
				paramId.constData(), widgetPtr->metaObject()->className());
		}
	}
}


void CGenericParamsSetGuiComp::ClearBindings()
{
	for (int i = 0; i < m_bindings.size(); ++i){
		if (m_bindings[i].connection){
			disconnect(m_bindings[i].connection);
		}
	}

	m_bindings.clear();
}


void CGenericParamsSetGuiComp::UpdateWidgetFromParam(WidgetBinding& binding) const
{
	if (binding.widgetPtr == NULL || binding.paramPtr == NULL){
		return;
	}

	// Block signals to avoid recursive updates
	bool wasBlocked = binding.widgetPtr->blockSignals(true);

	switch (binding.type){
		case BT_NAME:{
			iprm::INameParam* nameParamPtr = dynamic_cast<iprm::INameParam*>(binding.paramPtr);
			QLineEdit* lineEditPtr = qobject_cast<QLineEdit*>(binding.widgetPtr);
			if (nameParamPtr != NULL && lineEditPtr != NULL){
				lineEditPtr->setText(nameParamPtr->GetName());
			}
			break;
		}

		case BT_SELECTION:{
			iprm::ISelectionParam* selectionParamPtr = dynamic_cast<iprm::ISelectionParam*>(binding.paramPtr);
			QComboBox* comboBoxPtr = qobject_cast<QComboBox*>(binding.widgetPtr);
			if (selectionParamPtr != NULL && comboBoxPtr != NULL){
				PopulateComboBox(comboBoxPtr, selectionParamPtr);
				int selectedIndex = selectionParamPtr->GetSelectedOptionIndex();
				if (selectedIndex >= 0 && selectedIndex < comboBoxPtr->count()){
					comboBoxPtr->setCurrentIndex(selectedIndex);
				}
			}
			break;
		}

		case BT_FILENAME:{
			ifile::IFileNameParam* fileParamPtr = dynamic_cast<ifile::IFileNameParam*>(binding.paramPtr);
			if (fileParamPtr != NULL){
				QLineEdit* lineEditPtr = qobject_cast<QLineEdit*>(binding.widgetPtr);
				if (lineEditPtr != NULL){
					lineEditPtr->setText(fileParamPtr->GetPath());
				}
				else{
					QComboBox* comboBoxPtr = qobject_cast<QComboBox*>(binding.widgetPtr);
					if (comboBoxPtr != NULL){
						comboBoxPtr->setEditText(fileParamPtr->GetPath());
					}
				}
			}
			break;
		}

		case BT_ENABLEABLE:{
			iprm::IEnableableParam* enableableParamPtr = dynamic_cast<iprm::IEnableableParam*>(binding.paramPtr);
			QCheckBox* checkBoxPtr = qobject_cast<QCheckBox*>(binding.widgetPtr);
			if (enableableParamPtr != NULL && checkBoxPtr != NULL){
				checkBoxPtr->setChecked(enableableParamPtr->IsEnabled());
				checkBoxPtr->setEnabled(enableableParamPtr->IsEnablingAllowed() && !m_isReadOnly);
			}
			break;
		}

		case BT_TEXT:{
			iprm::ITextParam* textParamPtr = dynamic_cast<iprm::ITextParam*>(binding.paramPtr);
			if (textParamPtr != NULL){
				QLineEdit* lineEditPtr = qobject_cast<QLineEdit*>(binding.widgetPtr);
				if (lineEditPtr != NULL){
					lineEditPtr->setText(textParamPtr->GetText());
				}
				else{
					QTextEdit* textEditPtr = qobject_cast<QTextEdit*>(binding.widgetPtr);
					if (textEditPtr != NULL){
						textEditPtr->setPlainText(textParamPtr->GetText());
					}
					else{
						QPlainTextEdit* plainTextEditPtr = qobject_cast<QPlainTextEdit*>(binding.widgetPtr);
						if (plainTextEditPtr != NULL){
							plainTextEditPtr->setPlainText(textParamPtr->GetText());
						}
					}
				}
			}
			break;
		}
	}

	binding.widgetPtr->blockSignals(wasBlocked);
}


void CGenericParamsSetGuiComp::UpdateParamFromWidget(const WidgetBinding& binding) const
{
	if (binding.widgetPtr == NULL || binding.paramPtr == NULL){
		return;
	}

	switch (binding.type){
		case BT_NAME:{
			iprm::INameParam* nameParamPtr = dynamic_cast<iprm::INameParam*>(binding.paramPtr);
			QLineEdit* lineEditPtr = qobject_cast<QLineEdit*>(binding.widgetPtr);
			if (nameParamPtr != NULL && lineEditPtr != NULL){
				if (nameParamPtr->GetName() != lineEditPtr->text()){
					nameParamPtr->SetName(lineEditPtr->text());
				}
			}
			break;
		}

		case BT_SELECTION:{
			iprm::ISelectionParam* selectionParamPtr = dynamic_cast<iprm::ISelectionParam*>(binding.paramPtr);
			QComboBox* comboBoxPtr = qobject_cast<QComboBox*>(binding.widgetPtr);
			if (selectionParamPtr != NULL && comboBoxPtr != NULL){
				int currentIndex = comboBoxPtr->currentIndex();
				if (selectionParamPtr->GetSelectedOptionIndex() != currentIndex){
					selectionParamPtr->SetSelectedOptionIndex(currentIndex);
				}
			}
			break;
		}

		case BT_FILENAME:{
			ifile::IFileNameParam* fileParamPtr = dynamic_cast<ifile::IFileNameParam*>(binding.paramPtr);
			if (fileParamPtr != NULL){
				QString newPath;
				QLineEdit* lineEditPtr = qobject_cast<QLineEdit*>(binding.widgetPtr);
				if (lineEditPtr != NULL){
					newPath = lineEditPtr->text();
				}
				else{
					QComboBox* comboBoxPtr = qobject_cast<QComboBox*>(binding.widgetPtr);
					if (comboBoxPtr != NULL){
						newPath = comboBoxPtr->currentText();
					}
				}

				if (fileParamPtr->GetPath() != newPath){
					fileParamPtr->SetPath(newPath);
				}
			}
			break;
		}

		case BT_ENABLEABLE:{
			iprm::IEnableableParam* enableableParamPtr = dynamic_cast<iprm::IEnableableParam*>(binding.paramPtr);
			QCheckBox* checkBoxPtr = qobject_cast<QCheckBox*>(binding.widgetPtr);
			if (enableableParamPtr != NULL && checkBoxPtr != NULL){
				bool isEnabled = checkBoxPtr->isChecked();
				if (enableableParamPtr->IsEnabled() != isEnabled){
					enableableParamPtr->SetEnabled(isEnabled);
				}
			}
			break;
		}

		case BT_TEXT:{
			iprm::ITextParam* textParamPtr = dynamic_cast<iprm::ITextParam*>(binding.paramPtr);
			if (textParamPtr != NULL){
				QString newText;
				QLineEdit* lineEditPtr = qobject_cast<QLineEdit*>(binding.widgetPtr);
				if (lineEditPtr != NULL){
					newText = lineEditPtr->text();
				}
				else{
					QTextEdit* textEditPtr = qobject_cast<QTextEdit*>(binding.widgetPtr);
					if (textEditPtr != NULL){
						newText = textEditPtr->toPlainText();
					}
					else{
						QPlainTextEdit* plainTextEditPtr = qobject_cast<QPlainTextEdit*>(binding.widgetPtr);
						if (plainTextEditPtr != NULL){
							newText = plainTextEditPtr->toPlainText();
						}
					}
				}

				if (textParamPtr->GetText() != newText){
					textParamPtr->SetText(newText);
				}
			}
			break;
		}
	}
}


void CGenericParamsSetGuiComp::PopulateComboBox(QComboBox* comboBoxPtr, const iprm::ISelectionParam* paramPtr) const
{
	Q_ASSERT(comboBoxPtr != NULL);
	Q_ASSERT(paramPtr != NULL);

	const iprm::IOptionsList* optionsPtr = paramPtr->GetSelectionConstraints();
	if (optionsPtr == NULL){
		return;
	}

	bool wasBlocked = comboBoxPtr->blockSignals(true);

	int currentCount = comboBoxPtr->count();
	int optionsCount = optionsPtr->GetOptionsCount();

	if (currentCount != optionsCount){
		comboBoxPtr->clear();

		for (int i = 0; i < optionsCount; ++i){
			comboBoxPtr->addItem(optionsPtr->GetOptionName(i));
		}
	}
	else{
		for (int i = 0; i < optionsCount; ++i){
			QString optionName = optionsPtr->GetOptionName(i);
			if (comboBoxPtr->itemText(i) != optionName){
				comboBoxPtr->setItemText(i, optionName);
			}
		}
	}

	int selectedIndex = paramPtr->GetSelectedOptionIndex();
	if (selectedIndex >= 0 && selectedIndex < comboBoxPtr->count()){
		comboBoxPtr->setCurrentIndex(selectedIndex);
	}

	comboBoxPtr->blockSignals(wasBlocked);
}


} // namespace iqtprm


