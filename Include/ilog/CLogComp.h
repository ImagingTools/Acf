#pragma once


// ACF includes
#include <ilog/CLogCompBase.h>
#include <ilog/CMessageContainer.h>


namespace ilog
{


/**
	Complete logging component with message storage and container interface.
	
	CLogComp is a full-featured logging component that combines thread-safe message
	reception (via CLogCompBase) with message storage (via CMessageContainer). It's
	one of the most commonly used logging components in the ACF framework.
	
	This component stores received messages and provides the IMessageContainer
	interface for retrieving, serializing, and managing the stored messages. It
	supports a configurable maximum message count with automatic pruning when
	the limit is exceeded.
	
	\ingroup Logging
	
	\par Features
	- Thread-safe message addition via signals/slots
	- Message storage with IMessageContainer interface
	- Configurable maximum message count
	- Automatic old message removal when limit reached
	- Serialization of all stored messages
	- Message retrieval and clearing
	- Worst category tracking
	- Hierarchical container support (via CMessageContainer)
	
	\par Configuration
	Component attributes:
	- **MaxMessageCount**: Maximum number of messages to store (default: 1000)
	  - Set to -1 for unlimited storage (use with caution)
	  - When limit reached, oldest messages are removed
	
	\par Usage Example
	\code{.cpp}
	// Create log component
	istd::TSharedInterfacePtr<ilog::CLogComp> log(new ilog::CLogComp);
	
	// Configure maximum messages
	// In .acc: MaxMessageCount: 5000
	
	// Add messages (thread-safe)
	log->AddMessage(istd::TSharedInterfacePtr<ilog::CMessage>(
	    new ilog::CMessage(istd::IInformationProvider::IC_INFO,
	                      1001, "Application started", "Main")));
	
	// Retrieve all messages
	ilog::IMessageContainer::Messages messages = log->GetMessages();
	
	// Check severity
	if (log->GetWorstCategory() >= istd::IInformationProvider::IC_ERROR) {
	    // Errors occurred
	}
	
	// Clear messages
	log->ClearMessages();
	
	// Serialize to file
	iser::CFileWriteArchive archive("log.dat");
	log->Serialize(archive);
	\endcode
	
	\par Component Integration
	In ACF component configuration (.acc files):
	\code{.xml}
	<Component Id="ApplicationLog" Class="ilog::CLogComp">
	    <Attribute Id="MaxMessageCount" Value="10000"/>
	</Component>
	\endcode
	
	\see ilog::CLogCompBase, ilog::CMessageContainer, ilog::IMessageContainer
*/
class CLogComp:
			public ilog::CLogCompBase,
			public ilog::CMessageContainer
{
public:
	/// Base class typedef for component functionality
	typedef CLogCompBase BaseClass;
	
	/// Base class typedef for container functionality
	typedef ilog::CMessageContainer BaseClass2;

	I_BEGIN_COMPONENT(CLogComp);
		I_REGISTER_INTERFACE(ilog::IMessageContainer);
		I_ASSIGN(m_maxMessageCountAttrPtr, "MaxMessageCount", "Maximal number of messages", false, 1000);
	I_END_COMPONENT;

	// reimplemented (ilog::IMessageConsumer)
	
	/**
		Check if a message is supported.
		
		Delegates to CMessageContainer's implementation which typically accepts
		all messages.
		
		\param	messageCategory	Message category to check (or -1 for any)
		\param	messageId		Message ID to check (or -1 for any)
		\param	messagePtr		Complete message to check (or NULL)
		\return true if message would be accepted
	*/
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const override;
	
	/**
		Add a message to the log.
		
		Thread-safe method that stores the message in the container. If the
		maximum message count is exceeded, the oldest message is removed first.
		
		\param	messagePtr	Shared pointer to message to add
		
		\note This method is asynchronous. The message is queued and processed
		      on the component's thread.
	*/
	virtual void AddMessage(const MessagePtr& messagePtr) override;

protected:
	// reimplemented (ilog::CLogCompBase)
	
	/**
		Process a message by storing it in the container.
		
		Called on the component's thread when a message arrives. Adds the
		message to the internal container (CMessageContainer base class).
		
		\param	messagePtr	Shared pointer to message to store
	*/
	virtual void WriteMessageToLog(const MessagePtr& messagePtr) override;

	// reimplemented (icomp::CComponentBase)
	
	/**
		Called when component is created.
		
		Initializes the maximum message count from the component attribute.
	*/
	virtual void OnComponentCreated() override;

private:
	/// Attribute holding maximum message count
	I_ATTR(int, m_maxMessageCountAttrPtr);
};


} // namespace ilog



