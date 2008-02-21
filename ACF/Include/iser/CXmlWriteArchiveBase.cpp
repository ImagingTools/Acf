#include "iser/CXmlWriteArchiveBase.h"


#include <strstream>


namespace iser
{


CXmlWriteArchiveBase::~CXmlWriteArchiveBase()
{
	Flush();
}


// reimplemented (iser::IArchive)

bool CXmlWriteArchiveBase::BeginTag(const CArchiveTag& tag, bool /*useTagSkipping*/)
{
	bool retVal = MakeIndent() && WriteString("<" + tag.GetId() + ">");

	++m_indent;

	m_isSeparatorNeeded = false;

	return retVal;
}


bool CXmlWriteArchiveBase::BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count, bool /*useTagSkipping*/)
{
	::std::ostrstream stream;
	stream << count;

	bool retVal = MakeIndent() && WriteString("<" + tag.GetId() + " count=\"" + stream.str() + "\">");

	++m_indent;

	m_isSeparatorNeeded = false;

	return retVal;
}


bool CXmlWriteArchiveBase::EndTag(const CArchiveTag& tag)
{
	--m_indent;

	return MakeIndent() && WriteString("</" + tag.GetId() + ">");
}


bool CXmlWriteArchiveBase::Process(bool& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(char& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(I_BYTE& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(I_SBYTE& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(I_WORD& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(I_SWORD& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(I_DWORD& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(I_SDWORD& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(I_QWORD& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(I_SQWORD& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(float& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(double& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(::std::string& value)
{
	return ProcessInternal(value);
}


bool CXmlWriteArchiveBase::Process(istd::CString& value)
{
	return ProcessInternal(value.ToString());
}


bool CXmlWriteArchiveBase::ProcessData(void* dataPtr, int size)
{
	return false;
}


// protected methods

CXmlWriteArchiveBase::CXmlWriteArchiveBase(const IVersionInfo* versionInfoPtr, const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr),
	m_rootTag(rootTag),
	m_indent(0),
	m_isHeaderSerialized(false),
	m_isSeparatorNeeded(false)
{
}


bool CXmlWriteArchiveBase::Flush()
{
	if (m_isHeaderSerialized){
		m_isHeaderSerialized = false;

		return EndTag(m_rootTag);
	}

	return true;
}


bool CXmlWriteArchiveBase::MakeIndent()
{
	return WriteString(::std::string(' ', m_indent));
}


bool CXmlWriteArchiveBase::SerializeXmlHeader()
{
	bool retVal = true;
	
	retVal = retVal && WriteString("<?xml version=\"1.0\"?>\n");
	retVal = retVal && BeginTag(m_rootTag);

	m_isHeaderSerialized = true;

	return retVal;
}


} // namespace iser


