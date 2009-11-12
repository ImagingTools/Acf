#ifndef iabc_IDirectoryMonitorParams_included
#define iabc_IDirectoryMonitorParams_included


// Qt includes
#include <QDir>


// ACF includes
#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace iabc
{


/**
	Interface for a folder monitor parameters.
*/
class IDirectoryMonitorParams: virtual public iser::ISerializable
{
public:
	enum ObserveItems
	{
		OI_DIR = QDir::Dirs,
		OI_FILES = QDir::Files,
		OI_DRIVES = QDir::Drives,
		OI_ALL = QDir::AllEntries
	};

	enum ObserveChanges
	{
		OC_ADD = 0x1,
		OC_REMOVE = 0x2,
		OC_MODIFIED = 0x4,
		OC_ATTR_CHANGED = 0x8,
		OC_ALL = OC_ADD | OC_REMOVE | OC_MODIFIED | OC_ATTR_CHANGED
	};

	/**
		Get notification intervall in seconds.	
	*/
	virtual double GetNotificationIntervall() const = 0;

	/**
		Get pooling intervall for directory changes in seconds.	
	*/
	virtual double GetPoolingIntervall() const = 0;

	/**
		Get the group of items to be observed.
	*/
	virtual int GetObservedItemTypes() const = 0;

	/**
		Get the group of changes to be observed.
	*/
	virtual int GetObservedChanges() const = 0;

	/**
		Get file filters, given as the list of regular expressions.
	*/
	virtual istd::CStringList GetFileFilters() const = 0;
};


} // namespace iabc


#endif // !iabc_IDirectoryMonitorParams_included


