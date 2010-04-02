#include "icomp/CComponentMetaDescriptionEncoder.h"


#include "istd/CRange.h"


namespace icomp
{


// public methods

CComponentMetaDescriptionEncoder::CComponentMetaDescriptionEncoder(const istd::CString& metaDescription)
	:m_metaDescription(metaDescription)
{
	CalculateMetaValuesMap();
}


istd::CStringList CComponentMetaDescriptionEncoder::GetMetaKeys() const
{
	istd::CStringList keyList;

	for(		MetaValuesMap::const_iterator index = m_metaValuesMap.begin();
				index != m_metaValuesMap.end();
				index++){
		keyList.push_back(index->first);
	}

	return keyList;
}


istd::CStringList CComponentMetaDescriptionEncoder::GetValues(const istd::CString& key) const
{
	static istd::CStringList emptyList;

	if (!key.IsEmpty()){
		MetaValuesMap::const_iterator foundKeyIter = m_metaValuesMap.find(key);
		if (foundKeyIter != m_metaValuesMap.end()){
			return foundKeyIter->second;
		}
	}
	else{
		istd::CStringList keyList = GetMetaKeys();
		istd::CStringList retVal;

		for (int index = 0; index < int(keyList.size()); index++){
			istd::CStringList keyMatches = GetValues(keyList[index]);

			retVal.insert(retVal.end(), keyMatches.begin(), keyMatches.end());
		}

		return retVal;
	}

	return emptyList;
}


void CComponentMetaDescriptionEncoder::SetValues(const istd::CString& key, const istd::CStringList& values)
{
	m_metaValuesMap[key] = values;
}



istd::CString CComponentMetaDescriptionEncoder::GetMetaDescription() const
{
	return m_metaDescription;
}


// private methods

void CComponentMetaDescriptionEncoder::CalculateMetaValuesMap()
{
	m_metaValuesMap.clear();

	istd::CString metaDescriptionText = m_metaDescription;

	istd::CString::size_type categorySeparator = metaDescriptionText.find_first_of(':');
	if (categorySeparator == istd::CString::npos){
		m_metaValuesMap["Tags"] = m_metaDescription.Split(" ", false);
	}
	else{
		bool workDone = false;
		do{
			workDone = true;

			istd::CString::size_type endKey = metaDescriptionText.find_first_of(':', 0);
			if (endKey != istd::CString::npos){
				istd::CString categoryKey = metaDescriptionText.substr(0, endKey);
				
				RemoveUnusedCharacters(categoryKey, " ");

				istd::CString::size_type startValue = endKey + 1;
				istd::CString::size_type endValue = metaDescriptionText.find_first_of(':', startValue);
				if (endValue != istd::CString::npos){
					istd::CString valueString = metaDescriptionText.substr(0, endValue);
					endValue = valueString.find_last_of(' ');	
				}
				else{
					endValue = metaDescriptionText.length();
				}

				istd::CString valueString = metaDescriptionText.substr(startValue, endValue - startValue);
				if (!valueString.IsEmpty()){
					RemoveUnusedCharacters(valueString, " ");

					m_metaValuesMap[categoryKey] = SplitString(valueString);
				}

				metaDescriptionText = metaDescriptionText.substr(endValue, metaDescriptionText.length() - endValue);

				workDone = false;
			}

		} while (!workDone);
	}
}


// private static methods

void CComponentMetaDescriptionEncoder::RemoveUnusedCharacters(istd::CString& string, const istd::CString& characters)
{
	istd::CString::size_type foundPos = 0;
	while ((foundPos = string.find_first_of(characters)) != istd::CString::npos && foundPos == 0){
		string = string.substr(foundPos + 1);
	}

	while ((foundPos = string.find_last_of(characters)) != istd::CString::npos && foundPos == string.length() - 1){
		string = string.substr(0, foundPos);
	}
}


istd::CStringList CComponentMetaDescriptionEncoder::SplitString(istd::CString& string)
{
	istd::CStringList retVal;

	std::vector<int> quatationMarkIndexes;

	istd::CString::size_type foundPos = 0;
	while ((foundPos = string.find_first_of('\"', foundPos)) != istd::CString::npos){
		quatationMarkIndexes.push_back(foundPos);

		foundPos++;
	}

	if (quatationMarkIndexes.empty()){
		return string.Split(" ", false);
	}

	if ((quatationMarkIndexes.size() % 2) != 0){
		istd::CString leftString = string.substr(0, quatationMarkIndexes.back());
		istd::CString rightString = string.substr(quatationMarkIndexes.back() + 1);

		string = leftString + rightString;
	}

	std::vector<istd::CRange> quatationMarks;
	for (int quatationMarkIndex = 0; quatationMarkIndex < int(quatationMarkIndexes.size()); quatationMarkIndex += 2){
		quatationMarks.push_back(istd::CRange(quatationMarkIndexes[quatationMarkIndex], quatationMarkIndexes[quatationMarkIndex + 1]));
	}

	
	istd::CString::size_type offset = 0;
	foundPos = 0;

	while ((foundPos = string.find_first_of(' ', foundPos)) != istd::CString::npos){
		bool splitString = true;
		for (int quatationMarksIndex = 0; quatationMarksIndex < int(quatationMarks.size()); quatationMarksIndex++){
			if (quatationMarks[quatationMarksIndex].Contains(foundPos)){
				splitString = false;
				offset = int(quatationMarks[quatationMarksIndex].GetMinValue()) + 1;
				break;
			}
		}

		if (splitString){
			istd::CString splitedString = string.substr(offset, foundPos - offset);
			
			RemoveUnusedCharacters(splitedString, "\"");
			
			retVal.push_back(splitedString);
			
			offset = foundPos + 1;
		}

		foundPos++;
	}

	if (offset < string.size()){
		istd::CString splitedString = string.substr(offset);

		RemoveUnusedCharacters(splitedString, "\"");

		retVal.push_back(splitedString);
	}

	return retVal;
}


} // namespace icomp


