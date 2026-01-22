// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/ILogger.h>

#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace iser
{


/**
	Base implementation of iser::IArchive interface.
	It provides standard implementation of some methods and standard tags.
*/
class CArchiveBase:
			virtual public IArchive,
			virtual protected istd::ILogger
{
public:
	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const override;
	virtual bool BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count) override;

	static const CArchiveTag& GetAcfRootTag();

protected:
	CArchiveBase(){}

	// list of standard tags used to serialize header
	static CArchiveTag s_acfRootTag;

private:
	/**
		Block of copy contructor for all derrived classes.
	*/
	CArchiveBase(const CArchiveBase&);
};


} // namespace iser




