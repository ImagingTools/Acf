// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <iser/ISerializable.h>


namespace iprm
{


/**
	\brief Interface for an object containing simple text.
	
	ITextParam provides a basic text parameter interface for storing and retrieving
	string values. It supports read-only and editable modes, making it suitable for
	both input parameters and calculated/display-only values.
	
	\section itextparam_usage Usage Example
	
	\code{.cpp}
	#include <iprm/CTextParam.h>
	
	// Create a text parameter
	iprm::CTextParam textParam;
	
	// Set text value
	textParam.SetText("Hello, World!");
	
	// Get text value
	QString text = textParam.GetText();
	qDebug() << "Text:" << text;
	
	// Check if read-only
	bool isReadOnly = textParam.IsReadOnly();
	if (!isReadOnly)
	{
	    textParam.SetText("New value");
	}
	
	// Clear text
	textParam.SetText(QString());
	\endcode
	
	\section itextparam_serialization Serialization Example
	
	\code{.cpp}
	#include <iser/CMemoryReadArchive.h>
	#include <iser/CMemoryWriteArchive.h>
	
	iprm::CTextParam param;
	param.SetText("Important data");
	
	// Save to archive
	iser::CMemoryWriteArchive writeArchive(nullptr);
	param.Serialize(writeArchive);
	
	// Load from archive
	iprm::CTextParam loadedParam;
	iser::CMemoryReadArchive readArchive(writeArchive);
	loadedParam.Serialize(readArchive);
	
	QString restoredText = loadedParam.GetText();  // "Important data"
	\endcode
	
	\note Inherits from iser::ISerializable for persistence support.
	\note Supports standard IChangeable operations (clone, copy, compare, reset).
	
	\see CTextParam, IIdParam, INameParam
*/
class ITextParam: virtual public iser::ISerializable
{
public:
	/**
		\brief Get the text value.
		
		\return The current text content as QString.
		
		\code{.cpp}
		QString text = textParam.GetText();
		qDebug() << "Current text:" << text;
		\endcode
		
		\see SetText
	*/
	virtual QString GetText() const = 0;

	/**
		\brief Set the text value.
		
		Changes the text content of this parameter.
		
		\param text New text value to set. Can be empty string.
		
		\note If the parameter is read-only (IsReadOnly() returns true), the behavior
		      may be implementation-dependent.
		
		\code{.cpp}
		textParam.SetText("New value");
		
		// Clear text
		textParam.SetText(QString());
		
		// Set multiline text
		textParam.SetText("Line 1\nLine 2\nLine 3");
		\endcode
		
		\see GetText, IsReadOnly
	*/
	virtual void SetText(const QString& text) = 0;

	/**
		\brief Return true if the text is read-only.
		
		Indicates whether the text can be modified by the user or application.
		Read-only parameters are typically used for calculated values or display-only
		information.
		
		\return true if text cannot be modified, false if text is editable.
		
		\code{.cpp}
		if (!textParam.IsReadOnly())
		{
		    textParam.SetText("Editable value");
		}
		else
		{
		    qDebug() << "This parameter is read-only";
		}
		\endcode
		
		\see SetText
	*/
	virtual bool IsReadOnly() const = 0;
};


} // namespace iprm


