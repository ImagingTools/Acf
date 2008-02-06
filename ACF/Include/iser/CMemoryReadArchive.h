#ifndef iser_CMemoryReadArchive_included
#define iser_CMemoryReadArchive_included


#include <vector>

#include "iser/CBinaryReadArchiveBase.h"


namespace iser
{


class CMemoryWriteArchive;


class CMemoryReadArchive: public CBinaryReadArchiveBase
{
public:
	typedef CBinaryReadArchiveBase BaseClass;

	CMemoryReadArchive(
				const void* bufferPtr,
				int bufferSize,
				bool serializeHeader = true);
	CMemoryReadArchive(
				const CMemoryWriteArchive& writeArchive,
				bool serializeHeader = true);

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* data, int size);
	
protected:
	const I_BYTE* m_bufferPtr;
	int m_bufferSize;
	int m_readPosition;
};


} // namespace iser


#endif // iser_CMemoryReadArchive_included


