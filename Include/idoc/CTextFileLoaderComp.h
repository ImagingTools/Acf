// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <ifile/IFilePersistence.h>
#include <icomp/CComponentBase.h>


namespace idoc
{


/**
	Persistence component for files containing plain text.
	
	This component handles loading and saving of plain text files. It implements
	the IFilePersistence interface to integrate with the ACF file I/O framework
	and IFileTypeInfo to provide file type metadata.
	
	The loader supports multiple file extensions and can provide different type
	descriptions for each extension.
	
	\par Component Attributes
	- \b FileExtensions - List of supported file extensions (default: "txt")
	- \b TypeDescriptions - List of human-readable descriptions for each extension (default: "Text file")
	
	\par Registered Interfaces
	- ifile::IFileTypeInfo - Provides file type metadata
	- ifile::IFilePersistence - Handles file loading/saving operations
	
	\par Configuration Example
	\code
	CTextFileLoaderComp {
		FileExtensions = ["txt", "log", "md"]
		TypeDescriptions = ["Text file", "Log file", "Markdown file"]
	}
	\endcode
	
	\par Usage with Document Template
	\code
	CSingleDocumentTemplateComp {
		DocumentTypeId = "TextDocument"
		DocumentFactory = CTextDocumentComp
		DocumentLoader = CTextFileLoaderComp {
			FileExtensions = ["txt", "text"]
			TypeDescriptions = ["Text Document", "Plain Text"]
		}
	}
	\endcode
	
	The component automatically handles:
	- Reading text files with various encodings
	- Writing text files
	- File type validation
	- File dialog filters
	
	\sa CTextDocumentComp, ITextDocument
	\ingroup DocumentBasedFramework
*/
class CTextFileLoaderComp: public icomp::CComponentBase, public ifile::IFilePersistence
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTextFileLoaderComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN_MULTI_1(m_fileExtensionsAttrPtr, "FileExtensions", "List of possible file extensions", true, "txt");
		I_ASSIGN_MULTI_1(m_typeDescriptionsAttrPtr, "TypeDescriptions", "List of descriptions for each extension", true, "Text file");
	I_END_COMPONENT;

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const override;
	virtual ifile::IFilePersistence::OperationState LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const override;
	virtual ifile::IFilePersistence::OperationState SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const override;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const override;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const override;

private:
	I_MULTIATTR(QString, m_fileExtensionsAttrPtr);
	I_MULTITEXTATTR(m_typeDescriptionsAttrPtr);
};


} // namespace idoc




