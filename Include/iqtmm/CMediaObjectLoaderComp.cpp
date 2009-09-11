#include "iqtmm/CMediaObjectLoaderComp.h"


// Qt includes
#include <Phonon>
#include <QFileInfo>


// ACF includes
#include "iqtmm/IMediaObject.h"

#include "istd/TChangeNotifier.h"


namespace iqtmm
{


// public methods


// reimplemented (iser::IFileLoader)

bool CMediaObjectLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iqtmm::IMediaObject*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, iqt::GetCString(QObject::tr("Object is not a valid")));
		}

		return false;
	}

	if (filePathPtr != NULL){
		QString qtFilePath = iqt::GetQString(*filePathPtr);
		QFileInfo info(qtFilePath);

		istd::CStringList extensions;
		if (GetFileExtensions(extensions, flags)){
			QStringList extensionsList = iqt::GetQStringList(extensions);

			if (!extensionsList.contains(info.suffix(), Qt::CaseInsensitive)){
				if (!beQuiet){
					SendInfoMessage(MI_BAD_EXTENSION, iqt::GetCString(QObject::tr("Bad multimedia file extension %1").arg(info.suffix())));
				}

				return false;
			}
		}

		if ((flags & QF_NO_SAVING) != 0){
			return false;
		}
	}

	return ((flags & QF_ANONYMOUS_ONLY) == 0);
}


int CMediaObjectLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	iqtmm::IMediaObject* mediaObjectPtr = dynamic_cast<iqtmm::IMediaObject*>(&data);
	if (mediaObjectPtr != NULL){
		istd::CChangeNotifier notifier(&data);

		QString qtFilePath = iqt::GetQString(filePath);

		mediaObjectPtr->SetMediumUrl(iqt::GetQString(filePath));
			
		return StateOk;
	}
	else{
		SendInfoMessage(MI_BAD_OBJECT_TYPE, iqt::GetCString(QObject::tr("Wrong object type")));
	}

	return StateFailed;
}


int CMediaObjectLoaderComp::SaveToFile(const istd::IChangeable&/* data*/, const istd::CString&/* filePath*/) const
{
	return StateFailed;
}


bool CMediaObjectLoaderComp::GetFileExtensions(istd::CStringList& result, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	result.push_back("mov");
	result.push_back("wmv");
	result.push_back("avi");

	return true;
}


istd::CString CMediaObjectLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if (		(extensionPtr == NULL) ||
				extensionPtr->IsEqualNoCase("mov") ||
				extensionPtr->IsEqualNoCase("wmv") ||
				extensionPtr->IsEqualNoCase("avi")){
		return iqt::GetCString(QObject::tr("Multimedia"));
	}

	return "";
}


} // namespace iqtmm


