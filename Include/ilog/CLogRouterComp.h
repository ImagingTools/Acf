// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <ilog/IMessageConsumer.h>
#include <ilog/IMessageContainer.h>
#include <imod/IModel.h>
#include <imod/CSingleModelObserverBase.h>

namespace ilog
{


/**
	Component that routes messages from one container to another with filtering.
	
	CLogRouterComp observes a source message container and automatically forwards
	new messages to an output message consumer. It can filter messages by minimum
	severity level, and prevents duplicate forwarding by tracking already-processed
	messages.
	
	This component is useful for:
	- Separating errors from general logs
	- Forwarding critical messages to multiple destinations
	- Building hierarchical logging systems
	- Creating filtered log views
	
	\ingroup Logging
	
	\par Features
	- Automatic message forwarding when source container changes
	- Severity-based filtering (forward only messages >= minimum category)
	- Duplicate detection (won't forward same message twice)
	- Model-observer integration for reactive updates
	- Component-based configuration
	
	\par Configuration
	Component references:
	- **InputMessageContainer**: Source container to monitor (IMessageContainer)
	- **OutputMessageConsumer**: Destination for forwarded messages (IMessageConsumer)
	
	Component attributes:
	- **MinimalCategory**: Minimum severity to forward (-1=all, 0=none, 1=info+, 
	                       2=warning+, 3=error+, 4=critical only)
	
	\par Usage Example
	\code{.cpp}
	// Create source and destination
	istd::TSharedInterfacePtr<ilog::CLogComp> mainLog(new ilog::CLogComp);
	istd::TSharedInterfacePtr<ilog::CLogComp> errorLog(new ilog::CLogComp);
	
	// Create router
	istd::TSharedInterfacePtr<ilog::CLogRouterComp> router(
	    new ilog::CLogRouterComp);
	
	// Configure via .acc:
	//   InputMessageContainer -> mainLog
	//   OutputMessageConsumer -> errorLog
	//   MinimalCategory -> 3 (errors and critical)
	
	// Add messages to main log
	mainLog->AddMessage(infoMsg);    // Not forwarded
	mainLog->AddMessage(errorMsg);   // Forwarded to errorLog
	mainLog->AddMessage(criticalMsg);// Forwarded to errorLog
	\endcode
	
	\par Component Configuration
	\code{.xml}
	<Component Id="ErrorRouter" Class="ilog::CLogRouterComp">
	    <Reference Id="InputMessageContainer" Value="MainLog"/>
	    <Reference Id="OutputMessageConsumer" Value="ErrorLog"/>
	    <Attribute Id="MinimalCategory" Value="3"/>
	</Component>
	\endcode
	
	\see ilog::IMessageContainer, ilog::IMessageConsumer, imod::IObserver
*/
class CLogRouterComp:
			public icomp::CComponentBase,
			public imod::CSingleModelObserverBase
{
public:
	/// Base class typedef for component functionality
	typedef icomp::CComponentBase BaseClass;
	
	/// Base class typedef for observer functionality
	typedef imod::CSingleModelObserverBase BaseClass2;

	I_BEGIN_COMPONENT(CLogRouterComp);
		I_REGISTER_INTERFACE(imod::IObserver)
		I_ASSIGN(m_messageContainerPtr, "InputMessageContainer", "Message container to track", true, "InputMessageContainer");
		I_ASSIGN_TO(m_messageContainerModelPtr, m_messageContainerPtr, true);
		I_ASSIGN(m_outputMessageConsumerPtr, "OutputMessageConsumer", "The acceptor of the forwarded messages", false, "OutputMessageConsumer");
		I_ASSIGN(m_minimalCategoryAttr, "MinimalCategory", "Minimal category to forward to the output log, negative value means forwarding everything", true, -1);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

private:
	QDateTime m_lastMessageTimestamp;
	QList<const istd::IInformationProvider*> m_lastProcessed;

private:
	I_REF(ilog::IMessageContainer, m_messageContainerPtr);
	I_REF(imod::IModel, m_messageContainerModelPtr);
	I_REF(ilog::IMessageConsumer, m_outputMessageConsumerPtr);
	I_ATTR(int, m_minimalCategoryAttr);
};

} // namespace ilog




