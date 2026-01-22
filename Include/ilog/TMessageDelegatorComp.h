// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <ilog/IMessageConsumer.h>


namespace ilog
{


/**
	Template component that delegates messages to a slave consumer.
	
	TMessageDelegatorComp provides message delegation functionality by implementing
	IMessageConsumer and forwarding all messages to a configurable slave consumer.
	This enables building chains and hierarchies of message consumers.
	
	The template can be combined with any component base class to add message
	delegation capability.
	
	\tparam BaseComponent	Base component class to extend with delegation
	                        (typically icomp::CComponentBase or derived)
	
	\ingroup Logging
	
	\par Features
	- Implements IMessageConsumer interface
	- Forwards all messages to slave consumer
	- Component-based configuration
	- Supports message filtering by slave
	- Can be used to build consumer chains
	
	\par Component References
	- **SlaveMessageConsumer**: Destination for forwarded messages (IMessageConsumer)
	
	\par Usage Example
	\code{.cpp}
	// Create a simple delegator component
	class MyDelegator : public ilog::TMessageDelegatorComp<icomp::CComponentBase>
	{
	public:
	    typedef ilog::TMessageDelegatorComp<icomp::CComponentBase> BaseClass;
	    
	    I_BEGIN_COMPONENT(MyDelegator);
	        // SlaveMessageConsumer reference is inherited
	    I_END_COMPONENT;
	    
	    // Can add custom processing before/after delegation
	    virtual void AddMessage(const MessagePtr& messagePtr) override {
	        // Custom processing
	        qDebug() << "Forwarding:" << messagePtr->GetInformationDescription();
	        
	        // Delegate to slave
	        BaseClass::AddMessage(messagePtr);
	    }
	};
	
	// Configure chain
	// MessageSource -> MyDelegator -> FinalConsumer
	\endcode
	
	\par Component Configuration
	\code{.xml}
	<Component Id="Delegator" Class="MyDelegatorClass">
	    <Reference Id="SlaveMessageConsumer" Value="FinalConsumer"/>
	</Component>
	\endcode
	
	\par Building Consumer Chains
	\code{.cpp}
	// Create consumers
	istd::TSharedInterfacePtr<ilog::CLogComp> finalLog(new ilog::CLogComp);
	istd::TSharedInterfacePtr<MyDelegator> delegator1(new MyDelegator);
	istd::TSharedInterfacePtr<MyDelegator> delegator2(new MyDelegator);
	
	// Configure chain: source -> delegator1 -> delegator2 -> finalLog
	// delegator1.SlaveMessageConsumer -> delegator2
	// delegator2.SlaveMessageConsumer -> finalLog
	
	// Messages sent to delegator1 are forwarded through the chain
	\endcode
	
	\see ilog::IMessageConsumer, ilog::CLogCompBase
*/
template <class BaseComponent>
class TMessageDelegatorComp: public BaseComponent, virtual public ilog::IMessageConsumer
{
public:
	/// Base class typedef
	typedef BaseComponent BaseClass;

	I_BEGIN_COMPONENT(TMessageDelegatorComp);
		I_REGISTER_INTERFACE(ilog::IMessageConsumer);
		I_ASSIGN(m_slaveMessageConsumerCompPtr, "SlaveMessageConsumer", "Slave message consumer", false, "SlaveMessageConsumer");
	I_END_COMPONENT;

	// reimplemented (ilog::IMessageConsumer)
	
	/**
		Check if a message is supported.
		
		Delegates the check to the slave consumer if present. If no slave is
		configured, returns true (accepts all messages).
		
		\param	messageCategory	Message category to check (or -1)
		\param	messageId		Message ID to check (or -1)
		\param	messagePtr		Complete message to check (or NULL)
		\return true if message is supported by slave (or if no slave)
	*/
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const override;
	
	/**
		Forward a message to the slave consumer.
		
		If a slave consumer is configured via SlaveMessageConsumer reference,
		the message is forwarded to it. If no slave is configured, the message
		is silently discarded.
		
		\param	messagePtr	Shared pointer to message to forward
		
		\note This method does nothing if no slave is configured. Override to
		      add custom behavior when no slave is present.
	*/
	virtual void AddMessage(const MessagePtr& messagePtr) override;

protected:
	I_REF(ilog::IMessageConsumer, m_slaveMessageConsumerCompPtr);
};


// public methods

// reimplemented (ilog::IMessageConsumer)

template <class BaseComponent>
bool TMessageDelegatorComp<BaseComponent>::IsMessageSupported(
			int messageCategory,
			int messageId,
			const istd::IInformationProvider* messagePtr) const
{
	if (m_slaveMessageConsumerCompPtr.IsValid()){
		return m_slaveMessageConsumerCompPtr->IsMessageSupported(messageCategory, messageId, messagePtr);
	}

	return true;
}


template <class BaseComponent>
void TMessageDelegatorComp<BaseComponent>::AddMessage(const MessagePtr& messagePtr)
{
	if (m_slaveMessageConsumerCompPtr.IsValid()){
		return m_slaveMessageConsumerCompPtr->AddMessage(messagePtr);
	}
}


} // namespace ilog




