#ifndef icmpstr_CComponentHelpFileProvider_included
#define icmpstr_CComponentHelpFileProvider_included


#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/CComponentBase.h"
#include "icomp/CComponentAddress.h"

#include "idoc/IHelpFileProvider.h"


namespace icmpstr
{


class CComponentHelpFileProvider:
			public icomp::CComponentBase,
			virtual public idoc::IHelpFileProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CComponentHelpFileProvider);
		I_REGISTER_INTERFACE(idoc::IHelpFileProvider);
		I_ASSIGN(m_classHelpProviderCompPtr, "ClassHelpProvider", "Slave help file provider for class info objects", true, "ClassHelpProvider");
		I_ASSIGN(m_packagesLoaderInfoCompPtr, "PackagesLoader", "Packages loader used to get component info", true, "PackagesLoader");
		I_ASSIGN(m_packagesLoaderManagerCompPtr, "PackagesLoader", "Packages loader used to get component info", true, "PackagesLoader");
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpFileProvider)
	virtual double GetHelpQuality(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const;
	virtual istd::CString GetHelpFilePath(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const;

protected:
	virtual istd::CString GetInfoFilePath(const icomp::CComponentAddress& componentAddress) const;
	virtual istd::CString GetSlaveFilePath(const icomp::CComponentAddress& componentAddress) const;

	bool ExtractComponentAddress(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr, icomp::CComponentAddress& result) const;

private:
	I_REF(idoc::IHelpFileProvider, m_classHelpProviderCompPtr);
	I_REF(icomp::IComponentStaticInfo, m_packagesLoaderInfoCompPtr);
	I_REF(icomp::IRegistriesManager, m_packagesLoaderManagerCompPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CComponentHelpFileProvider_included


