#ifndef ifile_CRelativeFileNameParamComp_included
#define ifile_CRelativeFileNameParamComp_included


#include "ifile/CFileNameParamComp.h"

#include "ifile/ifile.h"


namespace ifile
{


class CRelativeFileNameParamComp: public ifile::CFileNameParamComp
{
public:
	typedef ifile::CFileNameParamComp BaseClass;

	I_BEGIN_COMPONENT(CRelativeFileNameParamComp);
		I_ASSIGN(m_relativeToCompPtr, "ReferencePath", "Reference path, relative to this file or directory internal path representation will be calculated", false, "ReferencePath");
	I_END_COMPONENT;

	// reimplemented (ifile::IFileNameParam)
	virtual const QString& GetPath() const;
	virtual void SetPath(const QString& path);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	I_REF(ifile::IFileNameParam, m_relativeToCompPtr);

	mutable QString m_relativePath;
};


} // namespace ifile


#endif // !ifile_CRelativeFileNameParamComp_included


