// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QVector>
#include <QtCore/QByteArray>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <ibase/TModelObserverCompWrap.h>
#include <iqtgui/TGuiObserverWrap.h>
#include <iqtgui/TGuiComponentBase.h>
#include <iprm/IParamsSet.h>


namespace iqtprm
{


/**
	\brief Generic parameter set GUI component that loads a Qt Designer .ui file
	at runtime and automatically binds UI widgets to parameters.

	Unlike CComposedParamsSetGuiComp which uses statically defined editor components,
	this component loads a .ui file via QUiLoader and discovers parameter bindings
	through a dynamic Qt property \c ACF_PARAM_ID set on each widget in Qt Designer.

	Supported parameter type to widget mappings:
	- iprm::INameParam       ↔ QLineEdit
	- iprm::ISelectionParam  ↔ QComboBox
	- iprm::IEnableableParam ↔ QCheckBox
	- iprm::ITextParam       ↔ QLineEdit, QTextEdit, QPlainTextEdit
	- ifile::IFileNameParam  ↔ QLineEdit, QComboBox

	\section cgenericparamssetguicomp_usage Usage

	1. Create a .ui file in Qt Designer.
	2. For each widget that should be bound to a parameter, add a Dynamic Property
	   named \c ACF_PARAM_ID with a string value matching the parameter ID in the
	   observed IParamsSet.
	3. Configure the component with the \c UiFilePath attribute pointing to the .ui file.

	\see CComposedParamsSetGuiComp, iprm::IParamsSet
*/
class CGenericParamsSetGuiComp:
			public ibase::TModelObserverCompBaseWrap<
						iqtgui::TGuiObserverWrap<
							iqtgui::TGuiComponentBase<QWidget>,
							imod::TSingleModelObserverBase<iprm::IParamsSet>>>
{
	Q_OBJECT

public:
	typedef ibase::TModelObserverCompBaseWrap<
				iqtgui::TGuiObserverWrap<
					iqtgui::TGuiComponentBase<QWidget>,
					imod::TSingleModelObserverBase<iprm::IParamsSet>>> BaseClass;

	I_BEGIN_COMPONENT(CGenericParamsSetGuiComp);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_ASSIGN(m_uiFilePathAttrPtr, "UiFilePath", "Path to the Qt Designer .ui file to load at runtime", true, "");
		I_ASSIGN(m_isReadOnlyAttrPtr, "EditorReadOnly", "If enabled, the editor for the data model is always read only", false, false);
	I_END_COMPONENT;

	CGenericParamsSetGuiComp();

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual QWidget* CreateQtWidget(QWidget* parentPtr) override;
	virtual void OnGuiCreated() override;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached() override;
	virtual void OnGuiModelDetached() override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual void UpdateModel() const override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

	// reimplemented (imod::IModelEditor)
	virtual void SetReadOnly(bool state) override;

private Q_SLOTS:
	void OnWidgetValueChanged();

private:
	enum BindingType
	{
		BT_NAME,
		BT_SELECTION,
		BT_FILENAME,
		BT_ENABLEABLE,
		BT_TEXT
	};

	struct WidgetBinding
	{
		QWidget* widgetPtr;
		QByteArray paramId;
		iser::ISerializable* paramPtr;
		BindingType type;
		QMetaObject::Connection connection;
	};

	void CreateBindings();
	void ClearBindings();
	void UpdateWidgetFromParam(WidgetBinding& binding) const;
	void UpdateParamFromWidget(const WidgetBinding& binding) const;
	void PopulateComboBox(QComboBox* comboBoxPtr, const iprm::ISelectionParam* paramPtr) const;

	I_ATTR(QString, m_uiFilePathAttrPtr);
	I_ATTR(bool, m_isReadOnlyAttrPtr);

	QWidget* m_loadedUiWidgetPtr;
	QVector<WidgetBinding> m_bindings;
};


} // namespace iqtprm


