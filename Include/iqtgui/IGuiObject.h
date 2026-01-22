// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include <istd/IPolymorphic.h>
#include <iqtgui/iqtgui.h>


namespace iqtgui
{


/**
	Common interface for GUI objects used in ACF component context.
	
	\section GuiObjectPurpose Purpose
	IGuiObject provides a standardized interface for managing the lifecycle of GUI components
	within the ACF framework. It handles initialization, parent-child relationships, and
	cleanup of Qt-based GUI elements in a component-oriented architecture.

	\section GuiObjectLifecycle Lifecycle Management
	The typical lifecycle of a GUI object follows these stages:
	1. Creation: Object is constructed but GUI is not yet created
	2. Initialization: CreateGui() is called to create and initialize Qt widgets
	3. Active: GUI is created and operational (IsGuiCreated() returns true)
	4. Cleanup: DestroyGui() is called to release GUI resources
	5. Destruction: Object is destroyed

	\section GuiObjectUsage Usage Pattern
	\code
	// Example GUI component implementation
	class CMyGuiComponent: public iqtgui::IGuiObject
	{
	public:
		CMyGuiComponent() : m_widget(nullptr) {}

		virtual bool IsGuiCreated() const override
		{
			return m_widget != nullptr;
		}

		virtual bool CreateGui(QWidget* parentPtr) override
		{
			if (IsGuiCreated()) {
				return false;  // Already created
			}

			m_widget = new QWidget(parentPtr);
			m_layout = new QVBoxLayout(m_widget);
			m_button = new QPushButton("Click Me", m_widget);
			m_layout->addWidget(m_button);

			// Connect signals, configure widget properties, etc.
			connect(m_button, &QPushButton::clicked, this, &CMyGuiComponent::onButtonClicked);

			return true;
		}

		virtual bool DestroyGui() override
		{
			if (!IsGuiCreated()) {
				return false;  // Not created
			}

			// Cleanup: Qt will delete child widgets automatically
			delete m_widget;
			m_widget = nullptr;
			m_layout = nullptr;
			m_button = nullptr;

			return true;
		}

		virtual QWidget* GetWidget() const override
		{
			return m_widget;
		}

		virtual void OnTryClose(bool* ignoredPtr) override
		{
			// Check if there are unsaved changes
			if (HasUnsavedChanges()) {
				if (ignoredPtr) {
					*ignoredPtr = true;  // Prevent closing
					ShowSaveDialog();
				}
			}
		}

	private:
		QWidget* m_widget;
		QVBoxLayout* m_layout;
		QPushButton* m_button;

		bool HasUnsavedChanges() const { return false; }  // Example
		void ShowSaveDialog() {}  // Example
		void onButtonClicked() {}  // Example
	};

	// Usage
	CMyGuiComponent component;
	QWidget* mainWindow = GetMainWindow();

	// Create the GUI
	if (component.CreateGui(mainWindow)) {
		// GUI created successfully
		QWidget* widget = component.GetWidget();
		widget->show();
	}

	// Later, when closing
	bool ignored = false;
	component.OnTryClose(&ignored);
	if (!ignored) {
		component.DestroyGui();
	}
	\endcode

	\sa iqtgui::CGuiComponentBase, iqtgui::TGuiComponentBase

	\ingroup BasicUi
 */
class IGuiObject: virtual public istd::IPolymorphic
{
public:
	/**
		Checks if the GUI has been initialized and created.
		
		This method returns true after CreateGui() has been successfully called and
		before DestroyGui() is called. It can be used to verify the GUI state before
		performing operations that require an active GUI.
		
		\return true if the GUI is created and initialized, false otherwise.
		
		\sa CreateGui(), DestroyGui()
	 */
	virtual bool IsGuiCreated() const = 0;
	
	/**
		Initializes the GUI and connects it to the parent widget.
		
		This method creates and initializes all Qt widgets that form the GUI component.
		It should only be called once; subsequent calls should be ignored or return false.
		The created widgets become children of the specified parent widget.
		
		\param parentPtr Pointer to the parent QWidget. The created GUI will be a child
		                of this widget. Can be nullptr for top-level widgets.
		
		\return true if initialization succeeded, false if it failed or was already created.
		
		\note After successful creation, IsGuiCreated() should return true.
		\note This method should be idempotent - calling it multiple times should not
		      create multiple GUI instances.
		
		\sa DestroyGui(), IsGuiCreated(), GetWidget()
	 */
	virtual bool CreateGui(QWidget* parentPtr) = 0;
	
	/**
		Releases GUI resources and disconnects from the parent.
		
		This method destroys all Qt widgets created by CreateGui() and cleans up
		associated resources. After calling this method, IsGuiCreated() should
		return false, and GetWidget() should return nullptr or an invalid pointer.
		
		\return true if the operation succeeded, false if it failed or GUI was not created.
		
		\note It's safe to call this method even if the GUI was not created; it should
		      simply return false in that case.
		\note After calling this method, CreateGui() can be called again to recreate
		      the GUI if needed.
		
		\sa CreateGui(), IsGuiCreated()
	 */
	virtual bool DestroyGui() = 0;
	
	/**
		Gets access to the internal QWidget object.
		
		Returns a pointer to the main QWidget that represents this GUI component.
		This widget can be used for layout management, showing/hiding, or other
		Qt-specific operations.
		
		\return Pointer to the internal QWidget, or nullptr if GUI is not created.
		
		\note The returned pointer is owned by this IGuiObject instance. The caller
		      should not delete it.
		\note The pointer becomes invalid after DestroyGui() is called.
		
		\sa CreateGui(), IsGuiCreated()
	*/
	virtual QWidget* GetWidget() const = 0;
	
	/**
		Called when the application is attempting to close.
		
		This method is invoked before the application closes, giving the GUI component
		a chance to prevent the close operation (e.g., to save unsaved data, show
		confirmation dialogs, or perform cleanup operations).
		
		\param ignoredPtr Optional pointer to a boolean flag. If not nullptr and the
		                 component wants to prevent closing (e.g., user cancels a save
		                 dialog), the implementation should set *ignoredPtr to true.
		                 If nullptr, the close operation cannot be prevented.
		
		\note If ignoredPtr is nullptr, the implementation must not try to prevent
		      closing and should only perform necessary cleanup.
		\note This method may be called multiple times if the close is initially
		      prevented but later attempted again.
		
		\code
		// Example implementation
		virtual void OnTryClose(bool* ignoredPtr)
		{
			if (m_hasUnsavedData) {
				if (ignoredPtr) {
					// Show save dialog
					int result = QMessageBox::question(GetWidget(), 
						"Unsaved Changes",
						"Do you want to save your changes?",
						QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
					
					if (result == QMessageBox::Save) {
						SaveData();
					} else if (result == QMessageBox::Cancel) {
						*ignoredPtr = true;  // Prevent closing
						return;
					}
				}
			}
			// Perform cleanup if needed
		}
		\endcode
		
		\sa DestroyGui()
	 */
	virtual void OnTryClose(bool* ignoredPtr = NULL) = 0;
};


typedef istd::TUniqueInterfacePtr<IGuiObject> IGuiObjectUniquePtr;
typedef istd::TSharedInterfacePtr<IGuiObject> IGuiObjectSharedPtr;


} // namespace iqtgui




