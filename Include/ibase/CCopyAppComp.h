#ifndef ibase_CCopyAppComp_included
#define ibase_CCopyAppComp_included


#include "istd/IChangeable.h"

#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/IApplication.h"
#include "ibase/IFileConvertCopy.h"


namespace ibase
{


/**
	General file copy or transformation.
	This implementation uses general file loader, saver and any object instance.
	It implements ibase::IApplication and can be also used as standalone application.
*/
class CCopyAppComp:
			public icomp::CComponentBase,
			virtual public IApplication,
			virtual public IFileConvertCopy
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCopyAppComp);
		I_REGISTER_INTERFACE(IApplication);
		I_ASSIGN(m_objectCompPtr, "Object", "Object used as representation of copied data", true, "Object");
		I_ASSIGN(m_inputLoaderCompPtr, "InputLoader", "input file loader", true, "InputLoader");
		I_ASSIGN(m_outputLoaderCompPtr, "OutputLoader", "output file loader", true, "OutputLoader");
	I_END_COMPONENT;

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual istd::CString GetHelpText() const;

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool CopyFile(const istd::CString& inputFilePath, const istd::CString& outputFilePath) const;

private:
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(iser::IFileLoader, m_inputLoaderCompPtr);
	I_REF(iser::IFileLoader, m_outputLoaderCompPtr);
};


} // namespace ibase


#endif // !ibase_CCopyAppComp_included


