// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <ilog/TMessageDelegatorComp.h>


namespace ilog
{


/**
	Base implementation for logging components using Qt event queue.
	
	CLogCompBase provides thread-safe message delivery by using Qt signals and slots
	to transport messages between threads. When AddMessage() is called (potentially
	from a worker thread), it emits a signal that's delivered on the component's
	thread where WriteMessageToLog() is called.
	
	This class combines QObject (for signals/slots) with TMessageDelegatorComp to
	provide both thread-safe message handling and message delegation capabilities.
	
	Derived classes must implement WriteMessageToLog() to define what happens to
	messages when they arrive.
	
	\ingroup Logging
	
	\par Thread Safety
	AddMessage() is thread-safe and can be called from any thread. The message
	is queued via Qt's signal/slot mechanism and processed on the component's
	thread. This makes it safe to log from worker threads to a UI-based log
	viewer or any other thread-sensitive consumer.
	
	\par Message Delegation
	Inherits from TMessageDelegatorComp, so messages can be forwarded to a
	slave consumer if configured. This allows chaining multiple log handlers.
	
	\par Signal/Slot Mechanism
	Uses EmitAddMessage signal connected to OnAddMessage slot. Qt handles
	thread transitions automatically based on the connection type.
	
	\par Usage Example
	\code{.cpp}
	// Derive to create custom log component
	class MyLogComp : public ilog::CLogCompBase
	{
	protected:
	    virtual void WriteMessageToLog(const MessagePtr& messagePtr) override {
	        // Process message on component's thread
	        qDebug() << messagePtr->GetInformationDescription();
	        
	        // Forward to slave consumer if present
	        if (m_slaveMessageConsumerCompPtr.IsValid()) {
	            m_slaveMessageConsumerCompPtr->AddMessage(messagePtr);
	        }
	    }
	};
	
	// Usage from any thread
	MyLogComp* log = new MyLogComp;
	// ... initialize component ...
	
	// Safe to call from worker thread
	log->AddMessage(messagePtr);
	\endcode
	
	\see ilog::CLogComp, ilog::CConsoleLogComp, ilog::CStreamLogCompBase
*/
class CLogCompBase:
			public QObject,
			public ilog::TMessageDelegatorComp<icomp::CComponentBase>
{
	Q_OBJECT
public:
	/// Base class typedef for component functionality
	typedef ilog::TMessageDelegatorComp<icomp::CComponentBase> BaseClass;
	
	/// Base class typedef for QObject functionality
	typedef QObject BaseClass2;

	I_BEGIN_BASE_COMPONENT(CLogCompBase);
	I_END_COMPONENT;

	/**
		Default constructor.
		
		Connects the EmitAddMessage signal to OnAddMessage slot for
		thread-safe message delivery.
	*/
	CLogCompBase();

	// reimplemented (ilog::IMessageConsumer)
	
	/**
		Add a message to this log component.
		
		Thread-safe method that can be called from any thread. Emits
		EmitAddMessage signal which is delivered on the component's thread
		where OnAddMessage slot processes it.
		
		\param	messagePtr	Shared pointer to message to log
		
		\note This method returns immediately after emitting the signal.
		      Actual message processing happens asynchronously.
	*/
	virtual void AddMessage(const MessagePtr& messagePtr) override;

protected:
	/**
		Abstract method called to process a message.
		
		Derived classes must implement this to define message handling behavior.
		This is always called on the component's thread, regardless of which
		thread called AddMessage().
		
		\param	messagePtr	Shared pointer to message to process
		
		\par Example
		\code{.cpp}
		virtual void WriteMessageToLog(const MessagePtr& messagePtr) override {
		    // Store in container
		    m_messages.append(messagePtr);
		    
		    // Update UI
		    UpdateDisplay();
		    
		    // Forward to slave
		    if (m_slave) m_slave->AddMessage(messagePtr);
		}
		\endcode
	*/
	virtual void WriteMessageToLog(const MessagePtr& messagePtr) = 0;

private Q_SLOTS:
	/**
		Slot that receives messages on the component's thread.
		
		Called automatically when EmitAddMessage signal is emitted.
		Forwards the message to WriteMessageToLog().
		
		\param	messagePtr	Shared pointer to message to process
	*/
	void OnAddMessage(const MessagePtr& messagePtr);

Q_SIGNALS:
	/**
		Signal emitted when a message is added.
		
		This signal provides thread-safe message delivery. When AddMessage()
		is called from any thread, this signal is emitted and Qt delivers it
		to the OnAddMessage slot on the component's thread.
		
		\param	messagePtr	Shared pointer to message being added
	*/
	void EmitAddMessage(const MessagePtr& messagePtr);
};


} // namespace ilog



