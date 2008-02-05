#ifndef iser_CFileWriteArchive_h_included
#define iser_CFileWriteArchive_h_included


#include "binarywritearchive.h"
#include <fstream>


namespace iser
{


class CFileWriteArchive: public CBinaryWriteArchiveBase
{
public:
	CFileWriteArchive(const CString& fileName);
	~CFileWriteArchive();

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* data, int size);
	
	// reimplemented (CBinaryWriteArchiveBase)
	virtual void Flush();

protected:
	std::ofstream m_stream;
};


} // namespace iser


#endif // iser_CFileWriteArchive_h_included

