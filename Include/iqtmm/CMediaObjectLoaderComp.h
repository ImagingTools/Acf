#ifndef iqtmm_CMediaObjectLoaderComp_included
#define iqtmm_CMediaObjectLoaderComp_included


// ACF includes
#include "iqtmm/iqtmm.h"

#include "iser/IFileLoader.h"

#include "ibase/TLoggerCompWrap.h"

#include "icomp/CComponentBase.h"


namespace iqtmm
{


class CMediaObjectLoaderComp: public ibase::TLoggerCompWrap<icomp::CComponentBase>, virtual public iser::IFileLoader
{
public:
	typedef ibase::TLoggerCompWrap<icomp::CComponentBase> BaseClass;

	I_BEGIN_COMPONENT(CMediaObjectLoaderComp)
		I_REGISTER_INTERFACE(iser::IFileLoader);
	I_END_COMPONENT

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xa7d0,
		MI_FILE_NOT_EXIST,
		MI_BAD_FORMAT
	};

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const;
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = 0, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;
};


} // namespace iqtmm


#endif // !iqtmm_CMediaObjectLoaderComp_included

