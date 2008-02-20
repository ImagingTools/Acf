#ifndef iser_CXmlReadArchiveBase_included
#define iser_CXmlReadArchiveBase_included


#include "iser/CReadArchiveBase.h"


namespace iser
{


class CXmlReadArchiveBase: public iser::CReadArchiveBase
{
public:
	typedef iser::CReadArchiveBase BaseClass;

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const CArchiveTag& tag, bool useTagSkipping = false);
	virtual bool BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count, bool useTagSkipping = false);
	virtual bool EndTag(const CArchiveTag& tag);
	virtual bool Process(bool& value);
	virtual bool Process(char& value);
	virtual bool Process(I_BYTE& value);
	virtual bool Process(I_SBYTE& value);
	virtual bool Process(I_WORD& value);
	virtual bool Process(I_SWORD& value);
	virtual bool Process(I_DWORD& value);
	virtual bool Process(I_SDWORD& value);
	virtual bool Process(I_QWORD& value);
	virtual bool Process(I_SQWORD& value);
	virtual bool Process(float& value);
	virtual bool Process(double& value);
	virtual bool Process(::std::string& value);
	virtual bool Process(istd::CString& value);
	virtual bool ProcessData(void* dataPtr, int size);

protected:
	CXmlReadArchiveBase(const iser::CArchiveTag& rootTag);

	virtual bool SerializeXmlHeader();

	// template methods
	template <typename Type>
	bool ProcessInternal(Type& value);

	// abstract methods
	/**
		Read input stream till one of specified delimeters is found.
		\param	delimeters		list of delimeters.
		\param	result			result string object. White spaces will be trimmed at the begin and end of this string.
								Delimeter will be not excluded from this string.
		\param	skipDelimeter	if it is true, delimeter will be not parsed by next call of this method.
		\param	foundDelimeter	optional found delimeter will be returned in this character (it means 1 byte, it is not C string).
	*/
	virtual bool ReadToDelimeter(const ::std::string& delimeters, ::std::string& result, bool skipDelimeter = true, char* foundDelimeter = NULL) = 0;

private:
	iser::CArchiveTag m_rootTag;
};


// template methods

template <typename Type>
bool CXmlReadArchiveBase::ProcessInternal(Type& value)
{
	::std::string elementText;

	if (Process(elementText) && !elementText.empty()){
		::std::istrstream stream(elementText.c_str());

		stream >> value;

		return true;
	}

	return false;
}


} // namespace iser


#endif // !iser_CXmlReadArchiveBase_included


