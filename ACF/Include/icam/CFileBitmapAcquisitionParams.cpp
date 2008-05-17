#include "icam/CFileBitmapAcquisitionParams.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icam
{


// reimplemented (icam::IFileBitmapAcquisitionParams)

istd::CString CFileBitmapAcquisitionParams::GetDirectory() const
{
	return m_directory;
}


// reimplemented (iser::ISerializable)

bool CFileBitmapAcquisitionParams::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag directoryTag("Directory", "File directory of images");
	retVal = retVal && archive.BeginTag(directoryTag);
	retVal = retVal && archive.Process(m_directory);
	retVal = retVal && archive.EndTag(directoryTag);

	return retVal;
}


} // namespace icam


