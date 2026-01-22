// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QTextStream>

// ACF includes
#include <iser/CXmlReadArchiveBase.h>


namespace iser
{


/**
	Base implementation of a reading archive for XML format using a stream implementation.
*/
class CXmlStreamReadArchiveBase: public CXmlReadArchiveBase
{
public:
	typedef CXmlReadArchiveBase BaseClass;

	int GetLastReadLine() const;

protected:
	explicit CXmlStreamReadArchiveBase(const CArchiveTag& rootTag = s_acfRootTag);

	// reimplemented (iser::CXmlReadArchiveBase)
	virtual bool ReadToDelimeter(
				const QByteArray& delimeters,
				QByteArray& result,
				bool skipDelimeter = true,
				char* foundDelimeterPtr = NULL) override;

	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const override;

	QTextStream m_stream;

private:
	char m_lastReadChar;
	bool m_useLastReadChar;

	int m_lastReadLine;
};


} // namespace iser




