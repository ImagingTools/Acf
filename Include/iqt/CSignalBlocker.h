// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <iqt/iqt.h>


namespace iqt
{


/**
	RAII helper class for temporarily blocking Qt signals from QObjects.
	
	\section SignalBlockerPurpose Purpose
	CSignalBlocker provides an exception-safe way to temporarily block Qt signal
	emissions from one or more QObjects. Signals are automatically blocked in the
	constructor and restored to their previous state in the destructor, following
	the RAII (Resource Acquisition Is Initialization) pattern.

	\section SignalBlockerUseCases Use Cases
	Signal blocking is useful when you need to:
	- Update multiple related widgets without triggering intermediate signal chains
	- Prevent recursive signal-slot connections during updates
	- Temporarily disable UI notifications during batch operations
	- Avoid performance issues from excessive signal emissions

	\section SignalBlockerExample Usage Examples
	\code
	// Example 1: Block signals during widget updates
	void UpdateSliderAndSpinBox(int value)
	{
		// Block signals to prevent circular updates
		iqt::CSignalBlocker blocker1(m_slider);
		iqt::CSignalBlocker blocker2(m_spinBox);
		
		m_slider->setValue(value);
		m_spinBox->setValue(value);
		
		// Signals automatically unblocked when blockers go out of scope
	}

	// Example 2: Block parent and all children
	void ResetForm()
	{
		// Block signals for entire form, including all child widgets
		iqt::CSignalBlocker blocker(m_formWidget, true);  // true = block children
		
		// Reset all form fields without triggering valueChanged signals
		m_nameEdit->clear();
		m_ageSpinBox->setValue(0);
		m_addressEdit->clear();
		// ... more fields
		
		// All signals restored when blocker is destroyed
	}

	// Example 3: Scope-based blocking
	void LoadConfiguration(const Config& config)
	{
		{
			// Inner scope for signal blocking
			iqt::CSignalBlocker blocker(m_settingsPanel, true);
			
			// Load all settings without emitting signals
			for (const auto& setting : config.settings) {
				ApplySetting(setting);  // Would normally emit signals
			}
		}  // Blocker destroyed here, signals restored
		
		// Now emit a single "configuration changed" signal
		emit configurationLoaded();
	}

	// Example 4: Conditional blocking
	void SetValue(int value, bool silent)
	{
		std::unique_ptr<iqt::CSignalBlocker> blocker;
		
		if (silent) {
			// Only block if silent mode is requested
			blocker.reset(new iqt::CSignalBlocker(m_widget));
		}
		
		m_widget->setValue(value);
		
		// If blocker exists, it's destroyed here and signals are restored
	}

	// Example 5: Nested widgets
	void UpdateComplexWidget()
	{
		// Block entire widget tree during update
		iqt::CSignalBlocker blocker(m_containerWidget, true);
		
		// Update parent
		m_containerWidget->setEnabled(true);
		
		// Update children (signals still blocked)
		m_childWidget1->setText("Updated");
		m_childWidget2->setChecked(true);
		
		// All widgets restored when blocker goes out of scope
	}
	\endcode

	\section SignalBlockerNotes Important Notes
	- Signal blocking is **not** recursive by default. Use blockChilds=true to block
	  all child objects as well.
	- The blocker restores the **previous** blocking state, not necessarily unblocking.
	  If signals were already blocked, they remain blocked after the blocker is destroyed.
	- Blocking signals does **not** prevent the object from receiving events or updating
	  its internal state - it only prevents signal emission.
	- The object pointer is **not** owned by CSignalBlocker. Ensure the object remains
	  valid for the lifetime of the blocker.

	\section SignalBlockerPerformance Performance Considerations
	Signal blocking is very lightweight and has minimal overhead. It's safe to use
	in performance-critical code paths. However, remember that blocked signals are
	simply not emitted - they are not queued or delayed.

	\sa QObject::blockSignals(), QSignalBlocker

	\ingroup BasicUi
*/
class CSignalBlocker
{
public:

	/**
		Constructs a signal blocker and immediately blocks signals on the specified object.
		
		The constructor saves the current signal blocking state of the object and then
		blocks its signals. The previous state will be restored in the destructor.
		
		\param objectPtr Pointer to the QObject whose signals should be blocked.
		                If nullptr, the blocker does nothing (no-op).
		
		\param blockChilds If true, recursively blocks signals on all child objects
		                  as well. If false (default), only the specified object is affected.
		
		\note If the object already has signals blocked, this blocker will maintain
		      that state when destroyed.
		\note The objectPtr is not owned by this class. Ensure it remains valid for
		      the lifetime of the CSignalBlocker instance.
		
		\code
		// Block just the slider
		iqt::CSignalBlocker blocker(mySlider);
		
		// Block a form and all its child widgets
		iqt::CSignalBlocker blocker(myForm, true);
		
		// Safe with nullptr
		iqt::CSignalBlocker blocker(nullptr);  // Does nothing
		\endcode
		
		\sa ~CSignalBlocker()
	*/
	CSignalBlocker(QObject* objectPtr, bool blockChilds = false);

	/**
		Destroys the signal blocker and restores the previous signal blocking state.
		
		The destructor automatically restores the signal blocking state that was
		active when the blocker was constructed. This ensures proper cleanup even
		in the presence of exceptions.
		
		\note If blockChilds was true in the constructor, child objects' signals
		      are also restored.
		\note This follows the RAII pattern - you don't need to manually unblock signals.
		
		\sa CSignalBlocker()
	*/
	virtual ~CSignalBlocker();

private:
	/**
		Block/unblock all children.
		
		\param [in,out]	objectPtr	If non-null, pointer to the object to be blocked/unblocked.
		\param	block			 	true to block, false to unblock.
	*/
	void BlockChilds(QObject* objectPtr, bool block);

private:
	QObject* m_objectPtr;
	bool m_blockChilds;
};


} // namespace iqt



