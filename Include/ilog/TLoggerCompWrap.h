// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <ilog/CLoggerBase.h>
#include <ilog/ITracingConfiguration.h>


namespace ilog
{


/**
	Template wrapper adding comprehensive logging functionality to components.
	
	TLoggerCompWrap combines any component base class with CLoggerBase to provide
	full logging capabilities. It automatically integrates with the ACF component
	system, managing log consumer connections, verbose message support, tracing
	configuration, and message source decoration.
	
	This is one of the most commonly used templates in the ilog library, providing
	the standard way to add logging to ACF components.
	
	\tparam Base	Base component class to extend with logging (typically icomp::CComponentBase
	                or a derived class)
	
	\ingroup Logging
	
	\par Features
	- Automatic log consumer connection via "Log" reference
	- Verbose message support with configurable enable/disable
	- Tracing level integration for granular verbose control
	- Automatic component ID decoration of message sources
	- All CLoggerBase convenience methods (SendInfoMessage, SendErrorMessage, etc.)
	- "Send once" message variants to prevent log spam
	- Customizable message decoration
	
	\par Component References
	- **Log**: Message consumer to receive log messages (IMessageConsumer)
	- **TracingConfiguration**: Optional tracing level control (ITracingConfiguration)
	
	\par Component Attributes
	- **EnableVerbose**: Enable/disable verbose messages (default: false)
	- **ShowComponentId**: Include component ID in message source (default: true)
	
	\par Usage Example
	\code{.cpp}
	// Define component with logging
	class MyComponent : public ilog::TLoggerCompWrap<icomp::CComponentBase>
	{
	public:
	    typedef ilog::TLoggerCompWrap<icomp::CComponentBase> BaseClass;
	    
	    I_BEGIN_COMPONENT(MyComponent);
	        // ... component definition ...
	    I_END_COMPONENT;
	    
	    void ProcessData() {
	        // Standard logging
	        SendInfoMessage(1001, "Processing started", "ProcessData");
	        
	        try {
	            // ... processing ...
	            SendInfoMessage(1002, "Processing completed", "ProcessData");
	        }
	        catch (const std::exception& e) {
	            SendErrorMessage(1003, 
	                QString("Processing failed: %1").arg(e.what()),
	                "ProcessData");
	        }
	    }
	    
	    void DetailedOperation() {
	        // Verbose message (only if enabled)
	        SendVerboseMessage("Starting detailed operation", "DetailedOperation");
	        
	        // Tracing level 1 (only if tracing >= 1)
	        if (IsVerboseEnabled(1)) {
	            SendVerboseMessage("Step 1 complete", "DetailedOperation", 1);
	        }
	        
	        // Tracing level 2 (only if tracing >= 2)
	        if (IsVerboseEnabled(2)) {
	            SendVerboseMessage("Very detailed info", "DetailedOperation", 2);
	        }
	    }
	};
	\endcode
	
	\par Component Configuration
	\code{.xml}
	<Component Id="MyComp" Class="MyComponent">
	    <Reference Id="Log" Value="MainLog"/>
	    <Reference Id="TracingConfiguration" Value="GlobalTracing"/>
	    <Attribute Id="EnableVerbose" Value="true"/>
	    <Attribute Id="ShowComponentId" Value="true"/>
	</Component>
	\endcode
	
	\par Message Source Decoration
	When ShowComponentId is enabled, messages are automatically prefixed with the
	component ID:
	\code
	// Without ShowComponentId:
	"Processing started - ProcessData"
	
	// With ShowComponentId:
	"Processing started - MyComp (ProcessData)"
	\endcode
	
	\par Type Aliases
	For convenience, a typedef is provided for the common case:
	\code{.cpp}
	typedef ilog::TLoggerCompWrap<icomp::CComponentBase> CLoggerComponentBase;
	
	// Usage:
	class MyComponent : public ilog::CLoggerComponentBase
	{
	    // ... component implementation ...
	};
	\endcode
	
	\see ilog::CLoggerBase, ilog::ITracingConfiguration, ilog::CLoggerComponentBase
*/
template <class Base>
class TLoggerCompWrap:
			public Base,
			virtual public ilog::CLoggerBase
{
public:
	/// Base class typedef for component functionality
	typedef Base BaseClass;
	
	/// Base class typedef for logger functionality
	typedef ilog::CLoggerBase BaseClass2;

	I_BEGIN_BASE_COMPONENT(TLoggerCompWrap);
		I_ASSIGN(m_logCompPtr, "Log", "Consumer log messages", false, "Log");
		I_ASSIGN(m_verboseEnabledAttrPtr, "EnableVerbose", "If enabled, verbose messages can be produced", true, false);
		I_ASSIGN(m_tracingConfigurationCompPtr, "TracingConfiguration", "If enabled, verbose messages can be produced with tracing level", false, "TracingConfiguration");
		I_ASSIGN(m_showComponentIdAttrPtr, "ShowComponentId", "If enabled the component ID will be shown as a part of the message source", true, true);
	I_END_COMPONENT;

protected:
	/**
		Check if verbose messages are enabled for a given tracing level.
		
		Determines whether verbose messages at the specified tracing level should
		be output. Checks both the EnableVerbose attribute and the tracing
		configuration (if present).
		
		\param	tracingLevel	Tracing level to check (0=always if enabled, higher=more selective)
		\return true if verbose messages at this level are enabled
		
		\par Example
		\code{.cpp}
		if (IsVerboseEnabled(1)) {
		    SendVerboseMessage("Detailed diagnostic", QString(), 1);
		}
		\endcode
		
		\see SendVerboseMessage()
	*/
	bool IsVerboseEnabled(int tracingLevel = 0) const;

	/**
		Send a verbose message if enabled.
		
		Convenience method for sending verbose/diagnostic messages. The message
		is only sent if IsVerboseEnabled(tracingLevel) returns true.
		
		Verbose messages use category IC_NONE and are typically used for
		diagnostic output that's not always needed.
		
		\param	message			Message text
		\param	messageSource	Optional message source (default: empty)
		\param	tracingLevel	Tracing level (0=always if enabled, higher=more selective)
		
		\par Example
		\code{.cpp}
		// Basic verbose message
		SendVerboseMessage("Operation started");
		
		// With source
		SendVerboseMessage("Processing item 5", "ProcessItems");
		
		// With tracing level
		SendVerboseMessage("Very detailed info", "Details", 2);
		\endcode
		
		\see IsVerboseEnabled()
	*/
	void SendVerboseMessage(const QString& message, const QString& messageSource = QString(), int tracingLevel = 0) const;

	// reimplemented (istd::ILogger)
	
	/**
		Decorate message before sending.
		
		Adds component ID to message source if ShowComponentId is enabled.
		Called automatically before messages are sent.
		
		Override to add custom decoration:
		\code{.cpp}
		virtual void DecorateMessage(
		            istd::IInformationProvider::InformationCategory category,
		            int id,
		            int flags,
		            QString& message,
		            QString& messageSource) const override
		{
		    BaseClass::DecorateMessage(category, id, flags, message, messageSource);
		    // Add custom decoration
		    message = QString("[%1] %2").arg(GetCustomPrefix()).arg(message);
		}
		\endcode
		
		\param	category		Message category
		\param	id				Message ID
		\param	flags			Message flags
		\param	message			Message text (can be modified)
		\param	messageSource	Message source (can be modified)
	*/
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const override;

	// reimplemented (icomp::CComponentBase)
	
	/**
		Called when component is created.
		
		Automatically connects the log consumer from the "Log" reference.
		
		\note Override to add initialization, but always call base class:
		\code{.cpp}
		virtual void OnComponentCreated() override {
		    // Custom initialization
		    BaseClass::OnComponentCreated();
		}
		\endcode
	*/
	virtual void OnComponentCreated() override;
	
	/**
		Called when component is destroyed.
		
		Automatically disconnects the log consumer for cleanup.
		
		\note Override to add cleanup, but always call base class:
		\code{.cpp}
		virtual void OnComponentDestroyed() override {
		    // Custom cleanup
		    BaseClass::OnComponentDestroyed();
		}
		\endcode
	*/
	virtual void OnComponentDestroyed() override;

private:
	I_REF(ilog::IMessageConsumer, m_logCompPtr);
	I_ATTR(bool, m_verboseEnabledAttrPtr);
	I_REF(ilog::ITracingConfiguration, m_tracingConfigurationCompPtr);
	I_ATTR(bool, m_showComponentIdAttrPtr);
};


// protected methods
	
template <class Base>
bool TLoggerCompWrap<Base>::IsVerboseEnabled(int tracingLevel) const
{
	static const istd::IInformationProvider::InformationCategory categoryNone = istd::IInformationProvider::IC_NONE;

	bool retVal = *m_verboseEnabledAttrPtr;;

	if (m_tracingConfigurationCompPtr.IsValid()){
		retVal = m_tracingConfigurationCompPtr->GetTracingLevel() >= tracingLevel;
	}

	retVal = retVal && BaseClass2::IsLogConsumed(&categoryNone);

	return retVal;
}


template <class Base>
void TLoggerCompWrap<Base>::SendVerboseMessage(const QString& message, const QString& messageSource, int tracingLevel) const
{
	if (IsVerboseEnabled(tracingLevel)){
		BaseClass2::SendLogMessage(istd::IInformationProvider::IC_NONE, 0, message, messageSource);
	}
}


// reimplemented (istd::ILogger)

template <class Base>
void TLoggerCompWrap<Base>::DecorateMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			int flags,
			QString& message,
			QString& messageSource) const
{
	BaseClass2::DecorateMessage(category, id, flags, message, messageSource);

	if (*m_showComponentIdAttrPtr){
		const icomp::IComponentContext* contextPtr = BaseClass::GetComponentContext().get();
		if (contextPtr != NULL){
			if (messageSource.isEmpty()){
				messageSource = contextPtr->GetContextId();
			}
			else{
				messageSource = QString(contextPtr->GetContextId()) + " (" + messageSource + ")";
			}
		}
	}
}


// reimplemented (icomp::CComponentBase)

template <class Base>
void TLoggerCompWrap<Base>::OnComponentCreated()
{
	if (m_logCompPtr.IsValid()){
		this->SetLogPtr(m_logCompPtr.GetPtr());
	}

	BaseClass::OnComponentCreated();
}


template <class Base>
void TLoggerCompWrap<Base>::OnComponentDestroyed()
{
	this->SetLogPtr(NULL);

	BaseClass::OnComponentDestroyed();
}


/**
	Convenience typedef for simple components with logging.
	
	CLoggerComponentBase is the most commonly used base class for ACF components
	that need logging functionality. It combines icomp::CComponentBase with
	TLoggerCompWrap to provide a complete component with integrated logging.
	
	Use this as a base class for most components that need to output log messages.
	
	\ingroup Main
	
	\par Usage Example
	\code{.cpp}
	class MyComponent : public ilog::CLoggerComponentBase
	{
	public:
	    typedef ilog::CLoggerComponentBase BaseClass;
	    
	    I_BEGIN_COMPONENT(MyComponent);
	        // ... component definition ...
	    I_END_COMPONENT;
	    
	    void DoWork() {
	        SendInfoMessage(1, "Work started", "DoWork");
	        // ... do work ...
	        SendInfoMessage(2, "Work completed", "DoWork");
	    }
	};
	\endcode
	
	\see ilog::TLoggerCompWrap, icomp::CComponentBase
*/
typedef TLoggerCompWrap<icomp::CComponentBase> CLoggerComponentBase;


} // namespace ilog


