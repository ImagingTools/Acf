// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace ilog
{


/**
	Interface for configuring tracing verbosity levels.
	
	This interface allows runtime control of diagnostic/tracing output verbosity.
	Higher tracing levels enable more detailed diagnostic messages, while -1 disables
	tracing entirely.
	
	The tracing level is typically used in conjunction with TLoggerCompWrap's verbose
	message methods to control debug output granularity without recompilation.
	
	\ingroup Logging
	
	\par Tracing Levels
	- **-1**: Tracing disabled (no verbose messages)
	- **0**: Tracing all verbose messages (highest verbosity)
	- **1+**: Selective tracing (only messages with level <= current level)
	
	\par Usage Example
	\code{.cpp}
	// In component with TLoggerCompWrap
	class MyComp : public ilog::CLoggerComponentBase
	{
	public:
	    void Analyze() {
	        // Level 0 - major operations
	        SendVerboseMessage("Starting analysis", QString(), 0);
	        
	        // Level 1 - detailed steps
	        if (IsVerboseEnabled(1)) {
	            SendVerboseMessage("Processing chunk 1", QString(), 1);
	        }
	        
	        // Level 2 - very detailed
	        if (IsVerboseEnabled(2)) {
	            SendVerboseMessage("Iteration details", QString(), 2);
	        }
	    }
	};
	
	// Configuration controls verbosity
	istd::TSharedInterfacePtr<ilog::ITracingConfiguration> tracing = ...;
	tracing->SetTracingLevel(1);  // Enable levels 0 and 1 only
	\endcode
	
	\see ilog::TLoggerCompWrap, ilog::CTracingConfigurationComp
*/
class ITracingConfiguration: virtual public istd::IChangeable
{
public:
	/**
		Get the current tracing level.
		
		Returns the currently configured tracing verbosity level.
		
		\return Current tracing level:
		        - -1 = tracing disabled
		        - 0 = trace all verbose messages
		        - N = trace messages with level <= N
		        
		\par Example
		\code{.cpp}
		int level = tracing->GetTracingLevel();
		if (level >= 0) {
		    qDebug() << "Tracing enabled at level" << level;
		}
		else {
		    qDebug() << "Tracing disabled";
		}
		\endcode
		
		\see SetTracingLevel()
	*/
	virtual int GetTracingLevel() const = 0;
	
	/**
		Set the tracing level.
		
		Changes the current tracing verbosity level. This affects which verbose
		messages will be output by components using TLoggerCompWrap.
		
		Observers are notified of the change through the IChangeable interface.
		
		\param	tracingLevel	New tracing level:
		                        - -1 = disable tracing (default)
		                        - 0 = enable all verbose messages
		                        - N = enable messages with level <= N
		                        
		\par Example
		\code{.cpp}
		// Disable all tracing
		tracing->SetTracingLevel(-1);
		
		// Enable all tracing (maximum verbosity)
		tracing->SetTracingLevel(0);
		
		// Enable selective tracing (levels 0 and 1)
		tracing->SetTracingLevel(1);
		\endcode
		
		\see GetTracingLevel()
	*/
	virtual void SetTracingLevel(int tracingLevel) = 0;
};


} // namespace ilog

