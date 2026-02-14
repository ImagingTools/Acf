// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QVector>
#include <QtCore/QFile>

// ACF includes
#include <iser/CBinaryReadArchiveBase.h>
#include <ifile/CFileArchiveInfo.h>


namespace ifile
{


/**
	Simple implementation of archive reading from own ACF format binary file.
	This imlementation is very fast and efficient and should be used if any standardized file format is needed.

	\ingroup Persistence
*/
class CFileReadArchive:
			public iser::CBinaryReadArchiveBase,
			public CFileArchiveInfo
{
public:
	typedef iser::CBinaryReadArchiveBase BaseClass;
	typedef CFileArchiveInfo BaseClass2;

	/**
		Contructor.
		\param	filePath			name of file.
		\param	supportTagSkipping	if it is true skipping of tags on EndTag is supported.
									Please note that supporting of tag skipping store additional data in file
									and it is not compatible with files created without tag skipping.
									\sa	EndTag and IsTagSkippingSupported.
		\param	serializeHeader		if it is true (default) archive header will be serialized.
	*/
	CFileReadArchive(const QString& filePath, bool supportTagSkipping = true, bool serializeHeader = true);

	/**
		Constructor for device-based reading.
		\param	device				QIODevice to read from (must be open for reading).
		\param	supportTagSkipping	if it is true skipping of tags on EndTag is supported.
		\param	serializeHeader		if it is true (default) archive header will be serialized.
	*/
	CFileReadArchive(QIODevice& device, bool supportTagSkipping = true, bool serializeHeader = true);

	// reimplemented (ifile::IArchive)
	virtual bool IsTagSkippingSupported() const override;
	virtual bool BeginTag(const iser::CArchiveTag& tag) override;
	virtual bool EndTag(const iser::CArchiveTag& tag) override;
	virtual bool ProcessData(void* data, int size) override;
	
protected:
	struct TagStackElement
	{
		quint32 tagBinaryId;
		quint32 endPosition;
		bool useTagSkipping;
	};

	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const override;

	// reimplemented (iser::CArchiveBase)
	virtual int GetMaxStringLength() const override;

private:
	QIODevice& GetDevice();
	const QIODevice& GetDevice() const;

	QFile m_file;
	QIODevice* m_devicePtr;  // Non-null when using external device, null when using m_file

	bool m_supportTagSkipping;

	typedef QVector<TagStackElement> TagStack;

	TagStack m_tagStack;
};


} // namespace ifile


