#include "i2d/CPolypoint.h"


namespace i2d
{


// reimplemented (iser::ISerializable)

bool CPolypoint::Serialize(iser::IArchive& archive)
{
	int pointsCount = m_points.size();
	bool retVal = true;

	static iser::CArchiveTag polypointTag("Polypoint", "Polypoint");
	static iser::CArchiveTag vectorTag("V", "Vector");
	retVal = retVal && archive.BeginMultiTag(polypointTag, vectorTag, pointsCount);

	if (!archive.IsStoring() && retVal){
		m_points.resize(pointsCount);
	}

	for (int nodeIndex = 0; nodeIndex < pointsCount; ++nodeIndex){
		retVal = retVal && archive.BeginTag(vectorTag);
		retVal = retVal && m_points[nodeIndex].Serialize(archive);
		retVal = retVal && archive.EndTag(vectorTag);
	}

	retVal = retVal && archive.EndTag(polypointTag);

	return retVal;
}


} // namespace i2d

