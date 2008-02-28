#ifndef iser_CMemoryWriteArchive_included
#define iser_CMemoryWriteArchive_included


#include <vector>

#include "iser/CBinaryWriteArchiveBase.h"


namespace iser
{


/**
	Implementation of archive using memory buffer to store state of persistent objects.
	Internal format of this buffer is compatible with class \c iser::CMemoryReadArchive.
*/
class CMemoryWriteArchive: public CBinaryWriteArchiveBase
{
public:
	typedef CBinaryWriteArchiveBase BaseClass;

	CMemoryWriteArchive(
				const IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true);

	const void* GetBuffer() const;
	int GetBufferSize() const;

	/**
		Reset internal buffer.
	*/
	void Reset();

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* data, int size);
	
protected:
	typedef ::std::vector<unsigned char> DataBuffer;

	DataBuffer m_dataBuffer;

	bool m_serializeHeader;
};


} // namespace iser


#endif // iser_CMemoryWriteArchive_included


