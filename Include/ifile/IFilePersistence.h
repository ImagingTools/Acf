#ifndef ifile_IFileLoader_included
#define ifile_IFileLoader_included


// Qt includes
#include <QtCore/QString>


// ACF includes
#include "istd/IChangeable.h"

#include "ifile/IFileTypeInfo.h"


namespace ifile
{


/**
	Provide loading and saving of objects.

	\ingroup Persistence
*/
class IFilePersistence: virtual public IFileTypeInfo
{
public:
	/**
		Result of operation.
	*/
	enum OperationState
	{
		/**
			Operation was successfull.
		*/
		StateOk,
		/**
			Operation was aborted by user.
		*/
		StateAborted,
		/**
			Operation failed.
		*/
		StateFailed
	};

	/**
		Possible file I/O errors.
	*/
	enum ErrorType
	{
		/**
			File could not be written, no write access granted.
		*/
		ET_NO_WRITE_PERMISSIONS,

		/**
			File could not be read, no read access granted.
		*/
		ET_NO_READ_PERMISSIONS,

		/**
			File doesn't exist.
		*/
		ET_FILE_NOT_EXIST
	};

	/**
		List of possible message ids used in context of this interface.
	*/
	enum MessageId
	{
		MI_BAD_OBJECT_TYPE = 0xabf0,
		MI_CANNOT_LOAD,
		MI_CANNOT_SAVE
	};

	/**
		Returns \c true if object \c dataObject can be loaded/saved.
		\param	dataObjectPtr	optional pointer to data object should be loaded/stored.
								It can be NULL if any object is meant.
		\param	filePathPtr		optional pointer to file should be loaded/stored.
								It can be NULL if any file is meant.
								If it points at empty string, anonymous loading is mean.
		\param	flags			combination of flags defined in \c QueryFlags and \c ifile::IFileTypeInfo::QueryFlags.
		\param	beQuiet			if true, no user message output is allowed.
	*/
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const = 0;

	/**
		This function loads data \c data from file \c filePath
		\returns File loading state. \sa OperationState
	*/
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath = QString()) const = 0;

	/**
		This function saves data \c data to file \c filePath
		\returns File saving state. \sa OperationState
	*/
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath = QString()) const = 0;
};


} // namespace ifile


#endif // !ifile_IFileLoader_included


