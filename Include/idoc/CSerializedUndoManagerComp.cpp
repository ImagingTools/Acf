// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <idoc/CSerializedUndoManagerComp.h>


// ACF includes
#include <iser/CMemoryReadArchive.h>


namespace idoc
{


// CMemoryUndoState

CSerializedUndoManagerComp::CMemoryUndoState::CMemoryUndoState(iser::CMemoryWriteArchive* archivePtr)
:	m_archivePtr(archivePtr)
{
}


const iser::CMemoryWriteArchive& CSerializedUndoManagerComp::CMemoryUndoState::GetArchive() const
{
	return *m_archivePtr;
}


qint64 CSerializedUndoManagerComp::CMemoryUndoState::GetStateSize() const
{
	return m_archivePtr.IsValid()? m_archivePtr->GetBufferSize(): 0;
}


// reimplemented (CSerializedUndoManagerCompBase)

CSerializedUndoManagerCompBase::IUndoState* CSerializedUndoManagerComp::CreateState(iser::ISerializable& object)
{
	istd::TDelPtr<iser::CMemoryWriteArchive> archivePtr(new iser::CMemoryWriteArchive());

	if (archivePtr.IsValid() && object.Serialize(*archivePtr)){
		return new CMemoryUndoState(archivePtr.PopPtr());
	}

	return NULL;
}


bool CSerializedUndoManagerComp::RestoreState(const IUndoState& state, iser::ISerializable& object)
{
	const CMemoryUndoState& memoryState = static_cast<const CMemoryUndoState&>(state);

	iser::CMemoryReadArchive readArchive(memoryState.GetArchive());

	return object.Serialize(readArchive);
}


bool CSerializedUndoManagerComp::AreStatesEqual(const IUndoState& state1, const IUndoState& state2) const
{
	const CMemoryUndoState& memoryState1 = static_cast<const CMemoryUndoState&>(state1);
	const CMemoryUndoState& memoryState2 = static_cast<const CMemoryUndoState&>(state2);

	return memoryState1.GetArchive() == memoryState2.GetArchive();
}


} // namespace idoc
