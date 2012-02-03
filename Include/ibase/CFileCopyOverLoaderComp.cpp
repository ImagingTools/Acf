#include "ibase/CFileCopyOverLoaderComp.h"


namespace ibase
{


// reimplemented (ibase::IFileConvertCopy)

bool CFileCopyOverLoaderComp::ConvertFile(
			const istd::CString& inputFilePath,
			const istd::CString& outputFilePath,
			const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	if (!m_inputLoaderCompPtr.IsValid()){
		SendErrorMessage(0, "Input data loader is not defined", "FileCopyOverLoader");

		return false;
	}

	if (!m_outputLoaderCompPtr.IsValid()){
		SendErrorMessage(0, "Output data loader is not defined", "FileCopyOverLoader");

		return false;
	}

	if (!m_objectCompPtr.IsValid()){
		SendErrorMessage(0, "Data object for copy operaration is not set", "FileCopyOverLoader");

		return false;
	}

	istd::CString usedOutputPath = outputFilePath;

	if (usedOutputPath.IsEmpty()){
		istd::CStringList extensions;
		m_outputLoaderCompPtr->GetFileExtensions(extensions);

		if (extensions.empty()){
			SendErrorMessage(0, "File extension list is empty", "FileCopyOverLoader");
	
			return false;
		}

		istd::CString::size_type pointPos = inputFilePath.rfind('.');

		if (pointPos != istd::CString::npos){
			usedOutputPath = inputFilePath.substr(0, pointPos + 1) + extensions.front();
		}
		else{
			usedOutputPath = inputFilePath + extensions.front();
		}
	}

	int loadState = m_inputLoaderCompPtr->LoadFromFile(*m_objectCompPtr, inputFilePath);
	if (loadState != iser::IFileLoader::StateOk){
		SendErrorMessage(0, "Data could not be loaded", "FileCopyOverLoader");

		return false;
	}

	int saveState = m_outputLoaderCompPtr->SaveToFile(*m_objectCompPtr, usedOutputPath);
	if (saveState != iser::IFileLoader::StateOk){
		SendErrorMessage(0, "Data could not be saved", "FileCopyOverLoader");

		return false;
	}

	return true;
}


} // namespace ibase


