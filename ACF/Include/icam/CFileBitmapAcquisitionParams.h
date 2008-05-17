#ifndef icam_CFileBitmapAcquisitionParams_included
#define icam_CFileBitmapAcquisitionParams_included


#include "icam/IFileBitmapAcquisitionParams.h"


namespace icam
{


class CFileBitmapAcquisitionParams: public IFileBitmapAcquisitionParams
{
public:
	// reimplemented (icam::IFileBitmapAcquisitionParams)
	virtual istd::CString GetDirectory() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_directory;
};


} // namespace icam


#endif // !icam_CFileBitmapAcquisitionParams_included


