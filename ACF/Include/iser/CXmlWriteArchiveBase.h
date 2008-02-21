#ifndef iser_CXmlWriteArchiveBase_included
#define iser_CXmlWriteArchiveBase_included


#include "iser/CWriteArchiveBase.h"
#include "iser/CXmlDocumentInfoBase.h"


namespace iser
{


class CXmlWriteArchiveBase: public CWriteArchiveBase, public CXmlDocumentInfoBase
{
public:
	typedef iser::CWriteArchiveBase BaseClass;

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
	CXmlWriteArchiveBase(const IVersionInfo* versionInfoPtr, const iser::CArchiveTag& rootTag);

	bool Flush();
	bool MakeIndent();

	bool SerializeXmlHeader();

	// template methods
	template <typename Type>
	bool ProcessInternal(const Type& value);

	// abstract methods
	virtual bool WriteString(const ::std::string& value) = 0;

private:
	iser::CArchiveTag m_rootTag;

	int m_indent;
	bool m_isHeaderSerialized;

	bool m_isSeparatorNeeded;
};


// template methods

template <typename Type>
bool CXmlWriteArchiveBase::ProcessInternal(const Type& value)
{
	bool retVal = true;

	if (m_isSeparatorNeeded){
		retVal = retVal && MakeIndent();
		retVal = retVal && WriteString("<" + GetElementSeparator() + ">\n");
	}

	retVal = retVal && MakeIndent();

	::std::ostrstream stream;

	stream << value << "\n" << ::std::ends;

	retVal = retVal && WriteString(stream.str());

	return retVal;
}


} // namespace iser


#endif // !iser_CXmlWriteArchiveBase_included


