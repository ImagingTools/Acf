// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <ilog/IMessageConsumer.h>


namespace ilog
{


/**
	Interface for objects that support logging functionality.
	
	This interface allows objects to have a message consumer (logger) attached and detached.
	It's typically used in conjunction with CLoggerBase to provide logging capability
	to components and other objects.
	
	Objects implementing this interface can accept a message consumer and use it to
	send log messages. The interface doesn't specify how messages are sent; that's
	typically handled by derived classes like CLoggerBase.
	
	\ingroup Logging
	
	\par Usage Example
	\code{.cpp}
	class MyClass : public ilog::ILoggable, public ilog::CLoggerBase
	{
	public:
	    void DoWork() {
	        if (GetLogPtr()) {
	            SendInfoMessage(1, "Starting work", "MyClass::DoWork");
	        }
	    }
	};
	
	// Usage
	MyClass obj;
	istd::TSharedInterfacePtr<ilog::IMessageConsumer> log = GetLogger();
	obj.SetLogPtr(log.get());
	obj.DoWork(); // Will send log message
	\endcode
	
	\see ilog::CLoggerBase, ilog::IMessageConsumer
*/
class ILoggable: virtual public istd::IPolymorphic
{
public:
	/**
		Attach a message consumer for logging.
		
		Sets the message consumer that will receive log messages from this object.
		Pass NULL to detach the current logger and disable logging.
		
		\param	logPtr	Pointer to message consumer to use for logging, or NULL to disable.
						The caller retains ownership of the consumer; this object only
						stores a weak pointer.
						
		\note This method doesn't perform reference counting. The caller must ensure
		      the consumer remains valid while attached, or call SetLogPtr(NULL)
		      before destroying the consumer.
		      
		\par Example
		\code{.cpp}
		istd::TSharedInterfacePtr<ilog::CConsoleLogComp> console(
		    new ilog::CConsoleLogComp);
		obj->SetLogPtr(console.get()); // Attach console logger
		
		// ... use object with logging ...
		
		obj->SetLogPtr(NULL); // Detach logger
		\endcode
		
		\see GetLogPtr()
	*/
	virtual void SetLogPtr(IMessageConsumer* logPtr) = 0;
	
	/**
		Get the currently attached message consumer.
		
		Returns the message consumer previously set via SetLogPtr(), or NULL if
		no consumer is attached or if it was detached.
		
		\return Pointer to the attached message consumer, or NULL if none attached.
		
		\note The returned pointer is not reference-counted. Don't store it;
		      use it immediately and discard.
		      
		\par Example
		\code{.cpp}
		IMessageConsumer* log = obj->GetLogPtr();
		if (log) {
		    // Logger is attached, can send messages
		}
		else {
		    // No logger, skip logging
		}
		\endcode
		
		\see SetLogPtr()
	*/
	virtual IMessageConsumer* GetLogPtr() const = 0;
};


} // namespace ilog




