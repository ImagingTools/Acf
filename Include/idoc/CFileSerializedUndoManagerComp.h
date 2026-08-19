// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <ifile/IFilePersistence.h>
#include <ifile/IFileNameParam.h>
#include <iprm/ISelectionParam.h>
#include <idoc/CSerializedUndoManagerCompBase.h>


namespace idoc
{


/**
	Implements multi-level UNDO mechanism based on storing complete object state at each step in files.

	Unlike \ref CSerializedUndoManagerComp, which keeps all undo/redo snapshots in memory, this
	component persists each document state to a separate file using an external file persistence.
	This keeps the memory footprint small and is well suited for large documents or long undo histories.

	Each undo/redo step is written to a file named \c step_&lt;index&gt;.bin inside the directory
	referenced by \b StorageDirectory. Files backing steps that are no longer needed (for example when
	the redo list is cleared after a new change) are removed automatically.

	The index of the current step is written to the external selection parameter referenced by
	\b CurrentStep, so that the current position within the undo history can be observed from outside.

	\par Component References
	- \b DocumentPersistence (ifile::IFilePersistence) - Persistence used to save/load document states
	- \b StorageDirectory (ifile::IFileNameParam) - Directory used to store the step files
	- \b CurrentStep (iprm::ISelectionParam) - External parameter receiving the current step index (optional)

	\par Component Attributes
	- \b MaxBufferSize - Maximum size for the undo buffer in megabytes (default: 100 MB)

	\par Registered Interfaces
	- idoc::IUndoManager - Provides undo/redo operations
	- idoc::IDocumentStateComparator - Allows state comparison
	- imod::IObserver - Observes document changes

	\note The document must be supported by the referenced file persistence for this undo manager to work.

	\sa CSerializedUndoManagerCompBase, CSerializedUndoManagerComp, IUndoManager, IDocumentStateComparator
	\ingroup DocumentBasedFramework
*/
class CFileSerializedUndoManagerComp: public CSerializedUndoManagerCompBase
{
public:
	typedef CSerializedUndoManagerCompBase BaseClass;

	I_BEGIN_COMPONENT(CFileSerializedUndoManagerComp);
		I_ASSIGN(m_persistenceCompPtr, "DocumentPersistence", "Persistence used to serialize document states to files", true, "DocumentPersistence");
		I_ASSIGN(m_directoryCompPtr, "StorageDirectory", "Directory where the undo step files are stored", true, "StorageDirectory");
		I_ASSIGN(m_currentStepCompPtr, "CurrentStep", "External selection parameter receiving the current undo step index", false, "CurrentStep");
	I_END_COMPONENT;

	CFileSerializedUndoManagerComp();

protected:
	/**
		File-based storage of a single document state.

		The backing file is removed when this object is destroyed, so that clearing the undo or redo
		list automatically deletes the files that are no longer needed.
	*/
	class CFileUndoState: public IUndoState
	{
	public:
		explicit CFileUndoState(const QString& filePath);
		virtual ~CFileUndoState();

		const QString& GetFilePath() const;

		// reimplemented (IUndoState)
		virtual qint64 GetStateSize() const override;

	private:
		QString m_filePath;
	};

	/**
		Build the absolute path of the file used to store the step with the given \a stepIndex.
		\return The file path, or an empty string if the storage directory is not available.
	*/
	QString CreateStepFilePath(int stepIndex) const;

	// reimplemented (CSerializedUndoManagerCompBase)
	virtual IUndoState* CreateState(iser::ISerializable& object) override;
	virtual bool RestoreState(const IUndoState& state, iser::ISerializable& object) override;
	virtual bool AreStatesEqual(const IUndoState& state1, const IUndoState& state2) const override;
	virtual void OnUndoPositionChanged() override;

private:
	int m_nextStepIndex;

	I_REF(ifile::IFilePersistence, m_persistenceCompPtr);
	I_REF(ifile::IFileNameParam, m_directoryCompPtr);
	I_REF(iprm::ISelectionParam, m_currentStepCompPtr);
};


} // namespace idoc
