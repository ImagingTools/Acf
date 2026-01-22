// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace idoc
{


/**
	Allows to store some document state used as reference for comparison.
	
	This interface provides functionality for tracking document state changes by storing
	snapshots of document state and comparing current state against stored snapshots.
	This is fundamental to undo/redo functionality and dirty state tracking.
	
	The comparator can:
	- Store a snapshot of the current document state
	- Restore a previously stored state
	- Check if the current state differs from the stored state
	- Determine if a stored state exists
	
	\par Usage Example
	\code
	idoc::IDocumentStateComparator* comparator = GetStateComparator();
	
	// Store initial document state (e.g., after loading or saving)
	comparator->StoreDocumentState();
	
	// Make changes to document...
	
	// Check if document has been modified
	idoc::IDocumentStateComparator::DocumentChangeFlag flag = 
		comparator->GetDocumentChangeFlag();
	
	if (flag == idoc::IDocumentStateComparator::DCF_DIFFERENT) {
		qDebug() << "Document has been modified";
	} else if (flag == idoc::IDocumentStateComparator::DCF_EQUAL) {
		qDebug() << "Document is unchanged";
	}
	
	// Restore to stored state (undo all changes)
	if (comparator->HasStoredDocumentState()) {
		comparator->RestoreDocumentState();
		qDebug() << "Document restored to previous state";
	}
	
	// After saving, store new state as clean baseline
	SaveDocument();
	comparator->StoreDocumentState();
	\endcode
	
	\sa IUndoManager
	\ingroup DocumentBasedFramework
*/
class IDocumentStateComparator: virtual public istd::IChangeable
{
public:
	/**
		Flag indicating the relationship between current and stored document state.
	*/
	enum DocumentChangeFlag
	{
		/**
			There is no information about document change (no state stored).
		*/
		DCF_UNKNOWN,
		
		/**
			Current document state equals the stored one (no changes).
		*/
		DCF_EQUAL,
		
		/**
			Current document state differs from the stored one (has changes).
		*/
		DCF_DIFFERENT
	};

	/**
		Check if there is stored document state available.
		\return	True if a document state has been stored and is available for comparison.
	*/
	virtual bool HasStoredDocumentState() const = 0;
	
	/**
		Store state of the document.
		\return	True if state was successfully stored, false otherwise.
		
		This creates a snapshot of the current document state that can be used
		for comparison or restoration later.
	*/
	virtual bool StoreDocumentState() = 0;
	
	/**
		Restore last stored document state.
		\return	True if state was successfully restored, false otherwise.
		
		This reverts the document to the last stored state, effectively undoing
		all changes made since the state was stored.
	*/
	virtual bool RestoreDocumentState() = 0;
	
	/**
		Check if document state is the same as stored state.
		\return	Flag indicating whether current state matches stored state.
	*/
	virtual DocumentChangeFlag GetDocumentChangeFlag() const = 0;
};


} // namespace idoc




