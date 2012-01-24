#include "i2d/CPolylineExNodeBase.h"


// ACF includes
#include "iser/IArchive.h"


namespace i2d
{


// reimplemented (iser::ISerializable)

bool CPolylineExNodeBase::Serialize(iser::IArchive& archive)
{
	bool retVal = BaseClass::Serialize(archive);

	// static attributes
	if (retVal){
		int nodesCount = GetNodesCount();
		static iser::CArchiveTag nodesDataTag("NodesExtraData", "Nodes Extra Data");
		retVal = retVal && archive.BeginTag(nodesDataTag);
		for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
			iser::ISerializable& nodeData = GetNodeDataRef(nodeIndex);

			static iser::CArchiveTag nodeDataTag("Node", "Node Data");
			retVal = retVal && archive.BeginTag(nodeDataTag);
			retVal = retVal && nodeData.Serialize(archive);
			retVal = retVal && archive.EndTag(nodeDataTag);
		}
		retVal = retVal && archive.EndTag(nodesDataTag);
	}

	return retVal;
}


} // namespace i2d

