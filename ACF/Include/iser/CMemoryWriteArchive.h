#ifndef iser_CMemoryWriteArchive_included
#define iser_CMemoryWriteArchive_included


#include <vector>

#include "iser/CBinaryWriteArchiveBase.h"


namespace iser
{


class CMemoryWriteArchive: public CBinaryWriteArchiveBase
{
public:
	CMemoryWriteArchive();
	~CMemoryWriteArchive();

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* data, int size);
	
protected:
	::std::vector<unsigned char> m_memory;
};


} // namespace iser


#endif // iser_CMemoryWriteArchive_included


