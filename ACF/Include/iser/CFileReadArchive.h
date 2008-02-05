#ifndef iser_CFileReadArchive_h_included
#define iser_CFileReadArchive_h_included


#include "binaryreadarchive.h"
#include <fstream>


namespace iser
{


class CFileReadArchive: public CBinaryReadArchiveBase
{
public:
	CFileReadArchive(const CString& fileName);
	~CFileReadArchive();

	// reimplemented (CBinaryReadArchiveBase)
	virtual int tagSize();

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* data, int size);
	
protected:
	std::ifstream m_stream;
};


} // namespace iser


#endif // iser_CFileReadArchive_h_included

