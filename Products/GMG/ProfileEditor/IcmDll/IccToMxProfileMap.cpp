// IccToMxProfileMap.cpp : main implementation file for the class CIccToMxProfileMap
//

#include "stdafx.h"

#include "IccToMxProfileMap.h"

// ACF Includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


::std::string CIccToMxProfileMap::FindMxProfile(const ::std::string& inputIccId, const ::std::string& outputIccId) const
{
	MxProfileKey key(inputIccId, outputIccId);

	MxProfileMap::const_iterator iter = m_mxProfileMap.find(key);
	if (iter != m_mxProfileMap.end()){
		return iter->second;
	}

//	return "C:\\Colorproof\\reference profiles\\ColorServer Profiles\\CMYK Conversion Profiles\\CS_con_ISO27_2_ISO28_V3_beta.mx4";
	return "";
}




// reimplemented (iser::ISerializable)

bool CIccToMxProfileMap::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	int elementsCount = int(m_mxProfileMap.size());

	static iser::CArchiveTag mapListTag("MxMapList", "List of map elements of ICC profiles to MX4");
	static iser::CArchiveTag profileMapTag("ProfileMap", "Map of two ICC profiles to single MX4");

	retVal = retVal && archive.BeginMultiTag(mapListTag, profileMapTag, elementsCount, true);

	if (archive.IsStoring()){
		for (		MxProfileMap::iterator iter = m_mxProfileMap.begin();
					iter != m_mxProfileMap.end();
					++iter){
			retVal = retVal && archive.BeginTag(profileMapTag);

			retVal = retVal && SerializeSingleMap(archive, *iter);

			retVal = retVal && archive.EndTag(profileMapTag);
		}
	}
	else{
		for (int i = 0; i < elementsCount; ++i){
			retVal = retVal && archive.BeginTag(profileMapTag);

			MxProfileMap::value_type singleMap;
			retVal = retVal && SerializeSingleMap(archive, singleMap);

			if (!retVal){
				return false;
			}

			m_mxProfileMap[singleMap.first] = singleMap.second;

			retVal = retVal && archive.EndTag(profileMapTag);
		}
	}

	retVal = retVal && archive.EndTag(mapListTag);

	return retVal;
}


// protected static methods

bool CIccToMxProfileMap::SerializeSingleMap(iser::IArchive& archive, MxProfileMap::value_type& singleMap)
{
	static iser::CArchiveTag keyTag("Key", "Mapping key");
	static iser::CArchiveTag inputIccIdTag("InputIccId", "ID of input ICC");
	static iser::CArchiveTag outputIccIdTag("OutputIccId", "ID of output ICC");
	static iser::CArchiveTag mxProfileTag("MxProfile", "Path to MX4 profile file");

	MxProfileKey& key = const_cast<MxProfileKey&>(singleMap.first);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(keyTag);

	retVal = retVal && archive.BeginTag(inputIccIdTag);
	retVal = retVal && archive.Process(key.first);
	retVal = retVal && archive.EndTag(inputIccIdTag);

	retVal = retVal && archive.BeginTag(outputIccIdTag);
	retVal = retVal && archive.Process(key.second);
	retVal = retVal && archive.EndTag(outputIccIdTag);

	retVal = retVal && archive.EndTag(keyTag);

	retVal = retVal && archive.BeginTag(mxProfileTag);
	retVal = retVal && archive.Process(singleMap.second);
	retVal = retVal && archive.EndTag(mxProfileTag);

	return retVal;
}


