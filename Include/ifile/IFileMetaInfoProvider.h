#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/TSmartPtr.h>
#include <idoc/IDocumentMetaInfo.h>


namespace ifile
{


/**
	Interface for acquiring meta-information of a file.
*/
class IFileMetaInfoProvider: virtual public istd::IPolymorphic
{
public:
	typedef istd::TSmartPtr<idoc::IDocumentMetaInfo> MetaInfoPtr;

	/**
		Create meta information of an existing file.
		\param filePath	Path of the file.
		\return Pointer to the meta-info instance or \c NULL, if no information could be provided.
		\note The caller of this method is responsible for the memory management of the created meta-info object.
	*/
	virtual MetaInfoPtr GetFileMetaInfo(const QString& filePath) const = 0;
};


} // namespace ifile


