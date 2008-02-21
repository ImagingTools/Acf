// IccToMxProfileMap.h : main header file for the class CIccToMxProfileMap
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


// STL Includes
#include <string>
#include <map>

// ACF Includes
#include "iser/ISerializable.h"


class CIccToMxProfileMap: public iser::ISerializable
{
public:
	::std::string FindMxProfile(const ::std::string& inputIccId, const ::std::string& outputIccId) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef ::std::pair<::std::string, ::std::string> MxProfileKey;
	typedef ::std::map<MxProfileKey, ::std::string> MxProfileMap;

	static bool SerializeSingleMap(iser::IArchive& archive, MxProfileMap::value_type& singleMap);

private:
	MxProfileMap m_mxProfileMap;
};


