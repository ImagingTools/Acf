// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <iser/CArchiveBase.h>
#include <iser/CArchiveHeaderInfo.h>


namespace iser
{


/**
	Basic implementation of an abstract archive for data reading.
*/
class CReadArchiveBase: public CArchiveBase
{
public:
	// reimplemented (iser::IArchive)
	virtual bool IsStoring() const override;
	virtual const IVersionInfo& GetVersionInfo() const override;
	virtual bool ProcessBits(void* dataPtr, int bitsCount, int bytesCount) override;

protected:
	/**
		Serialize standard header.
		During serialization of header list of known versions will be loaded.
	*/
	bool SerializeAcfHeader();

private:
	CArchiveHeaderInfo m_versionInfo;
};


} // namespace iser




