#include "i2d/CDataNodePolylineBase.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"


namespace i2d
{


// reimplemented (iser::ISerializable)

bool CDataNodePolylineBase::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag nodesDataTag("UserData", "User data stored in the nodes of polyline");
	static iser::CArchiveTag nodeDataTag("Node", "Data stored in the polyline node");

	static ChangeSet changeSet(CF_OBJECT_POSITION, CF_ALL_DATA);
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, changeSet);

	bool retVal = BaseClass::Serialize(archive);

	// static attributes
	if (retVal){
		int nodesCount = GetNodesCount();
		retVal = retVal && archive.BeginTag(nodesDataTag);
		for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
			iser::ISerializable& nodeData = GetNodeDataRef(nodeIndex);

			retVal = retVal && archive.BeginTag(nodeDataTag);
			retVal = retVal && nodeData.Serialize(archive);
			retVal = retVal && archive.EndTag(nodeDataTag);
		}
		retVal = retVal && archive.EndTag(nodesDataTag);
	}

	return retVal;
}


} // namespace i2d


