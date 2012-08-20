#ifndef ibase_CQtVersionInfo_included
#define ibase_CQtVersionInfo_included


// ACF includes
#include "iser/IVersionInfo.h"
#include "ibase/TLoggerCompWrap.h"


namespace ibase
{


/**
	Component representation of the Qt module version.
*/
class CQtVersionInfoComp: 
			public iser::IVersionInfo,
			public ibase::CLoggerComponentBase
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CQtVersionInfoComp);
		I_REGISTER_INTERFACE(iser::IVersionInfo);
	I_END_COMPONENT;

	enum QtVersionIds
	{
		/**
			Version of Qt framework againts the actual module was compiled.
		*/
		QVI_COMPILED = IVersionInfo::UserVersionId + 1,

		/**
			Version of Qt framework registered in the current runtume system.
		*/
		QVI_RUNTIME
	};

	// reimplemented (iser::IVersionInfo)
	virtual VersionIds GetVersionIds() const;
	virtual bool GetVersionNumber(int versionId, quint32& result) const;
	virtual QString GetVersionIdDescription(int versionId) const;
	virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const;

protected:
	// reimplemented (ibase::CComponentBase)
	virtual void OnComponentCreated();
};


} // namespace ibase


#endif // !ibase_CQtVersionInfo_included


