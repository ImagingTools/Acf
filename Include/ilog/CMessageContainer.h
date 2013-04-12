#ifndef ilog_CMessageContainer_included
#define ilog_CMessageContainer_included


// Qt includes
#include <QtCore/QList>
#include <QtCore/QMutex>

// ACF includes
#include "istd/IInformationProvider.h"
#include "istd/THierarchicalBase.h"
#include "ilog/IMessageContainer.h"
#include "ilog/IMessageConsumer.h"


namespace ilog
{		


/**
	Basic implementation of a message container.
*/
class CMessageContainer:
			public istd::THierarchicalBase<IHierarchicalMessageContainer>,
			virtual public IMessageConsumer
{
public:
	CMessageContainer();
	CMessageContainer(const CMessageContainer& container);

	virtual void AddChildContainer(IHierarchicalMessageContainer* childContainerPtr);

	void SetSlaveConsumer(ilog::IMessageConsumer* consumerPtr);

	/**
		Set maximal number of messages in the container. 
		If the container is full and a new message is added, the oldest message will be removed.
		\param maxMessageCount maximal number of messages in the container. If the value equals -1,
		no limit is set for.
	*/
	void SetMaxMessageCount(int maxMessageCount = -1);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (ilog::IMessageContainer)
	virtual int GetWorstCategory() const;
	virtual Messages GetMessages() const;
	virtual void ClearMessages();

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const IMessageConsumer::MessagePtr& messagePtr);

	// reimplemented (ilog::IHierarchicalMessageContainer)
	virtual int GetChildsCount() const;
	virtual IHierarchicalMessageContainer* GetChild(int index) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	typedef QList<IMessageConsumer::MessagePtr> MessageList;
	MessageList m_messages;

	typedef QVector<IHierarchicalMessageContainer*> Childs;
	Childs m_childContainers;

	ilog::IMessageConsumer* m_slaveConsumerPtr;

	int m_maxMessagesCount;

	int m_worstCategory;
};


} // namespace ilog


#endif // !ilog_CMessageContainer_included


