#include "ibase/CRenderedObjectFileLoaderComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace ibase
{


// public methods

// reimplemented (iser::IFileLoader)

bool CRenderedObjectFileLoaderComp::IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr,
				int flags,
				bool beQuiet) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->IsOperationSupported(dataObjectPtr, filePathPtr, flags, beQuiet);
	}

	return false;
}


int CRenderedObjectFileLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	if (m_fileLoaderCompPtr.IsValid() && m_fileDataCompPtr.IsValid() && m_objectSnapCompPtr.IsValid()){
		int loadResult = m_fileLoaderCompPtr->LoadFromFile(*m_fileDataCompPtr.GetPtr(), filePath);
		if (loadResult == StateOk){
			iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(&data);
			if (outputBitmapPtr == NULL){
				return StateFailed;
			}

			istd::CChangeNotifier changePtr(outputBitmapPtr);

			if (m_objectSnapCompPtr->GetSnap(*m_fileDataCompPtr.GetPtr(), *outputBitmapPtr, istd::CIndex2d(*m_widthAttrPtr, *m_heightAttrPtr))){
				return StateOk;
			}
		}
	}

	return StateFailed;
}


int CRenderedObjectFileLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->SaveToFile(data, filePath);
	}

	return iser::IFileLoader::StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CRenderedObjectFileLoaderComp::GetFileExtensions(istd::CStringList& result, int flags, bool doAppend) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->GetFileExtensions(result, flags, doAppend);
	}

	return false;
}

istd::CString CRenderedObjectFileLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->GetTypeDescription(extensionPtr);
	}

	return istd::CString::GetEmpty();
}


} // namespace ibase


