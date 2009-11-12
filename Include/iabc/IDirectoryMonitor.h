#ifndef iabc_IDirectoryMonitor_included
#define iabc_IDirectoryMonitor_included


#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "ibase/IFileListProvider.h"


namespace iabc
{


/**
	Interface for a folder monitor.
	Observers of this interface are notified about all changes made in all monitored directories, 
	such as removing or adding files, changes in file contents or file attributes.
*/
class IDirectoryMonitor: virtual public ibase::IFileListProvider
{
public:
	enum ChangeFlags
	{
		FilesAdded = 0x1000,
		FilesRemoved = 0x2000,
		FilesModified = 0x4000,
		FilesAttributeChanged = 0x8000
	};

	/**
		Get the list of changed files according to the given flags
		\sa ChangeFlags
	*/
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const = 0;
};


} // namespace iabc


#endif // !iabc_IDirectoryMonitor_included


