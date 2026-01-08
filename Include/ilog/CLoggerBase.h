#pragma once


// ACF includes
#include <istd/ILogger.h>
#include <ilog/ILoggable.h>


namespace ilog
{


class IMessageConsumer;


/**
	Base class for objects that need logging functionality.
	
	CLoggerBase provides a complete implementation of logging capabilities by combining
	istd::ILogger (for sending messages) with ilog::ILoggable (for attaching a message
	consumer). It offers convenience methods for sending messages at different severity
	levels and supports "send once" variants to prevent log spam.
	
	This class is designed to be used as a base class for components and other objects
	that need to output log messages. It doesn't own the message consumer; that must
	be provided via SetLogPtr().
	
	The class inherits ILogger as protected, so only derived classes can use the
	logging methods. This encourages encapsulation and prevents external code from
	arbitrarily logging through an object.
	
	\ingroup Logging
	
	\par Severity Levels
	Messages can be sent at four severity levels:
	- **Info**: Informational messages (IC_INFO)
	- **Warning**: Warning messages (IC_WARNING)
	- **Error**: Error messages (IC_ERROR)
	- **Critical**: Critical failure messages (IC_CRITICAL)
	
	\par Send Once Feature
	Each severity level has a "send once" variant that ensures a message with a
	specific ID is only sent the first time. Subsequent calls with the same ID
	are ignored. This prevents flooding the log with repeated messages from loops
	or frequently-called functions.
	
	\par Message Decoration
	The DecorateMessage() virtual method can be overridden to add custom prefixes,
	timestamps, or other formatting to messages before they're sent.
	
	\par Usage Example
	\code{.cpp}
	class MyComponent : public ilog::CLoggerBase
	{
	public:
	    void ProcessData() {
	        SendInfoMessage(1001, "Processing started", "MyComponent");
	        
	        if (data.IsInvalid()) {
	            SendErrorMessage(1002, "Invalid data detected", "MyComponent");
	            return;
	        }
	        
	        // This will only be logged once per component instance
	        SendWarningMessageOnce(1003, 
	            "Using fallback algorithm", 
	            "MyComponent::ProcessData");
	        
	        SendInfoMessage(1004, "Processing completed", "MyComponent");
	    }
	};
	
	// Usage
	MyComponent component;
	istd::TSharedInterfacePtr<ilog::IMessageConsumer> logger = GetLogger();
	component.SetLogPtr(logger.get());
	component.ProcessData(); // Messages sent to logger
	\endcode
	
	\par Thread Safety
	The class itself is not thread-safe. However, if the attached message consumer
	handles messages asynchronously (like CLogCompBase does), the logging methods
	can be safely called from multiple threads.
	
	\see ilog::ILoggable, ilog::IMessageConsumer, ilog::TLoggerCompWrap, istd::ILogger
*/
class CLoggerBase:
			virtual public ILoggable,
			virtual protected istd::ILogger
{
public:
	/**
		Default constructor.
		
		Creates a logger with no message consumer attached. Call SetLogPtr()
		to attach a consumer before logging.
	*/
	CLoggerBase();

	// reimplemented (ilog::ILoggable)
	virtual void SetLogPtr(ilog::IMessageConsumer* logPtr) override;
	virtual ilog::IMessageConsumer* GetLogPtr() const override;

protected:
	/**
		Send info message to log.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendInfoMessage(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send warning message to log.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendWarningMessage(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send error message to log.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendErrorMessage(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send critical message to log.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	optional human readable description of message source.
	*/
	bool SendCriticalMessage(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send once info message to log.
		Agains to \c SendInfoMessage the message will be sent only first time.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendInfoMessageOnce(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send once warning message to log.
		Agains to \c SendWarningMessage the message will be sent only first time.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendWarningMessageOnce(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send once error message to log.
		Agains to \c SendErrorMessage the message will be sent only first time.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendErrorMessageOnce(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send once critical message to log.
		Agains to \c SendCriticalMessage the message will be sent only first time.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	optional human readable description of message source.
	*/
	bool SendCriticalMessageOnce(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send message with user object.
		\param	messagePtr	pointer to user message object.
							This function overtake ownership to this object.
	*/
	bool SendUserMessage(const istd::IInformationProvider* messagePtr) const;

	/**
		Reset message lock. Enable message to be send again.
		Agains to \c SendXXXlMessageOnce for id will be sent once again.
		\param	id				binary id identifying this message type for automatic processing.
	*/
	bool AllowMessageOnceAgain(int id);

	/**
		Decorate message parts before outputting.
		It is designed to be overloaded if you want to change the decoration.
	*/
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const override;

	// reimplemented (istd::ILogger)
	virtual bool IsLogConsumed(
				const istd::IInformationProvider::InformationCategory* categoryPtr = NULL,
				const int* flagsPtr = NULL) const override;
	virtual bool SendLogMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& message,
				const QString& messageSource,
				int flags = 0) const override;

protected:
	mutable QSet<int> m_onceMessageIds;

private:
	ilog::IMessageConsumer* m_logPtr;
};


} // namespace ilog




