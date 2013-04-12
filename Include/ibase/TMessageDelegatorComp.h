#ifndef ibase_TMessageDelegatorComp_included
#define ibase_TMessageDelegatorComp_included


// ACF includes
#include "icomp/CComponentBase.h"
#include "ibase/IMessageConsumer.h"


namespace ibase
{		


/**
	Delegator component for a message consumer.

	\ingroup Helpers
*/
template <class BaseComponent>
class TMessageDelegatorComp: public BaseComponent, virtual public ibase::IMessageConsumer
{
public:
	typedef BaseComponent BaseClass;

	I_BEGIN_COMPONENT(TMessageDelegatorComp);
		I_REGISTER_INTERFACE(ibase::IMessageConsumer);
		I_ASSIGN(m_slaveMessageConsumerCompPtr, "SlaveMessageConsumer", "Slave message consumer", false, "SlaveMessageConsumer");
	I_END_COMPONENT;

	// reimplemented (ibase::IMessageConsumer)
	virtual bool IsMessageSupported(
		int messageCategory = -1,
		int messageId = -1,
		const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const MessagePtr& messagePtr);

protected:
	I_REF(ibase::IMessageConsumer, m_slaveMessageConsumerCompPtr);
};


// public methods

// reimplemented (ibase::IMessageConsumer)

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


} // namespace ibase


#endif // !ibase_TMessageDelegatorComp_included


