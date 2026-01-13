#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <iser/ISerializable.h>


namespace idoc
{


/**
	Simple interface for a text document.
	
	Provides a minimal interface for documents that store plain text content.
	This is one of the simplest document types in the ACF framework and serves
	as a good starting point for understanding the document/view architecture.
	
	The document inherits from ISerializable, allowing it to be saved and loaded
	through the ACF serialization framework.
	
	\par Usage Example
	\code
	// Create or get a text document
	idoc::ITextDocument* textDoc = GetTextDocument();
	
	// Set document content
	textDoc->SetText("Hello World!\nThis is a text document.");
	
	// Get document content
	QString content = textDoc->GetText();
	qDebug() << "Document has" << content.length() << "characters";
	
	// Save document using serialization
	iser::CXmlArchive archive;
	if (archive.OpenForWrite("/path/to/document.xml")) {
		textDoc->Serialize(archive);
		archive.Close();
	}
	
	// Load document using serialization
	if (archive.OpenForRead("/path/to/document.xml")) {
		textDoc->Serialize(archive);
		archive.Close();
	}
	\endcode
	
	\sa CTextDocumentComp, CTextFileLoaderComp
	\ingroup DocumentBasedFramework
*/
class ITextDocument: virtual public iser::ISerializable
{
public:
	/**
		Get document text.
		\return	The complete text content of the document.
	*/
	virtual QString GetText() const = 0;

	/**
		Set document text.
		\param	text	The new text content for the document.
		
		This replaces all existing content with the new text.
	*/
	virtual void SetText(const QString& text) = 0;
};


} // namespace idoc




