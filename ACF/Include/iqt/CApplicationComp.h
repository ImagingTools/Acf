#ifndef iqt_CApplicationComp_included
#define iqt_CApplicationComp_included


#include "ibase/IApplication.h"

#include "icomp/CComponentBase.h"

#include "iqt/IGuiObject.h"


namespace iqt
{


class CApplicationComp:
			public icomp::CComponentBase, 
			public ibase::IApplication
{
public:
	typedef icomp::CComponentBase BaseClass;
	I_COMPONENT(CApplicationComp);

	CApplicationComp(const icomp::IComponentContext* contextPtr);

	// reimplemented (ibase::IApplication)
	virtual int Execute(int argc, char** argv);
	virtual istd::CString GetHelpText() const;

protected:
	I_REF(IGuiObject, m_mainGuiCompPtr);
	I_REF(IGuiObject, m_splashScreenCompPtr);
};


} // namespace iqt


#endif // iqt_CApplicationComp_included

