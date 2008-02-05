#ifndef iser_CMemoryWriteArchive_h_included
#define iser_CMemoryWriteArchive_h_included


#include "binarywritearchive.h"
#include <fstream>


namespace iser
{


class CMemoryWriteArchive: public CBinaryWriteArchiveBase
{
public:
	CMemoryWriteArchive();
	~CMemoryWriteArchive();

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* data, int size);
	
	// reimplemented (CBinaryWriteArchiveBase)
	virtual void Flush();

protected:
	std::vector<unsigned char> m_memory;
};


} // namespace iser


#endif // iser_CMemoryWriteArchive_h_included


