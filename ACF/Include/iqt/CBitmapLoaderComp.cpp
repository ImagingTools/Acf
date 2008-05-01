#include "iqt/CBitmapLoaderComp.h"


// Qt includes
#include <QFileInfo>
#include <QByteArray>
#include <QImageReader>

#include "istd/TChangeNotifier.h"

#include "iqt/IQImageProvider.h"


namespace iqt
{


// reimplemented (iser::IFileLoader)

bool CBitmapLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			bool forLoading,
			bool forSaving) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const IQImageProvider*>(dataObjectPtr) == NULL)){
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
	IQImageProvider* bitmapPtr = dynamic_cast<IQImageProvider*>(&data);

	if (bitmapPtr != NULL){
		istd::CChangeNotifier notifier(&data);

		QImage image;
		if (image.load(iqt::GetQString(filePath))){
			const_cast<CBitmapLoaderComp*>(this)->SetLastLoadFileName(filePath);

			bitmapPtr->CopyImageFrom(image);

			return StateOk;
		}
	}

	return StateFailed;
}


int CBitmapLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	const IQImageProvider* bitmapPtr = dynamic_cast<const IQImageProvider*>(&data);

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


} // namespace iqt


