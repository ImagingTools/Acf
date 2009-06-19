#ifndef isys_IFolderMonitor_included
#define isys_IFolderMonitor_included


#include "isys/isys.h"


#include "istd/IChangeable.h"



namespace isys
{


/**
	Interface for a folder monitor.
	Observers of this interface are notified about all changes made in the monitored folder such as removing or adding files,
	changes in file contents or in file attributes.
*/
class IFolderMonitor: virtual public istd::IChangeable
{
public:
	enum FolderChangeFlags
	{
		FilesAdded = 0x4,
		FilesRemoved = 0x8,
		FilesModified = 0x10,
		FilesAttributeChanged = 0x20
	};

	/**
		Set the path for the folder to be monitored.
	*/
	virtual void SetFolderPath(const istd::CString& folderPath) = 0;

	/**
		Get the observed folder path.
	*/
	virtual istd::CString GetFolderPath() const = 0;

	/**
		Get the list of changed files according to the given flags
		\sa FolderChangeFlags
	*/
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const = 0;
};


} // namespace isys


#endif // !isys_IFolderMonitor_included
