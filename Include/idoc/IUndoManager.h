// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <idoc/IDocumentStateComparator.h>


namespace idoc
{


/**
	Interface providing UNDO/REDO functionality.
	
	The undo manager tracks changes made to a document and provides the ability to
	reverse (undo) and reapply (redo) those changes. It maintains a history of
	document states and allows navigation through that history.
	
	Key features:
	- Maintains a stack of undo/redo operations
	- Provides step counts and descriptions
	- Integrates with document state comparison
	- Supports multiple undo/redo levels
	- Can be reset to clear history
	
	The undo manager works in conjunction with IDocumentStateComparator to track
	and compare document states.
	
	\par Usage Example
	\code
	// Get undo manager for a document
	idoc::IUndoManager* undoMgr = GetUndoManagerForDocument(doc);
	
	// Check if undo is available
	if (undoMgr->GetAvailableUndoSteps() > 0) {
		QString desc = undoMgr->GetUndoLevelDescription(0);
		qDebug() << "Can undo:" << desc;
		undoMgr->DoUndo(); // Undo last change
	}
	
	// Check if redo is available
	if (undoMgr->GetAvailableRedoSteps() > 0) {
		QString desc = undoMgr->GetRedoLevelDescription(0);
		qDebug() << "Can redo:" << desc;
		undoMgr->DoRedo(); // Redo last undone change
	}
	
	// Undo multiple steps at once
	int stepsToUndo = 3;
	if (undoMgr->GetAvailableUndoSteps() >= stepsToUndo) {
		undoMgr->DoUndo(stepsToUndo);
	}
	
	// Clear all undo/redo history
	undoMgr->ResetUndo();
	
	// Get descriptions of all undo steps
	int count = undoMgr->GetAvailableUndoSteps();
	for (int i = 0; i < count; ++i) {
		QString desc = undoMgr->GetUndoLevelDescription(i);
		qDebug() << "Undo step" << i << ":" << desc;
	}
	\endcode
	
	\sa IDocumentManager, IDocumentStateComparator, CSerializedUndoManagerComp
	\ingroup DocumentBasedFramework
*/
class IUndoManager: virtual public IDocumentStateComparator
{
public:
	/**
		Get number of available UNDO levels.
		\return	Number of UNDO levels, or 0 if there is no UNDO available.
		
		Each level represents a separate change that can be undone.
	*/
	virtual int GetAvailableUndoSteps() const = 0;
	
	/**
		Get number of available REDO levels.
		\return	Number of REDO levels, or 0 if there is no REDO available.
		
		REDO levels are created when undo operations are performed.
	*/
	virtual int GetAvailableRedoSteps() const = 0;
	/**
		Get description of single UNDO level.
		\param	stepIndex	Index of the undo step (0 = most recent).
		\return	Human-readable description of what will be undone.
	*/
	virtual QString GetUndoLevelDescription(int stepIndex) const = 0;
	
	/**
		Get description of single REDO level.
		\param	stepIndex	Index of the redo step (0 = most recent).
		\return	Human-readable description of what will be redone.
	*/
	virtual QString GetRedoLevelDescription(int stepIndex) const = 0;
	
	/**
		Reset all UNDO and REDO steps.
		
		This clears the entire undo/redo history. Use with caution.
	*/
	virtual void ResetUndo() = 0;
	
	/**
		Process UNDO steps.
		\param	steps	Number of undo levels to execute (default is 1).
		\return	True if undo was successful, false otherwise.
	*/
	virtual bool DoUndo(int steps = 1) = 0;
	
	/**
		Process REDO steps.
		\param	steps	Number of redo levels to execute (default is 1).
		\return	True if redo was successful, false otherwise.
	*/
	virtual bool DoRedo(int steps = 1) = 0;
};


typedef istd::TUniqueInterfacePtr<idoc::IUndoManager> IUndoManagerUniquePtr;
typedef istd::TSharedInterfacePtr<idoc::IUndoManager> IUndoManagerSharedPtr;


} // namespace idoc




