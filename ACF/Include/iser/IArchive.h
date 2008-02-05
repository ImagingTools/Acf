#ifndef INCLUDED_iser_IArchive_h
#define INCLUDED_iser_IArchive_h


#include <string>


#include "istd/IPolymorphic.h"


namespace iser
{


class CArchiveTag;


/**
	Represent input/output persistence archive.
*/
class IArchive: public IPolymorphic
{
public:
	/**
		Type of version.
		This is needed for the sake of supporting mutliply separated versions for separated modules.
		Whole ACF framework lay under single versioning mechanism and has one version number identified by VT_FRAMEWORK type ID.
		User version type ID should be greater or equal VT_USER_APPLICATION value.
		As a default version type ID for user applications VT_USER_APPLICATION should be used.
	*/
	enum VersioningType
	{
		VT_FRAMEWORK,
		VT_USER_APPLICATION = 1024
	};

	enum{
		/**
			Biggest version value used if no version is needed or known.
		*/
		UNKNOWN_VERSION = 0xffffffff
	};

	/**	Check if this archive is loading or storing.
	 */
	virtual bool IsStoring() const = 0;
	/**	Get version of archived stream for specific versioning type.
	 *		@param	versioningType	versioning type. It allows you to distinguish different version aspects.
	 *								For example version number of framework cann differ from application version.
	 *		@sa VersioningType.
	 */
	virtual I_DWORD GetVersion(int versioningType = VT_USER_APPLICATION) const = 0;
	/**	Begin of archive tag.
	 *		@param	tag				tag object.
	 *		@param	skipFlagPtr		pointer to optional flag allowing to manage skipped tags.
	 *								If this pointer is not NULL, information will be returned if tag is present in archive.
	 *								Please note, that application have to manage of tag element skipping.
	 *								Checking of tag existence can be done static, using archive version only
	 *								(e.g. for binary archives) or dynamic, using information contained in archive.
	 *								Plase note, that if tag is not present and isOptionalPtr is not NULL, this method returns true,
	 *								flag pointer by skipFlagPtr is set to true. In this case user have to skip
	 *								its internal tag archive part and call EndTag method.
	 */
	virtual bool BeginTag(const CArchiveTag& tag, bool* skipFlagPtr = NULL) = 0;
	/**	End of archive tag.
	 *		This method should be allways called after BeginTag is successfull called.
	 */
	virtual bool EndTag(const CArchiveTag& tag)  = 0;
	/**	Process primitive type.
	 */
	virtual bool Process(char& value) = 0;
	/**	Process primitive type.
	 */
	virtual bool Process(bool& value) = 0;
	/**	Process primitive type.
	 */
	virtual bool Process(int& value) = 0;
	/**	Process primitive type.
	 */
	virtual bool Process(double& value) = 0;
	/**	Process primitive type.
	 */
	virtual bool Process(short& value) = 0;
	/**	Process primitive type.
	 */
	virtual bool Process(float& value) = 0;
	/**	Process primitive type.
	 */
	virtual bool Process(std::string& value) = 0;
	/**	Process primitive type.
	 */
	virtual bool Process(CString& value) = 0;
	/**	Process binary data block.
	 *		@param	dataPtr	pointer to memory block.
	 *		@param	size	size of memory block in bytes.
	 */
	virtual bool ProcessData(void* dataPtr, int size) = 0;
};


} // namespace iser


#endif // INCLUDED_iser_IArchive_h

