// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <ilog/CStandardStreamBridge.h>


namespace ilog
{


/**
	Component wrapper for standard stream bridging.
	
	CStandardStreamBridgeComp provides a component-based interface for redirecting
	std::cout and std::cerr to the logging system. It creates and manages
	CStandardStreamBridge instances for both streams, automatically handling their
	lifecycle.
	
	This is the recommended way to use stream bridging in ACF applications, as it
	integrates with the component system and handles initialization/cleanup properly.
	
	\ingroup Logging
	
	\par Features
	- Automatically bridges both std::cout (as INFO) and std::cerr (as ERROR)
	- Component-based configuration and lifecycle management
	- Automatic cleanup when component is destroyed
	- Thread-safe stream redirection
	
	\par Configuration
	Component references:
	- **SlaveMessageConsumer**: Consumer to receive messages from streams (IMessageConsumer)
	
	\par Usage Example
	\code{.cpp}
	// Create log consumer
	istd::TSharedInterfacePtr<ilog::CConsoleLogComp> logger(
	    new ilog::CConsoleLogComp);
	
	// Create bridge component
	istd::TSharedInterfacePtr<ilog::CStandardStreamBridgeComp> bridge(
	    new ilog::CStandardStreamBridgeComp);
	
	// Configure via .acc or programmatically:
	//   SlaveMessageConsumer -> logger
	
	// Now all std::cout and std::cerr output goes to logger
	std::cout << "This is logged as INFO" << std::endl;
	std::cerr << "This is logged as ERROR" << std::endl;
	
	// printf/fprintf also work (they use the same streams)
	printf("This is also logged as INFO\n");
	fprintf(stderr, "This is also logged as ERROR\n");
	
	// Bridge is destroyed with component, streams restored
	\endcode
	
	\par Component Configuration
	\code{.xml}
	<Component Id="StreamBridge" Class="ilog::CStandardStreamBridgeComp">
	    <Reference Id="SlaveMessageConsumer" Value="MainLog"/>
	</Component>
	\endcode
	
	\note The stream buffers are restored when the component is destroyed.
	      Ensure proper component lifecycle to avoid dangling redirections.
	
	\see ilog::CStandardStreamBridge, ilog::IMessageConsumer
*/
class CStandardStreamBridgeComp:public icomp::CComponentBase
{
public:
	/// Base class typedef
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CStandardStreamBridgeComp);
		I_ASSIGN(m_slaveMessageConsumerCompPtr, "SlaveMessageConsumer", "Slave message consumer", false, "SlaveMessageConsumer");
	I_END_COMPONENT;


protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	istd::TDelPtr<CStandardStreamBridge> m_coutBridgePtr;
	istd::TDelPtr<CStandardStreamBridge> m_cerrBridgePtr;

	I_REF(ilog::IMessageConsumer, m_slaveMessageConsumerCompPtr);
};


} // namespace ilog



