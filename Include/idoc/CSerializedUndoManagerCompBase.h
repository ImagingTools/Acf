// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/TDelPtr.h>
#include <iser/ISerializable.h>
#include <iser/CMemoryWriteArchive.h>
#include <imod/TSingleModelObserverBase.h>
#include <icomp/CComponentBase.h>
#include <idoc/IUndoManager.h>


namespace idoc
{


/**
	Base implementation of a multi-level UNDO mechanism based on storing complete object state at each step.

	This component provides the common infrastructure of an undo/redo implementation by capturing
	a complete snapshot of the document state before each change. It maintains separate undo and redo
	stacks, observes the document model and integrates with the ACF model/observer pattern.

	The way a single document state (undo/redo step) is stored and restored is left abstract, so that
	derived classes can decide whether to keep the snapshots in memory (\ref CSerializedUndoManagerComp)
	or persist them into files (\ref CFileSerializedUndoManagerComp).

	\par Component Attributes
	- \b MaxBufferSize - Maximum size for the undo buffer in megabytes (default: 100 MB)

	\par Registered Interfaces
	- idoc::IUndoManager - Provides undo/redo operations
	- idoc::IDocumentStateComparator - Allows state comparison
	- imod::IObserver - Observes document changes

	\note The document must implement iser::ISerializable for this undo manager to work.

	\sa CSerializedUndoManagerComp, CFileSerializedUndoManagerComp, IUndoManager, IDocumentStateComparator
	\ingroup DocumentBasedFramework
*/
class CSerializedUndoManagerCompBase:
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<iser::ISerializable>,
			virtual public IUndoManager
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<iser::ISerializable> BaseClass2;

	I_BEGIN_BASE_COMPONENT(CSerializedUndoManagerCompBase);
		I_REGISTER_INTERFACE(idoc::IUndoManager);
		I_REGISTER_INTERFACE(idoc::IDocumentStateComparator);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_maxBufferSizeAttrPtr, "MaxBufferSize", "Maximal size of the Undo-buffer in MByte", false, 100);
	I_END_COMPONENT;

	CSerializedUndoManagerCompBase();

	// reimplemented (idoc::IUndoManager)
	virtual int GetAvailableUndoSteps() const override;
	virtual int GetAvailableRedoSteps() const override;
	virtual QString GetUndoLevelDescription(int stepIndex) const override;
	virtual QString GetRedoLevelDescription(int stepIndex) const override;
	virtual void ResetUndo() override;
	virtual bool DoUndo(int steps = 1) override;
	virtual bool DoRedo(int steps = 1) override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;
	virtual bool OnModelDetached(imod::IModel* modelPtr) override;

protected:
	/**
		Abstract handle to a single stored document state (one undo/redo step).

		Concrete implementations are responsible for freeing any storage they use (e.g. deleting
		the backing file) in their destructor, so that removing an entry from the undo or redo list
		automatically releases the associated resources.
	*/
	class IUndoState: virtual public istd::IPolymorphic
	{
	public:
		/**
			Approximate size in bytes used to store this state.
			It is used to limit the total size of the undo buffer.
		*/
		virtual qint64 GetStateSize() const = 0;
	};

	typedef istd::TDelPtr<IUndoState> UndoStatePtr;

	struct UndoStepInfo
	{
		UndoStatePtr statePtr;
		QString description;
	};

	typedef QList<UndoStepInfo> UndoList;

	bool DoListShift(int steps, UndoList& fromList, UndoList& toList);

	/**
		Create a new state object holding a snapshot of the current state of \a object.
		\return Pointer to a newly allocated state, or \c NULL if the snapshot could not be created.
	*/
	virtual IUndoState* CreateState(iser::ISerializable& object) = 0;

	/**
		Restore \a object from a previously stored \a state.
	*/
	virtual bool RestoreState(const IUndoState& state, iser::ISerializable& object) = 0;

	/**
		Check whether two stored states are equal.
	*/
	virtual bool AreStatesEqual(const IUndoState& state1, const IUndoState& state2) const = 0;

	/**
		Restore the currently observed object from the given \a state.

		The observed document is restored without recording the change in the undo/redo history,
		so it can be used to bring the document in sync with a state loaded from persistent storage.
		\return \c true if an observed object exists and could be restored from \a state.
	*/
	bool RestoreObservedObject(const IUndoState& state);

	/**
		Called whenever the number of available undo steps (i.e. the current step position) may have changed.
		The default implementation does nothing.
	*/
	virtual void OnUndoPositionChanged();

	// reimplemented (imod::TSingleModelObserverBase<iser::ISerializable>)
	virtual iser::ISerializable* CastFromModel(imod::IModel* modelPtr) const override;

	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr) override;
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (idoc::IDocumentStateComparator)
	virtual bool HasStoredDocumentState() const override;
	virtual bool StoreDocumentState() override;
	virtual bool RestoreDocumentState() override;
	virtual DocumentChangeFlag GetDocumentChangeFlag() const override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed() override;

	UndoList m_undoList;
	UndoList m_redoList;

private:
	qint64 GetUsedMemorySize() const;

	UndoStatePtr m_beginStatePtr;

	bool m_hasStoredDocumentState;
	bool m_isBlocked;

	iser::CMemoryWriteArchive m_storedStateArchive;

	mutable DocumentChangeFlag m_stateChangedFlag;
	mutable bool m_isStateChangedFlagValid;

	I_ATTR(int, m_maxBufferSizeAttrPtr);
};


} // namespace idoc
