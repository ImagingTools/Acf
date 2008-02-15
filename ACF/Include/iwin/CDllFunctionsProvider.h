#ifndef iwin_CDllFunctionsProvider_included
#define iwin_CDllFunctionsProvider_included


#include "iwin/iwin.h"


#include <string>

#include <windows.h>

#include "isys/IFunctionsProvider.h"


namespace iwin
{


/**
	Provide access to public functions in DLL.
 */
class CDllFunctionsProvider: public isys::IFunctionsProvider
{
public:
	/**
		Construct this object opening specified DLL.
		\param	dllPath	path to DLL or empty string if no DLL should be opened.
	 */
	CDllFunctionsProvider(const ::std::string& dllPath = "");
	~CDllFunctionsProvider();

	bool OpenDll(const ::std::string& dllPath);

	void Reset();

	// reimplemented (isys::IFunctionsProvider)
	virtual bool IsValid() const;
	virtual void* GetFunction(const ::std::string& id) const;

private:
	HINSTANCE m_handler;
};


} // namespace iwin


#endif // !iwin_CDllFunctionsProvider_included


