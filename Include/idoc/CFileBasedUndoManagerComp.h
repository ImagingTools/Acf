// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QList>
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/TDelPtr.h>
#include <iser/ISerializable.h>
#include <iser/CMemoryWriteArchive.h>
#include <imod/TSingleModelObserverBase.h>
#include <icomp/CComponentBase.h>
#include <ifile/IFilePersistence.h>
#include <ifile/IFileNameParam.h>
#include <idoc/IUndoManager.h>


namespace idoc
{


/**
	Implements multi-level UNDO mechanism based on storing complete object state at each step in files.

	Unlike \ref CSerializedUndoManagerComp, which keeps all undo/redo snapshots in memory, this
	component persists each document state to a separate file using an external file persistence.
	This keeps the memory footprint small and is well suited for large documents or long undo histories.

	Each undo/redo step is written to a file named \c step_&lt;index&gt;.bin inside the directory
	referenced by \b StorageDirectory, where \c index reflects the position of the step within the
	undo history. Files backing steps that are no longer needed (for example when the redo list is
	cleared after a new change) are removed automatically.

	The complete undo/redo history together with the index of the current step can be persisted and
	restored using the iser::ISerializable interface. The observed document is not written into the
	archive itself: instead, its state at the current step is stored as a dedicated step file and,
	when loading the history, the observed document is brought back to the content it had at the
	current step by deserializing it from the file corresponding to that step.

	\par Component References
	- \b DocumentPersistence (ifile::IFilePersistence) - Persistence used to save/load document states
	- \b StorageDirectory (ifile::IFileNameParam) - Directory used to store the step files

	\par Registered Interfaces
	- idoc::IUndoManager - Provides undo/redo operations
	- idoc::IDocumentStateComparator - Allows state comparison
	- imod::IObserver - Observes document changes
	- iser::ISerializable - Persists the undo/redo history and the current step index

	\note The document must be supported by the referenced file persistence for this undo manager to work.

	\sa CSerializedUndoManagerComp, IUndoManager, IDocumentStateComparator
	\ingroup DocumentBasedFramework
*/
class CFileBasedUndoManagerComp:
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<iser::ISerializable>,
			virtual public IUndoManager,
			virtual public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<iser::ISerializable> BaseClass2;

	I_BEGIN_COMPONENT(CFileBasedUndoManagerComp);
		I_REGISTER_INTERFACE(idoc::IUndoManager);
		I_REGISTER_INTERFACE(idoc::IDocumentStateComparator);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_persistenceCompPtr, "DocumentPersistence", "Persistence used to serialize document states to files", true, "DocumentPersistence");
		I_ASSIGN(m_directoryCompPtr, "StorageDirectory", "Directory where the undo step files are stored", true, "StorageDirectory");
	I_END_COMPONENT;

	CFileBasedUndoManagerComp();

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

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	/**
		Abstract handle to a single stored document state (one undo/redo step).

		Concrete implementations are responsible for freeing any storage they use (e.g. deleting
		the backing file) in their destructor, so that removing an entry from the undo or redo list
		automatically releases the associated resources.
	*/
	class IUndoState: virtual public istd::IPolymorphic
	{
	};

	typedef istd::TDelPtr<IUndoState> UndoStatePtr;

	struct UndoStepInfo
	{
		UndoStatePtr statePtr;
		QString description;
	};

	typedef QList<UndoStepInfo> UndoList;

	/**
		File-based storage of a single document state.

		The backing file is removed when this object is destroyed, so that clearing the undo or redo
		list automatically deletes the files that are no longer needed.
	*/
	class CFileUndoState: public IUndoState, virtual public iser::ISerializable
	{
	public:
		explicit CFileUndoState(const QString& filePath);
		virtual ~CFileUndoState();

		const QString& GetFilePath() const;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive) override;

	private:
		QString m_filePath;
	};

	bool DoListShift(int steps, UndoList& fromList, UndoList& toList);

	/**
		Create a new state object holding a snapshot of the current state of \a object.
		\return Pointer to a newly allocated state, or \c NULL if the snapshot could not be created.
	*/
	IUndoState* CreateState(iser::ISerializable& object);

	/**
		Restore \a object from a previously stored \a state.
	*/
	bool RestoreState(const IUndoState& state, iser::ISerializable& object);

	/**
		Check whether two stored states are equal.
	*/
	bool AreStatesEqual(const IUndoState& state1, const IUndoState& state2) const;

	/**
		Restore the currently observed object from the given \a state.

		The observed document is restored without recording the change in the undo/redo history,
		so it can be used to bring the document in sync with a state loaded from persistent storage.
		\return \c true if an observed object exists and could be restored from \a state.
	*/
	bool RestoreObservedObject(const IUndoState& state);

	/**
		Build the absolute path of the file used to store the step with the given \a stepIndex.
		\return The file path, or an empty string if the storage directory is not available.
	*/
	QString CreateStepFilePath(int stepIndex) const;

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

private:
	UndoList m_undoList;
	UndoList m_redoList;

	UndoStatePtr m_beginStatePtr;

	int m_uniqueFileCounter;

	/**
		State of the observed document at the current undo step.

		It is created while storing and restored while loading, so that after loading the undo
		history the observed document is brought to the content of the current step.
	*/
	UndoStatePtr m_currentStatePtr;

	bool m_hasStoredDocumentState;
	bool m_isBlocked;

	iser::CMemoryWriteArchive m_storedStateArchive;

	mutable DocumentChangeFlag m_stateChangedFlag;
	mutable bool m_isStateChangedFlagValid;

	I_REF(ifile::IFilePersistence, m_persistenceCompPtr);
	I_REF(ifile::IFileNameParam, m_directoryCompPtr);
};


} // namespace idoc
