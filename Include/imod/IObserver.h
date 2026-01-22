// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/IChangeable.h>
#include <imod/imod.h>


namespace imod
{


class IModel;


/**
	Common interface for all classes implementing the Observer functionality
	in the Model/Observer design pattern.

	\section ObserverPurpose Purpose
	The IObserver interface is a key component of the Model/Observer pattern (also known
	as Model-View-Controller or MVC). It defines the contract for objects that need to
	be notified when an observed model's state changes. This enables loose coupling between
	data models and their visual representations or other dependent components.

	\section ObserverPattern The Observer Pattern
	In the Observer pattern:
	- **Model (Observable)**: Contains the data and business logic, implements IModel
	- **Observer**: Watches the model for changes, implements IObserver
	- **Notification Flow**: When model data changes, all attached observers are notified
	- **Decoupling**: Models don't need to know specific details about their observers

	\section ObserverLifecycle Observer Lifecycle
	1. **Attachment**: Observer is attached to a model via OnModelAttached()
	2. **Filtering**: Observer specifies which changes it's interested in (changeMask)
	3. **Updates**: Observer receives BeforeUpdate() and AfterUpdate() notifications
	4. **Detachment**: Observer is detached from model via OnModelDetached()

	\section ObserverExample Example Implementation
	\code
	// Example: GUI component observing a data model
	class CPersonViewObserver: public imod::IObserver
	{
	public:
		CPersonViewObserver(QLabel* nameLabel, QLabel* ageLabel)
			: m_nameLabel(nameLabel), m_ageLabel(ageLabel) {}

		// Check if a specific model is attached
		virtual bool IsModelAttached(const IModel* modelPtr) const override
		{
			return m_attachedModel == modelPtr || 
			       (modelPtr == nullptr && m_attachedModel != nullptr);
		}

		// Called when model is being attached
		virtual bool OnModelAttached(IModel* modelPtr, 
		                            istd::IChangeable::ChangeSet& changeMask) override
		{
			// Verify the model is of expected type
			CPerson* person = dynamic_cast<CPerson*>(modelPtr);
			if (!person) {
				return false;  // Reject incompatible models
			}

			m_attachedModel = modelPtr;
			
			// Specify which changes we're interested in
			changeMask.Set(CPerson::CF_NAME_CHANGED);
			changeMask.Set(CPerson::CF_AGE_CHANGED);
			
			// Initial update of display
			UpdateDisplay(person);
			return true;
		}

		// Called when model is being detached
		virtual bool OnModelDetached(IModel* modelPtr) override
		{
			if (m_attachedModel == modelPtr) {
				m_attachedModel = nullptr;
				// Clear display
				m_nameLabel->clear();
				m_ageLabel->clear();
				return true;
			}
			return false;
		}

		// Called before model update starts
		virtual void BeforeUpdate(IModel* modelPtr) override
		{
			// Optional: Save current state for comparison
			// or prepare for update (e.g., block signals)
		}

		// Called after model has been updated
		virtual void AfterUpdate(IModel* modelPtr, 
		                        const istd::IChangeable::ChangeSet& changeSet) override
		{
			CPerson* person = dynamic_cast<CPerson*>(modelPtr);
			if (!person) return;

			// Check which changes occurred and update accordingly
			if (changeSet.Contains(CPerson::CF_NAME_CHANGED)) {
				m_nameLabel->setText(person->GetName());
			}
			if (changeSet.Contains(CPerson::CF_AGE_CHANGED)) {
				m_ageLabel->setText(QString::number(person->GetAge()));
			}
		}

	private:
		void UpdateDisplay(CPerson* person)
		{
			m_nameLabel->setText(person->GetName());
			m_ageLabel->setText(QString::number(person->GetAge()));
		}

		IModel* m_attachedModel = nullptr;
		QLabel* m_nameLabel;
		QLabel* m_ageLabel;
	};

	// Usage
	CPerson personModel("John", 30);
	QLabel nameLabel, ageLabel;
	CPersonViewObserver observer(&nameLabel, &ageLabel);

	// Attach observer to model
	personModel.AttachObserver(&observer);

	// When model changes, observer is automatically notified
	personModel.SetName("Jane");  // nameLabel is updated automatically
	personModel.SetAge(25);       // ageLabel is updated automatically

	// Detach when done
	personModel.DetachObserver(&observer);
	\endcode

	\section ObserverBestPractices Best Practices
	- Always verify model type in OnModelAttached() before accepting
	- Specify precise change masks to avoid unnecessary updates
	- Keep update logic in AfterUpdate() fast to avoid UI freezing
	- Handle nullptr in IsModelAttached() to check for any attached model
	- Clean up resources in OnModelDetached()
	- BeforeUpdate() may be called without subsequent AfterUpdate() in some error cases

	\sa imod::IModel, istd::IChangeable, imod::CModelObserverComp

	\ingroup DataModel
	\ingroup ModelObserver
*/
class IObserver: virtual public istd::IPolymorphic
{
public:
	/**
		Checks if the specified model is currently attached to this observer.
		
		This method allows querying whether a specific model is being observed,
		or whether any model at all is attached.
		
		\param modelPtr Pointer to the model object to check. If nullptr, checks whether
		               any model is attached to this observer.
		
		\return true if the specified model (or any model when modelPtr is nullptr) 
		        is attached to this observer, false otherwise.
		
		\code
		// Check if a specific model is attached
		if (observer->IsModelAttached(myModel)) {
			// This specific model is being observed
		}
		
		// Check if any model is attached
		if (observer->IsModelAttached(nullptr)) {
			// At least one model is attached
		}
		\endcode
		
		\sa OnModelAttached(), OnModelDetached()
	*/
	virtual bool IsModelAttached(const IModel* modelPtr = NULL) const = 0;
	
	/**
		Callback invoked when an observable model is about to be attached to this observer.
		
		This method is called by the model when attempting to establish an observer
		relationship. The implementation should:
		1. Validate that the model is of an acceptable type
		2. Set the changeMask to specify which changes to monitor
		3. Return true to accept the attachment, or false to reject it
		
		\param modelPtr Pointer to the model object being attached. Never nullptr.
		
		\param changeMask Output parameter where the observer specifies which change
		                 types it wants to be notified about. Set the appropriate
		                 change flags using changeMask.Set(changeId). An empty mask
		                 means the observer wants all changes.
		
		\return true if the model is accepted and attachment succeeds, false to reject
		        the attachment (e.g., if the model is of incompatible type).
		
		\note After successful attachment, the observer should initialize its state
		      based on the current model data.
		\note The changeMask allows filtering notifications for better performance.
		      Only changes matching the mask will trigger AfterUpdate() calls.
		
		\code
		virtual bool OnModelAttached(IModel* modelPtr, 
		                            istd::IChangeable::ChangeSet& changeMask)
		{
			// Type check
			CMyModel* model = dynamic_cast<CMyModel*>(modelPtr);
			if (!model) return false;
			
			// Set up change filtering
			changeMask.Set(CMyModel::CF_DATA_CHANGED);
			changeMask.Set(CMyModel::CF_STATUS_CHANGED);
			// Don't set CF_INTERNAL_CHANGED - we don't care about those
			
			// Initialize view from current model state
			UpdateViewFromModel(model);
			return true;
		}
		\endcode
		
		\sa OnModelDetached(), IsModelAttached(), AfterUpdate()
	*/
	virtual bool OnModelAttached(IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) = 0;

	/**
		Callback invoked when an observable model is about to be detached from this observer.
		
		This method is called when the observer-model relationship is being terminated,
		either explicitly or because the model is being destroyed. The implementation
		should clean up any state or resources related to this model.
		
		\param modelPtr Pointer to the model object being detached
		
		\return true if the detachment was handled successfully, false otherwise
		
		\note After this call, the observer should not access the model pointer anymore.
		\note This method may be called even if OnModelAttached() previously returned false.
		\note Clean up any cached data or references to this model.
		
		\code
		virtual bool OnModelDetached(IModel* modelPtr)
		{
			if (m_currentModel == modelPtr) {
				// Clear any cached data
				m_cachedData.clear();
				m_currentModel = nullptr;
				
				// Update UI to show no model is attached
				ClearDisplay();
				return true;
			}
			return false;
		}
		\endcode
		
		\sa OnModelAttached(), IsModelAttached()
	*/
	virtual bool OnModelDetached(IModel* modelPtr) = 0;

	/**
		Callback invoked before an update of the observer's content occurs.
		
		This method is called at the start of a model update cycle, before any data
		actually changes. It allows the observer to prepare for the update, such as
		saving current state for comparison or disabling UI updates temporarily.
		
		\param modelPtr Pointer to the model that is about to change
		
		\warning In some error cases, this method may be called without a subsequent
		         AfterUpdate() call. Don't rely on AfterUpdate() always being called.
		\note This method is optional to implement - if you don't need preparation
		      logic, just provide an empty implementation.
		
		\code
		virtual void BeforeUpdate(IModel* modelPtr)
		{
			// Example: Block signals to prevent multiple GUI updates
			if (m_widget) {
				m_widget->blockSignals(true);
			}
			
			// Example: Save state for delta comparison
			m_previousValue = m_currentValue;
		}
		\endcode
		
		\sa AfterUpdate()
	*/
	virtual void BeforeUpdate(IModel* modelPtr) = 0;

	/**
		Callback invoked after an update of the observer's content occurs.
		
		This is the primary notification method where observers react to model changes.
		It's called after the model has finished updating its state. The implementation
		should examine the changeSet to determine what changed and update accordingly.
		
		\param modelPtr Pointer to the model that has changed
		
		\param changeSet Contains information about what changed in the model. Use
		                changeSet.Contains(changeId) to check for specific changes.
		                The changeSet respects the mask specified in OnModelAttached().
		
		\note This is typically where you update your GUI, recalculate derived values,
		      or trigger other dependent operations.
		\note Keep this method fast - it's called frequently and may block the UI thread.
		\note Use the changeSet to perform incremental updates rather than refreshing
		      everything.
		
		\code
		virtual void AfterUpdate(IModel* modelPtr, 
		                        const istd::IChangeable::ChangeSet& changeSet)
		{
			CMyModel* model = dynamic_cast<CMyModel*>(modelPtr);
			if (!model) return;
			
			// Selective update based on what changed
			if (changeSet.Contains(CMyModel::CF_NAME_CHANGED)) {
				m_nameLabel->setText(model->GetName());
			}
			if (changeSet.Contains(CMyModel::CF_VALUE_CHANGED)) {
				m_valueSpinBox->setValue(model->GetValue());
				RecalculateDependentValues();
			}
			
			// Re-enable signals if blocked in BeforeUpdate
			if (m_widget) {
				m_widget->blockSignals(false);
			}
		}
		\endcode
		
		\sa BeforeUpdate(), OnModelAttached()
	*/
	virtual void AfterUpdate(IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) = 0;
};


typedef istd::TUniqueInterfacePtr<imod::IObserver> IObserverPtr;


} // namespace imod


