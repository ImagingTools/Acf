#ifndef idoc_IMainWindow_included
#define idoc_IMainWindow_included


#include "istd/IPolymorphic.h"


namespace idoc
{


class IMainWindowCommands: virtual public istd::IPolymorphic
{
public:
	virtual bool OpenFile(const istd::CString& fileName) = 0;
};


} // namespace idoc


#endif // !idoc_IMainWindow_included


