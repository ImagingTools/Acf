#ifndef CRegistryLoaderComp_included
#define CRegistryLoaderComp_included


#include "BasePck/BasePck.h"


/**
	Special registry loader supporting of loading layout data.
*/
class CRegistryLoaderComp: public BasePck::XmlFileSerializer
{
public:
	typedef BasePck::XmlFileSerializer BaseClass;

	enum MessageId
	{
		MI_LOAD_ERROR = 680,
		MI_CANNOT_READ_LAYOUT
	};

	// reimplemented (iser::IFileLoader)
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const;
	virtual bool GetFileExtensions(istd::CStringList& result, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

protected:
	istd::CString GetLayoutPath(const istd::CString& registryPath) const;

	// reimplemented (ibase::TFileSerializerComp)
	virtual void OnReadError(
				const iser::CXmlFileReadArchive& archive,
				const istd::IChangeable& data,
				const istd::CString& filePath) const;
};


#endif // !CRegistryLoaderComp_included


