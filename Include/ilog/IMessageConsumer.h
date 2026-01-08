#pragma once


#include <istd/IPolymorphic.h>
#include <istd/IInformationProvider.h>
#include <istd/TSmartPtr.h>


namespace ilog
{


/**
	Common interface for a message container consuming information objects (messages).
	
	This is the fundamental interface in the ilog library that all message consumers must implement.
	It provides a standardized way to check message support and add messages to the consumer.
	
	The interface uses a shared pointer (MessagePtr) for message ownership, ensuring automatic
	memory management. Consumers can be chained together, filtered, or routed based on
	message properties.
	
	\ingroup Main
	
	\par Thread Safety
	Implementations must handle thread safety. Most implementations use Qt signals for
	thread-safe message delivery.
	
	\par Usage Example
	\code{.cpp}
	// Create a consumer
	istd::TSharedInterfacePtr<ilog::IMessageConsumer> consumer = GetLogConsumer();
	
	// Check if consumer accepts error messages
	if (consumer->IsMessageSupported(istd::IInformationProvider::IC_ERROR)) {
	    // Create and send error message
	    istd::TSharedInterfacePtr<ilog::CMessage> msg(
	        new ilog::CMessage(istd::IInformationProvider::IC_ERROR, 
	                          1001, "Error occurred", "Module"));
	    consumer->AddMessage(msg);
	}
	\endcode
	
	\see ilog::CLogComp, ilog::CConsoleLogComp, ilog::CMessageContainer
*/
class IMessageConsumer: virtual public istd::IPolymorphic
{
public:
	/// Shared pointer type for message objects
	typedef istd::TSharedInterfacePtr<istd::IInformationProvider> MessagePtr;

	/**
		Check if a message is supported by this consumer.
		
		This method allows consumers to filter messages based on category, ID, or the
		complete message object. It's used to determine whether a message should be
		sent to this consumer before actually adding it.
		
		\param	messageCategory	Category of message (IC_NONE, IC_INFO, IC_WARNING, IC_ERROR, IC_CRITICAL)
								or -1 if category check should be skipped.
								\sa istd::IInformationProvider::InformationCategory.
		\param	messageId		Numeric ID of message or -1 if ID check should be skipped.
								Message IDs are application-defined for filtering and routing.
		\param	messagePtr		Optional pointer to complete message object for detailed inspection,
								or NULL if not needed. The consumer will not store this pointer.
								
		\return true if the message is supported and should be sent to this consumer,
		        false if the message should be filtered out.
		        
		\note Returning false does not indicate an error; it simply means the message
		      doesn't match the consumer's filter criteria.
		      
		\par Example
		\code{.cpp}
		// Check if consumer accepts warnings
		if (consumer->IsMessageSupported(istd::IInformationProvider::IC_WARNING)) {
		    // Consumer accepts warnings
		}
		
		// Check specific message ID
		if (consumer->IsMessageSupported(-1, 5001)) {
		    // Consumer accepts message ID 5001
		}
		
		// Check complete message
		ilog::CMessage msg(istd::IInformationProvider::IC_INFO, 1, "Test", "Source");
		if (consumer->IsMessageSupported(-1, -1, &msg)) {
		    // Consumer accepts this specific message
		}
		\endcode
	*/
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const = 0;
	
	/**
		Adds a message to this consumer.
		
		This method transfers ownership of the message to the consumer via shared pointer.
		The consumer may store, display, forward, or otherwise process the message.
		
		Most implementations deliver messages asynchronously using Qt signals/slots to
		ensure thread safety when messages are added from worker threads.
		
		\param	messagePtr	Shared pointer to the message. After this call, the message
							is owned by the shared pointer and will be automatically
							deleted when no longer referenced.
							
		\note This method should not throw exceptions. Implementations should handle
		      errors gracefully (e.g., by logging internally or ignoring invalid messages).
		      
		\warning The message object should not be modified after being added to a consumer,
		         as multiple consumers may reference the same message object.
		         
		\par Example
		\code{.cpp}
		// Create message with shared pointer
		istd::TSharedInterfacePtr<ilog::CMessage> msg(
		    new ilog::CMessage(
		        istd::IInformationProvider::IC_ERROR,
		        2001,
		        "File not found: config.xml",
		        "ConfigLoader"
		    )
		);
		
		// Add to consumer (ownership transferred)
		consumer->AddMessage(msg);
		
		// msg can still be used as shared pointer maintains reference count
		// but don't modify the message after adding it
		\endcode
		
		\see IsMessageSupported()
	*/
	virtual void AddMessage(const MessagePtr& messagePtr) = 0;
};


} // namespace ilog




