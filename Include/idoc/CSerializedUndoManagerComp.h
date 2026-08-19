// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CMemoryWriteArchive.h>
#include <idoc/CSerializedUndoManagerCompBase.h>


namespace idoc
{


/**
	Implements multi-level UNDO mechanism based on storing complete object state at each step in memory.

	This component provides a complete undo/redo implementation by serializing the entire
	document state to memory before each change. It maintains separate undo and redo stacks and
	automatically manages memory usage by limiting the buffer size.

	The undo manager observes the document model and automatically captures state snapshots
	when changes occur. It integrates with the ACF model/observer pattern to track when
	undo/redo availability changes.

	Common undo/redo bookkeeping is implemented by \ref CSerializedUndoManagerCompBase; this class
	only provides the in-memory storage of each step.

	\par Component Attributes
	- \b MaxBufferSize - Maximum memory size for undo buffer in megabytes (default: 100 MB)

	\par Registered Interfaces
	- idoc::IUndoManager - Provides undo/redo operations
	- idoc::IDocumentStateComparator - Allows state comparison
	- imod::IObserver - Observes document changes

	\par Configuration Example
	\code
	CSerializedUndoManagerComp {
		MaxBufferSize = 50  // Limit undo buffer to 50 MB
	}
	\endcode

	\par Usage with Document Template
	\code
	CSingleDocumentTemplateComp {
		DocumentTypeId = "TextDocument"
		DocumentFactory = CTextDocumentComp
		UndoManager = CSerializedUndoManagerComp {
			MaxBufferSize = 100
		}
	}
	\endcode

	\note The document must implement iser::ISerializable for this undo manager to work.
	\note Memory usage is automatically managed based on MaxBufferSize setting.
	\note Each undo step stores the complete document state, so large documents may use significant memory.

	\sa CSerializedUndoManagerCompBase, CFileSerializedUndoManagerComp, IUndoManager, IDocumentStateComparator
	\ingroup DocumentBasedFramework
*/
class CSerializedUndoManagerComp: public CSerializedUndoManagerCompBase
{
public:
	typedef CSerializedUndoManagerCompBase BaseClass;

	I_BEGIN_COMPONENT(CSerializedUndoManagerComp);
	I_END_COMPONENT;

protected:
	/**
		In-memory storage of a single document state.
	*/
	class CMemoryUndoState: public IUndoState
	{
	public:
		explicit CMemoryUndoState(iser::CMemoryWriteArchive* archivePtr);

		const iser::CMemoryWriteArchive& GetArchive() const;

		// reimplemented (IUndoState)
		virtual qint64 GetStateSize() const override;

	private:
		istd::TDelPtr<iser::CMemoryWriteArchive> m_archivePtr;
	};

	// reimplemented (CSerializedUndoManagerCompBase)
	virtual IUndoState* CreateState(iser::ISerializable& object) override;
	virtual bool RestoreState(const IUndoState& state, iser::ISerializable& object) override;
	virtual bool AreStatesEqual(const IUndoState& state1, const IUndoState& state2) const override;
};


} // namespace idoc
