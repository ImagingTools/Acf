#include "CChannelSelectionNode.h"


#include <QString>


namespace imebase
{


CChannelSelectionNode::CChannelSelectionNode()
:	m_selectedIndex(0)
{
}


void CChannelSelectionNode::InsertNode(int physicalAddress, CChannelSelectionNode* nodePtr)
{
	SelectionInfo* newNodeInfoPtr = new SelectionInfo;
	newNodeInfoPtr->selectionPtr.SetPtr(nodePtr);
	newNodeInfoPtr->physicalIndex = physicalAddress;

	m_subselections.PushBack(newNodeInfoPtr);
}


int CChannelSelectionNode::GetActivePhysicalAddress() const
{
	if (m_selectedIndex >= GetOptionsCount()){
		return NULL;
	}

	SelectionInfo* infoPtr = m_subselections[m_selectedIndex];
	I_ASSERT(infoPtr != NULL);

	return m_subselections[m_selectedIndex]->physicalIndex;
}


// reimplemented (iser::ISelectionParam)

int CChannelSelectionNode::GetOptionsCount() const
{
	return m_subselections.GetCount();
}


int CChannelSelectionNode::GetSelectedOptionIndex() const
{
	return m_selectedIndex;
}


bool CChannelSelectionNode::SetSelectedOptionIndex(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetOptionsCount());

	m_selectedIndex = index;
}


const istd::CString& CChannelSelectionNode::GetOptionName(int index) const
{
	return iqt::GetQString(QString::number(index));
}


ISelectionParam* CChannelSelectionNode::GetActiveSubselection() const
{
	if (m_selectedIndex >= GetOptionsCount()){
		return NULL;
	}

	SelectionInfo* infoPtr = m_subselections[m_selectedIndex];
	I_ASSERT(infoPtr != NULL);

	return m_subselections[m_selectedIndex]->selectionPtr.GetPtr();
}

	
// reimplemented (iser::ISerializable)

bool CChannelSelectionNode::Serialize(iser::IArchive& archive)
{
	iser::CArchiveTag addressTag("Address", "Address of complete Meilhaus channel");
	iser::CArchiveTag selectionTag("Selection", "Selected index of single Meilhaus channel node");

	bool retVal = true;

	int selectionDepth = 0;
	if (archive.IsStoring()){
		selectionDepth = CalcSelectionDepth();
	}

	retVal = retVal && archive.BeginMultiTag(addressTag, selectionTag, selectionDepth);
	retVal = retVal && SerializeNode(archive, selectionTag, selectionDepth);
	retVal = retVal && archive.EndTag(addressTag);

	return retVal;
}


// protected methods

int CChannelSelectionNode::CalcSelectionDepth() const
{
	const CChannelSelectionNode* selectionPtr = dynamic_cast<CChannelSelectionNode*>(GetActiveSubselection());
	if (selectionPtr != NULL){
		return selectionPtr->CalcSelectionDepth() + 1;
	}

	return 1;
}


bool CChannelSelectionNode::SerializeNode(iser::IArchive& archive, const iser::CArchiveTag& nodeTag, int count)
{
	if (count <= 0){
		return true;
	}

	bool retVal = true;

	retVal = retVal && archive.BeginTag(nodeTag);
	retVal = retVal && archive.Process(m_selectedIndex);
	retVal = retVal && archive.EndTag(nodeTag);

	if (m_selectedIndex >= GetOptionsCount()){
		index = istd::Max(0, GetOptionsCount() - 1);
	}

	if (!retVal){
		return false;
	}

	const CChannelSelectionNode* selectionPtr = dynamic_cast<CChannelSelectionNode*>(GetActiveSubselection());
	if (selectionPtr != NULL){
		return selectionPtr->SerializeNode(archive, nodeTag, count - 1);
	}
	else{
		while (--count > 0){
			int dummy = 0;
			retVal = retVal && archive.BeginTag(nodeTag);
			retVal = retVal && archive.Process(dummy);
			retVal = retVal && archive.EndTag(nodeTag);
		}
	}

	return retVal;
}


} // namespace imebase


