// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IInformationProvider.h>
#include <iser/IObject.h>


namespace ilog
{


/**
	Basic implementation of the istd::IInformationProvider interface for log messages.
	
	CMessage is the fundamental message class in the ilog library. It stores all standard
	message properties including category (severity), ID, text, source, flags, and timestamp.
	
	The class implements both IInformationProvider (for message data access) and IObject
	(for serialization and type identification), making it fully compatible with the ACF
	framework's serialization and component systems.
	
	Messages are typically created with all properties at construction and treated as
	immutable thereafter, though setter methods are provided for scenarios requiring
	modification.
	
	\ingroup Logging
	
	\par Message Properties
	- **Category**: Severity level (IC_NONE, IC_INFO, IC_WARNING, IC_ERROR, IC_CRITICAL)
	- **ID**: Numeric identifier for message type (application-defined)
	- **Text**: Human-readable message description
	- **Source**: Origin of the message (component, module, or function name)
	- **Flags**: Application-defined flags for filtering or processing
	- **Timestamp**: When the message was created (automatically set if not provided)
	
	\par Thread Safety
	Individual message objects are not thread-safe for modification. However, they are
	typically treated as immutable after creation and shared via TSharedInterfacePtr,
	making them safe for multi-threaded read access.
	
	\par Usage Example
	\code{.cpp}
	// Create a simple info message
	istd::TSharedInterfacePtr<ilog::CMessage> msg(
	    new ilog::CMessage(
	        istd::IInformationProvider::IC_INFO,
	        1001,
	        "Application started successfully",
	        "Main"
	    )
	);
	
	// Create an error message with custom timestamp
	QDateTime customTime = QDateTime::currentDateTime();
	istd::TSharedInterfacePtr<ilog::CMessage> errorMsg(
	    new ilog::CMessage(
	        istd::IInformationProvider::IC_ERROR,
	        5001,
	        "Database connection failed",
	        "DatabaseManager",
	        0,
	        &customTime
	    )
	);
	
	// Send to logger
	logger->AddMessage(msg);
	\endcode
	
	\see ilog::CExtMessage, ilog::TExtMessage, istd::IInformationProvider
*/
class CMessage:
			virtual public istd::IInformationProvider,
			virtual public iser::IObject
{
public:
	/**
		Default constructor.
		
		Creates an empty message with:
		- Category: IC_NONE
		- ID: 0
		- Empty text and source
		- No flags
		- Current timestamp
		
		Typically used when deserializing or before calling SetMessageValues().
	*/
	CMessage();
	
	/**
		Construct a message with all properties.
		
		\param	category	Message severity level
		\param	id			Numeric message identifier (application-defined)
		\param	text		Human-readable message description
		\param	source		Message origin (component, module, function name)
		\param	flags		Optional application-defined flags (default: 0)
		\param	timeStampPtr Optional pointer to timestamp, or NULL to use current time
		
		\par Example
		\code{.cpp}
		// Create warning message
		ilog::CMessage warning(
		    istd::IInformationProvider::IC_WARNING,
		    2001,
		    "Memory usage above 80%",
		    "MemoryMonitor",
		    MY_FLAG_PERFORMANCE
		);
		\endcode
	*/
	CMessage(	istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags = 0,
				const QDateTime* timeStampPtr = NULL);

	/**
		Set all message properties at once.
		
		Allows modification of message properties after construction. Typically used
		when reusing message objects or when constructing incrementally.
		
		\param	category	Message severity level
		\param	id			Numeric message identifier
		\param	text		Human-readable message description
		\param	source		Message origin
		\param	flags		Optional application-defined flags (default: 0)
		\param	timeStampPtr Optional pointer to timestamp, or NULL to use current time
		
		\note Modifying messages after adding them to containers or consumers may
		      lead to inconsistencies. Prefer creating new messages.
	*/
	void SetMessageValues(istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text, 
				const QString& source,
				int flags = 0,
				const QDateTime* timeStampPtr = NULL);

	/**
		Set the message category (severity level).
		
		\param	category	New message category
		
		\see GetInformationCategory()
	*/
	virtual void SetCategory(istd::IInformationProvider::InformationCategory category);
	
	/**
		Set the message text.
		
		\param	text	New message description
		
		\see GetInformationDescription()
	*/
	virtual void SetText(const QString& text);
	
	/**
		Set the message source.
		
		\param	source	New message origin identifier
		
		\see GetInformationSource()
	*/
	virtual void SetSource(const QString& source);

	// reimplemented (istd::IInformationProvider)
	
	/**
		Get the message timestamp.
		
		\return When the message was created
		
		\see QDateTime
	*/
	virtual QDateTime GetInformationTimeStamp() const override;
	
	/**
		Get the message category (severity level).
		
		\return Message category (IC_NONE, IC_INFO, IC_WARNING, IC_ERROR, IC_CRITICAL)
	*/
	virtual InformationCategory GetInformationCategory() const override;
	
	/**
		Get the numeric message identifier.
		
		\return Application-defined message ID
	*/
	virtual int GetInformationId() const override;
	
	/**
		Get the human-readable message text.
		
		\return Message description
	*/
	virtual QString GetInformationDescription() const override;
	
	/**
		Get the message source identifier.
		
		\return Message origin (component, module, function name)
	*/
	virtual QString GetInformationSource() const override;
	
	/**
		Get application-defined flags.
		
		\return Message flags for custom filtering or processing
	*/
	virtual int GetInformationFlags() const override;

	// reimplemented (iser::IObject)
	
	/**
		Get the factory identifier for serialization.
		
		\return Type identifier for object factory
		
		\see GetMessageTypeId()
	*/
	virtual QByteArray GetFactoryId() const override;

	// reimplemented (iser::ISerializable)
	
	/**
		Serialize or deserialize the message.
		
		Stores/loads all message properties (category, ID, text, source, flags, timestamp)
		to/from the archive.
		
		\param	archive	Archive for reading or writing
		\return true if serialization succeeded, false on error
		
		\see iser::IArchive
	*/
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	
	/**
		Copy message data from another object.
		
		\param	object	Source object to copy from (must be compatible)
		\param	mode	Compatibility mode for copying
		\return true if copy succeeded, false if incompatible
	*/
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	
	/**
		Create a deep copy of this message.
		
		\param	mode	Compatibility mode for cloning
		\return New message object with copied data, or NULL on error
		
		\note Caller takes ownership of the returned pointer
	*/
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

	/**
		Get the message type identifier for serialization.
		
		\return Type identifier string
	*/
	static QByteArray GetMessageTypeId();

protected:
	/// Message category (severity level)
	istd::IInformationProvider::InformationCategory m_category;
	
	/// Numeric message identifier
	int m_id;
	
	/// Human-readable message text
	QString m_text;
	
	/// Message source identifier
	QString m_source;
	
	/// Application-defined flags
	int m_flags;

	/// Message creation timestamp
	QDateTime m_timeStamp;
};


// inline methods

// reimplemented (istd::IInformationProvider)

inline QDateTime CMessage::GetInformationTimeStamp() const
{
	return m_timeStamp;
}


inline istd::IInformationProvider::InformationCategory CMessage::GetInformationCategory() const
{
	return m_category;
}


inline int CMessage::GetInformationId() const
{
	return m_id;
}


inline QString CMessage::GetInformationDescription() const
{
	return m_text;
}


inline QString CMessage::GetInformationSource() const
{
	return m_source;
}


inline int CMessage::GetInformationFlags() const
{
	return m_flags;
}


} // namespace ilog




