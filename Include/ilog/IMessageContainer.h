// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// STL includes
#include <list>

// ACF includes
#include <istd/IInformationProvider.h>
#include <istd/TIHierarchical.h>
#include <iser/ISerializable.h>
#include <ilog/IMessageConsumer.h>


namespace ilog
{


/**
	Common interface for a message container.
	
	This interface extends ISerializable to provide storage and retrieval of log messages.
	It supports hierarchical organization, change notifications, and can track the worst
	(highest severity) message category encountered.
	
	Message containers are typically used to store messages for later display, analysis,
	export, or replay. They integrate with the model-observer pattern for automatic
	UI updates when messages change.
	
	\ingroup Logging
	
	\par Hierarchical Organization
	The IHierarchicalMessageContainer variant supports parent-child relationships,
	allowing messages to be organized by subsystem or component while still being
	accessible through a single interface.
	
	\par Change Notifications
	Containers notify observers when messages are added, removed, or cleared using
	the defined ChangeFlags. This enables automatic UI updates and logging reactions.
	
	\par Usage Example
	\code{.cpp}
	// Get message container
	istd::TSharedInterfacePtr<ilog::IMessageContainer> container = GetContainer();
	
	// Add messages...
	container->AddMessage(msg1);
	container->AddMessage(msg2);
	
	// Retrieve all messages
	ilog::IMessageContainer::Messages messages = container->GetMessages();
	for (const auto& msgPtr : messages) {
	    qDebug() << msgPtr->GetInformationDescription();
	}
	
	// Check severity
	if (container->GetWorstCategory() >= istd::IInformationProvider::IC_ERROR) {
	    // Errors were logged
	}
	
	// Clear for next session
	container->ClearMessages();
	\endcode
	
	\see ilog::CMessageContainer, ilog::CLogComp, ilog::IHierarchicalMessageContainer
*/
class IMessageContainer: virtual public iser::ISerializable
{
public:
	/// List type for storing message shared pointers
	typedef std::list<IMessageConsumer::MessagePtr> Messages;

	/**
		Data model change notification flags.
		
		These flags are used with the istd::IChangeable change notification system
		to inform observers of container state changes.
	*/
	enum ChangeFlags
	{
		CF_MESSAGE_ADDED = 0x1d32820,	///< A new message was added to the container
		CF_MESSAGE_REMOVED,				///< A message was removed from the container
		CF_RESET						///< Container was reset/cleared of all messages
	};

	/**
		Get the worst (highest severity) message category in the container.
		
		Scans all messages and returns the highest severity level encountered.
		This is useful for quick status checks without iterating all messages.
		
		The severity order is: IC_NONE < IC_INFO < IC_WARNING < IC_ERROR < IC_CRITICAL
		
		\return The worst message category found, or IC_NONE if no messages.
		        \sa istd::IInformationProvider::InformationCategory
		        
		\par Example
		\code{.cpp}
		istd::IInformationProvider::InformationCategory worst = 
		    container->GetWorstCategory();
		    
		switch (worst) {
		    case istd::IInformationProvider::IC_CRITICAL:
		    case istd::IInformationProvider::IC_ERROR:
		        statusIcon = errorIcon;
		        break;
		    case istd::IInformationProvider::IC_WARNING:
		        statusIcon = warningIcon;
		        break;
		    default:
		        statusIcon = okIcon;
		}
		\endcode
	*/
	virtual istd::IInformationProvider::InformationCategory GetWorstCategory() const = 0;

	/**
		Retrieve all messages currently in the container.
		
		Returns a copy of the internal message list. For hierarchical containers,
		this includes messages from all child containers as well.
		
		Messages are stored as shared pointers, so copying the list is efficient
		and safe. The returned messages are immutable and should not be modified.
		
		\return List of message shared pointers. May be empty if no messages.
		
		\note This creates a copy of the message list. For very large containers,
		      consider iterating or filtering differently.
		      
		\par Example
		\code{.cpp}
		Messages messages = container->GetMessages();
		qDebug() << "Container has" << messages.size() << "messages";
		
		for (const auto& msgPtr : messages) {
		    QString text = msgPtr->GetInformationDescription();
		    QString source = msgPtr->GetInformationSource();
		    qDebug() << source << ":" << text;
		}
		\endcode
		
		\see ClearMessages()
	*/
	virtual Messages GetMessages() const = 0;

	/**
		Remove all message items from the container.
		
		Clears the container of all stored messages. For hierarchical containers,
		this only clears the current container, not child containers.
		
		Observers are notified with CF_RESET change flag.
		
		\par Example
		\code{.cpp}
		// Start new session with clean log
		container->ClearMessages();
		
		// Or clear periodically to prevent memory buildup
		if (container->GetMessagesCount() > 10000) {
		    container->ClearMessages();
		}
		\endcode
		
		\see GetMessages(), CF_RESET
	*/
	virtual void ClearMessages() = 0;
};


/**
	Hierarchical message container interface.
	
	Combines IMessageContainer with hierarchical navigation capabilities.
	This allows containers to be organized in parent-child trees, with
	GetMessages() returning messages from the entire subtree.
	
	\ingroup Logging
	
	\par Example
	\code{.cpp}
	// Create main container
	istd::TSharedInterfacePtr<ilog::IHierarchicalMessageContainer> main = ...;
	
	// Create subsystem containers
	istd::TSharedInterfacePtr<ilog::IHierarchicalMessageContainer> network = ...;
	istd::TSharedInterfacePtr<ilog::IHierarchicalMessageContainer> database = ...;
	
	// Build hierarchy
	main->AddChild(network.get());
	main->AddChild(database.get());
	
	// main->GetMessages() includes messages from network and database
	\endcode
	
	\see istd::TIHierarchical, ilog::CMessageContainer
*/
typedef istd::TIHierarchical<ilog::IMessageContainer> IHierarchicalMessageContainer;


} // namespace ilog



