#include "binarywritearchive.h"
#include "istd/CString.h"


namespace iser
{


CBinaryWriteArchiveBase::CBinaryWriteArchiveBase()
{
	m_version = 0xffffffff;
}


CBinaryWriteArchiveBase::~CBinaryWriteArchiveBase()
{
}


bool CBinaryWriteArchiveBase::Process(CString& value)
{
	int size = value.size() * sizeof(wchar_t);
	ProcessData(&size, sizeof(int));

	return ProcessData((void*)value.c_str(), size);	
}


bool CBinaryWriteArchiveBase::Process(std::string& value)
{			
	int size = value.size() * sizeof(char);
	ProcessData(&size, sizeof(int));

	return ProcessData((void*)value.c_str(), size);
}


bool CBinaryWriteArchiveBase::BeginTag(const std::string& tag)
{
	bool result = Process(const_cast<std::string&>(tag));
	if (result){
		m_tags.push_back(tag);
	}

	return result;
}


bool CBinaryWriteArchiveBase::EndTag()
{
	if (m_tags.size() == 0){
		return false;
	}

	bool result = Process(m_tags.back());
	m_tags.pop_back();

	return result;
}


bool CBinaryWriteArchiveBase::IsStoring() const
{
	return true;
}


} // namespace iser

