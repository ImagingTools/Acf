#ifndef ibase_IFileConvertCopy_included
#define ibase_IFileConvertCopy_included


#include "istd/IPolymorphic.h"


class QString;


namespace iprm
{
	class IParamsSet;
}


namespace ibase
{


/**
	Common interface for file copying.
*/
class IFileConvertCopy: virtual public istd::IPolymorphic
{
public:
	enum MessageId
	{
		/**
			Message ID if input path is invalid
		*/
		MI_INPUT_PATH = 0x5cd40,
		/**
			Message ID if output path is invalid
		*/
		MI_OUTPUT_PATH
	};

	/**
		Copy a file or files from \c inputPath to destination \c outputPath.
		Optionally, you can use \c paramsPtr to control the copy process.
	*/
	virtual bool ConvertFiles(
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL) const = 0;
};


} // namespace ibase


#endif // !ibase_IFileConvertCopy_included


