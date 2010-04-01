#ifndef ibase_CTextFileLogComp_included
#define ibase_CTextFileLogComp_included


// STL includes
#include <fstream>


// ACF includes
#include "iprm/IFileNameParam.h"

#include "ibase/CStreamLogCompBase.h"


namespace ibase
{


/**
	Message container displaying messages as log list.
*/
class CTextFileLogComp: public CStreamLogCompBase
{
public:
	typedef CStreamLogCompBase BaseClass;

	I_BEGIN_COMPONENT(CTextFileLogComp);
		I_ASSIGN(m_fileNameCompPtr, "LogFile", "Name of the log file", true, "LogFile")
		I_ASSIGN(m_isAppendAttrPtr, "AppendToExisting", "Don't overwrite existing log file", false, true)
	I_END_COMPONENT;

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteText(const istd::CString& text);
	virtual void NewLine();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iprm::IFileNameParam, m_fileNameCompPtr);
	I_ATTR(bool, m_isAppendAttrPtr);

	std::wofstream m_outputFileStream;
};


} // namespace ibase


#endif // !ibase_CTextFileLogComp_included

