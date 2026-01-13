#pragma once


// STL includes
#include <iostream>
#include <streambuf>
#include <string>

// Qt includes
#include <QtCore/QString>
#include <QtCore/QMutex>

// ACF includes
#include <ilog/IMessageConsumer.h>


namespace ilog
{


/**
	Redirects C++ standard output streams (cout/cerr) to the logging system.
	
	CStandardStreamBridge intercepts output sent to std::cout or std::cerr and
	converts it into log messages. This is useful for capturing output from legacy
	code, third-party libraries, or console-based diagnostics.
	
	The class inherits from std::basic_streambuf and replaces the stream's buffer,
	capturing all output that would normally go to the console and instead sending
	it as log messages.
	
	\ingroup Logging
	
	\par Features
	- Captures std::cout and std::cerr output
	- Converts stream output to log messages
	- Configurable severity per stream (e.g., cout=info, cerr=error)
	- Thread-safe with mutex protection
	- Automatic restoration of original stream buffer on destruction
	- Line-based message generation (splits on newline)
	
	\par Thread Safety
	Uses QMutex to protect internal buffer during multi-threaded output.
	Safe to use with multiple threads writing to the same stream.
	
	\par Usage Example
	\code{.cpp}
	// Create log consumer
	istd::TSharedInterfacePtr<ilog::CConsoleLogComp> logger(
	    new ilog::CConsoleLogComp);
	
	// Create bridges for cout (as info) and cerr (as error)
	ilog::CStandardStreamBridge coutBridge(
	    std::cout, 
	    *logger, 
	    istd::IInformationProvider::IC_INFO);
	    
	ilog::CStandardStreamBridge cerrBridge(
	    std::cerr, 
	    *logger, 
	    istd::IInformationProvider::IC_ERROR);
	
	// Now std::cout and std::cerr are redirected
	std::cout << "This becomes an info message" << std::endl;
	std::cerr << "This becomes an error message" << std::endl;
	
	// Bridges are destroyed when going out of scope,
	// restoring original stream buffers
	\endcode
	
	\note When the bridge is destroyed, the original stream buffer is restored.
	      Make sure the bridge lifetime covers all code that needs redirection.
	
	\warning Don't delete the stream or consumer while the bridge is active.
	         The bridge holds raw pointers to both.
	
	\see ilog::CStandardStreamBridgeComp, ilog::IMessageConsumer
*/
class CStandardStreamBridge: protected std::basic_streambuf<char>
{
public:
	/// Base class typedef
	typedef std::basic_streambuf<char> BaseClass;

	/**
		Construct a stream bridge and activate redirection.
		
		Replaces the stream's buffer with this bridge, capturing all output.
		
		\param	stream				Reference to stream to bridge (std::cout, std::cerr, etc.)
		\param	messageConsumer		Consumer to receive converted messages
		\param	informationSeverity	Severity level for generated messages
		
		\par Example
		\code{.cpp}
		ilog::CStandardStreamBridge bridge(
		    std::cout,
		    *myLogger,
		    istd::IInformationProvider::IC_INFO
		);
		// std::cout is now redirected to myLogger
		\endcode
	*/
	CStandardStreamBridge(
				std::ostream &stream,
				ilog::IMessageConsumer& messageConsumer,
				istd::IInformationProvider::InformationCategory informationSeverity);
	
	/**
		Destructor - restores original stream buffer.
		
		When the bridge is destroyed, the original stream buffer is restored,
		returning the stream to normal console output.
	*/
	virtual ~CStandardStreamBridge();

protected:
	// reimplemented (std::basic_streambuf)
	
	/**
		Handle bulk character writes.
		
		Called when multiple characters are written at once. Accumulates
		characters in internal buffer until newline, then sends as message.
		
		\param	data		Pointer to character data
		\param	bufferSize	Number of characters to write
		\return Number of characters written
	*/
	virtual std::streamsize xsputn(const char* data, std::streamsize bufferSize) override;
	
	/**
		Handle single character writes.
		
		Called when a single character is written. Accumulates in buffer
		until newline, then sends as message.
		
		\param	v	Character to write
		\return The character written, or EOF on error
	*/
	virtual int_type overflow(int_type v) override;

private:
	/// Reference to the bridged stream
	std::ostream& m_stream;
	
	/// Pointer to original stream buffer (for restoration)
	std::streambuf* m_originalBufferPtr;

	/// Consumer to receive generated messages
	ilog::IMessageConsumer& m_messageConsumer;
	
	/// Severity level for generated messages
	istd::IInformationProvider::InformationCategory m_informationSeverity;

	/// Internal buffer accumulating current line
	QString m_textBuffer;
	
	/// Mutex protecting buffer access
	mutable QMutex m_bufferMutex;
};


} // namespace ilog




