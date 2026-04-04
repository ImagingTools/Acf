// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QMap>
#include <QtCore/QMutex>

// ACF includes
#include <imod/imod.h>
#include <imod/IModel.h>
#include <imod/IObserver.h>


namespace imod
{		


/**
	Basic implementation of a model.

	\ingroup ModelObserver
*/
class CModelBase: virtual public imod::IModel
{
public:
	typedef QSet<IObserver*> Observers;

	CModelBase();
	virtual ~CModelBase();

	/**
		Returns count of connected observers.
	*/
	virtual int GetObserverCount() const;

	/**
		Returns set of all observers.
	*/
	virtual Observers GetObservers() const;

	istd::IChangeable::ChangeSet GetCumulatedChanges() const;

	// reimplemented (imod::IModel)
	virtual bool AttachObserver(imod::IObserver* observerPtr) override;
	virtual void DetachObserver(imod::IObserver* observerPtr) override;
	virtual void DetachAllObservers() override;
	virtual bool IsAttached(const imod::IObserver* observerPtr) const override;

protected:
	/**
		Called before each change.
		\return	true, if some notification was done.
	*/
	void NotifyBeforeChange(const istd::IChangeable::ChangeSet& changeSet, bool isGroup);
	/**
		Called after each change.
		\return	true, if end notification was done.
	*/
	void NotifyAfterChange(const istd::IChangeable::ChangeSet& changeSet);

	/**
		Called before global changes will be started.
	*/
	virtual void OnBeginGlobalChanges() = 0;
	/**
		Called after global changes are finished.
	*/
	virtual void OnEndGlobalChanges(const istd::IChangeable::ChangeSet& changeSet) = 0;

private:
	Q_DISABLE_COPY(CModelBase);

	/**
		Non-virtual implementation of DetachAllObservers.
		Must be called with m_mutex already held.
	*/
	void DoDetachAllObservers();

	void CleanupObserverState();

	/**
		\brief RAII scope guard for m_blockCounter management.

		Increments m_blockCounter on construction and decrements on destruction,
		ensuring the counter is always properly balanced even when exceptions occur
		during observer notifications or circular model dependency chains
		(e.g. via CModelUpdateBridge A->B->A).

		Call Release() on the normal (non-exception) path to transfer counter
		ownership to the matching NotifyAfterChange call.

		\sa m_blockCounter, NotifyBeforeChange, NotifyAfterChange
	*/
	class CChangeScope
	{
		Q_DISABLE_COPY(CChangeScope);
	public:
		explicit CChangeScope(CModelBase& model) : m_modelPtr(&model) { ++m_modelPtr->m_blockCounter; }
		~CChangeScope() { if (m_modelPtr != nullptr) { --m_modelPtr->m_blockCounter; } }

		/**
			Release ownership of the counter increment.
			After calling this, the destructor will NOT decrement m_blockCounter.
			Used on the success path where the decrement will be handled by NotifyAfterChange.
		*/
		void Release() { m_modelPtr = nullptr; }
	private:
		CModelBase* m_modelPtr;
	};

private:
	/**
		Observer connection state.
	*/
	enum AttachingState
	{
		/**
			Observer is being to attached.
		*/
		AS_ATTACHING,

		/**
			Observer is connected to the model.
		*/
		AS_ATTACHED,
		/**
			Observer is connected to the model and it is during the update.
		*/
		AS_ATTACHED_UPDATING,
		/**
			Observer is in detaching stage.
		*/
		AS_DETACHING,

		/**
			Observer is in detached and connection will be removed.
		*/
		AS_DETACHED
	};

	struct ObserverInfo
	{
		AttachingState state;
		istd::IChangeable::ChangeSet mask;
	};

	typedef QMap<IObserver*, ObserverInfo> ObserversMap;
	ObserversMap m_observers;

	/**
		\brief Nesting depth of change transactions.

		Tracks the number of nested BeginChanges/EndChanges calls. When it reaches 0
		in NotifyAfterChange, the outermost transaction is complete and observer
		AfterUpdate notifications are sent. Managed via CChangeScope for exception safety.

		\note The type is intentionally \c int rather than \c unsigned \c int because
		unsigned underflow in the expression \c --m_blockCounter \c > \c 0 would silently
		produce a very large value instead of the expected comparison result.

		\invariant m_blockCounter >= 0
		\invariant (m_blockCounter > 0) || !m_isDuringChanges

		\sa m_isDuringChanges, CChangeScope
	*/
	int m_blockCounter;

	/**
		\brief Flag indicating whether observer BeforeUpdate notifications have been sent.

		Set to \c true when the first non-group change begins and observer BeforeUpdate
		notifications are sent. Reset to \c false in NotifyAfterChange when the outermost
		transaction completes (m_blockCounter reaches 0).

		\sa m_blockCounter
	*/
	bool m_isDuringChanges;

	/**
		\brief Accumulated change flags for the current transaction.

		Collects all change IDs from nested BeginChanges/EndChanges calls.
		Reset when the outermost transaction completes.
	*/
	istd::IChangeable::ChangeSet m_cumulatedChangeIds;

#if QT_VERSION >= 0x060000
	mutable QRecursiveMutex m_mutex;
#else
	mutable QMutex m_mutex;
#endif
};


// public inline methods

inline istd::IChangeable::ChangeSet CModelBase::GetCumulatedChanges() const
{
	QMutexLocker lock(&m_mutex);

	return m_cumulatedChangeIds;
}


} // namespace imod



