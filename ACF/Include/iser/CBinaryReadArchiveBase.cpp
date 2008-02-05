#include <assert.h>
#include "binaryreadarchive.h"
#include "istd/CString.h"


namespace iser
{


CBinaryReadArchiveBase::CBinaryReadArchiveBase()
{
	m_version = 0;
}


CBinaryReadArchiveBase::~CBinaryReadArchiveBase()
{

}


bool CBinaryReadArchiveBase::Process(CString& value)
{
	int stringLength;

	bool retVal = Process(stringLength);

	if (retVal && (stringLength > 0)){
		if (stringLength > MAX_STRING_LENGTH){
			return false;
		}

		::std::vector<wchar_t> buffer(stringLength + 1, 0);

		retVal = ProcessData(&buffer[0], stringLength * sizeof(wchar_t));	

		if (retVal){
			value = CString(&buffer[0]);
		}
	}

	return retVal;
}


bool CBinaryReadArchiveBase::Process(std::string& value)
{			
	int stringLength;

	bool retVal = Process(stringLength);

	if (retVal && (stringLength > 0)){
		if (stringLength > MAX_STRING_LENGTH){
			return false;
		}

		::std::vector<char> buffer(stringLength + 1, 0);

		retVal = ProcessData(&buffer[0], stringLength * sizeof(char));	

		if (retVal){
			value = std::string(&buffer[0]);
		}
	}
	
	return retVal;
}


bool CBinaryReadArchiveBase::IsStoring() const
{
	return false;
}


bool CBinaryReadArchiveBase::BeginTag(const CArchiveTag& tag, bool* skipFlagPtr)
{
	std::string readTag;
	if (Process(readTag)){
		if (tag == readTag){
			m_tags.push_back(tag);

			return true;
		}
	}

	return false;
}


bool CBinaryReadArchiveBase::EndTag()
{
	assert(m_tags.size() != 0);
	if (m_tags.size() == 0){

		return false;
	}

	std::string readTag;
	if (Process(readTag)){
		std::string tag = m_tags.back();
		if (tag == readTag){
			m_tags.pop_back();

			return true;
		}
	}

	return false;
}


} // namespace iser

