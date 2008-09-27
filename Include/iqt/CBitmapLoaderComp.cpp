#include "iqt/CBitmapLoaderComp.h"


// Qt includes
#include <QFileInfo>
#include <QByteArray>
#include <QImageReader>

#include "istd/TChangeNotifier.h"

#include "iqt/IQImageProvider.h"


namespace iqt
{


void CBitmapLoaderComp::SetLastLoadFileName(const istd::CString& fileName)
{
	m_lastLoadFileName = fileName;
}


void CBitmapLoaderComp::SetLastSaveFileName(const istd::CString& fileName)
{
	m_lastSaveFileName = fileName;
}


// reimplemented (iser::IFileLoader)

bool CBitmapLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			bool forLoading,
			bool forSaving) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iqt::IQImageProvider*>(dataObjectPtr) == NULL)){
		return false;
	}

	if (filePathPtr != NULL){
		QFileInfo info(iqt::GetQString(*filePathPtr));
		if (forLoading && !forSaving){
			if (!info.exists()){
				return false;
			}

			QByteArray format = QImageReader::imageFormat(info.filePath());

			return !format.isEmpty();
		}
	}

	return true;
}


int CBitmapLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	iqt::IQImageProvider* bitmapPtr = dynamic_cast<iqt::IQImageProvider*>(&data);

	if (bitmapPtr != NULL){
		istd::CChangeNotifier notifier(&data);

		const istd::CString& usedFilePath = (!filePath.IsEmpty() || !m_defaultFilePathAttrPtr.IsValid())?
					filePath:
					*m_defaultFilePathAttrPtr;

		QImage image;
		if (image.load(iqt::GetQString(usedFilePath))){
			const_cast<CBitmapLoaderComp*>(this)->SetLastLoadFileName(filePath);

			bitmapPtr->CopyImageFrom(image);

			return StateOk;
		}
	}

	return StateFailed;
}


int CBitmapLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	const iqt::IQImageProvider* bitmapPtr = dynamic_cast<const iqt::IQImageProvider*>(&data);

	if (bitmapPtr != NULL){
		const QImage& image = bitmapPtr->GetQImage();
		if (image.save(iqt::GetQString(filePath))){
			const_cast<CBitmapLoaderComp*>(this)->SetLastSaveFileName(filePath);

			return StateOk;
		}
	}

	return StateFailed;
}


const istd::CString& CBitmapLoaderComp::GetLastLoadFileName() const
{
	return m_lastLoadFileName;
}


const istd::CString& CBitmapLoaderComp::GetLastSaveFileName() const
{
	return m_lastSaveFileName;
}


bool CBitmapLoaderComp::GetFileExtensions(istd::CStringList& result, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	int extensionsCount = m_fileExtensionsAttrPtr.GetCount();

	for (int i = 0; i < extensionsCount; ++i){
		const istd::CString& extension = m_fileExtensionsAttrPtr[i];

		result.push_back(extension);
	}

	return true;
}


} // namespace iqt


