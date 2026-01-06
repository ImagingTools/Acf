#ifndef istd_IChangeDelegator_included
#define istd_IChangeDelegator_included


// ACF includes
#include <istd/IChangeable.h>


namespace istd
{


/**
	Common interface for all classes that support delegation of their data updates to
	another class. This is useful for composed objects to delegate information about data changes
	from a child element to its owner, enabling hierarchical change notification patterns.

	\section ChangeDelegatorPurpose Purpose
	The IChangeDelegator interface extends IChangeable to support automatic forwarding of change
	notifications from child objects to their parent or owner objects. This is essential in
	composite data structures where changes to child elements need to be propagated up the
	hierarchy.

	\section ChangeDelegatorUsage Usage
	When a class implements IChangeDelegator, it can forward its change notifications to a
	"slave" object (typically a parent or owner). When the delegator's BeginChanges() or
	EndChanges() methods are called, they will automatically propagate to the slave object.

	\section ChangeDelegatorExample Example
	\code
	// Child class that delegates changes to parent
	class CDataItem: public istd::TChangeDelegator<istd::IChangeable>
	{
	public:
		void SetValue(int value)
		{
			if (m_value != value) {
				static ChangeSet changeSet(CF_VALUE_CHANGED, "Value changed");
				BeginChanges(changeSet);
				m_value = value;
				EndChanges(changeSet);  // This will notify the slave (parent)
			}
		}

	private:
		int m_value;
	};

	// Parent class that owns child items
	class CDataContainer: virtual public istd::IChangeable
	{
	public:
		void AddItem(CDataItem* item)
		{
			m_items.append(item);
			// Register this container as the slave for change delegation
			item->SetSlavePtr(this);
		}

	protected:
		// This method will be called when any child item changes
		virtual void OnEndChanges(const ChangeSet& changeSet)
		{
			if (changeSet.Contains(CF_DELEGATED)) {
				// Handle child item changes
				UpdateDisplay();
			}
		}

	private:
		QList<CDataItem*> m_items;
	};
	\endcode

	\sa istd::IChangeable, istd::TChangeDelegator, istd::CChangeDelegator

	\ingroup DataModel
 */
class IChangeDelegator: virtual public istd::IChangeable
{
public:
	/**
		Gets the slave object pointer.
		
		The slave object will receive change notifications when this object's state changes.
		If no slave is set, returns nullptr.
		
		\return Pointer to the slave IChangeable object, or nullptr if not set.
	*/
	virtual istd::IChangeable* GetSlavePtr() const = 0;

	/**
		Sets the slave object pointer for change delegation.
		
		When a slave is set, all change notifications (BeginChanges and EndChanges) from
		this object will be automatically forwarded to the slave. This enables hierarchical
		change notification where child objects notify their parents of changes.
		
		\param slavePtr Pointer to the IChangeable object that should receive delegated
		               change notifications. Pass nullptr to disable delegation.
		
		\note The slave object is not owned by this delegator. The caller is responsible
		      for ensuring the slave object remains valid during the delegation lifetime.
		
		\sa GetSlavePtr()
	*/
	virtual void SetSlavePtr(istd::IChangeable* slavePtr) = 0;
};


} // namespace istd


#endif // !istd_IChangeDelegator_included


