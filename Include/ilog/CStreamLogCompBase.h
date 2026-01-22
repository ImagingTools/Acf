// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <ilog/CLogCompBase.h>


namespace ilog
{


/**
	Base class for stream-based logging components.
	
	CStreamLogCompBase provides foundation for logging components that output formatted
	messages to text streams. It handles message filtering by severity, message formatting
	with customizable elements (category, code, timestamp), and tracks statistics.
	
	Derived classes implement WriteText() to output the formatted message to their
	specific destination (console, file, network, etc.).
	
	\ingroup Logging
	
	\par Features
	- Severity level filtering (only log messages above minimum category)
	- Customizable message formatting:
	  - Optional category label ("Info:", "Warning:", "Error:", "Critical:")
	  - Optional message ID/code display
	  - Optional timestamp with configurable format
	- Dot display for filtered messages (shows "." for suppressed messages)
	- Worst category tracking (highest severity seen)
	- Thread-safe operation via CLogCompBase
	
	\par Configuration
	Component attributes:
	- **MinCategory**: Minimum severity to log (0=all, 1=info+, 2=warning+, 3=error+, 4=critical)
	- **ShowDots**: Show dot (".") for each filtered message (default: false)
	- **UseCategory**: Include category label in output (default: true)
	- **UseCode**: Include message ID in output (default: true)
	- **UseTimeStamp**: Include timestamp in output (default: false)
	- **TimeFormat**: Qt date/time format string (default: "dd.MM hh:mm:ss:zzz")
	
	\par Message Format
	Typical output format (depending on configuration):
	\code
	[12:34:56.789] Warning (2001): Low disk space - DiskMonitor
	Error (5001): Connection failed - NetworkManager
	Info: Application started - Main
	\endcode
	
	\par Usage Example (Derived Class)
	\code{.cpp}
	class CFileLogComp : public ilog::CStreamLogCompBase
	{
	public:
	    CFileLogComp() : m_file("log.txt") {
	        m_file.open(QIODevice::WriteOnly | QIODevice::Text);
	    }
	    
	protected:
	    virtual void WriteText(const QString& text, 
	                          istd::IInformationProvider::InformationCategory category) override {
	        if (m_file.isOpen()) {
	            m_file.write(text.toUtf8());
	            m_file.flush();
	        }
	    }
	    
	private:
	    QFile m_file;
	};
	\endcode
	
	\see ilog::CConsoleLogComp, ilog::CLogCompBase
*/
class CStreamLogCompBase: public CLogCompBase
{
public:
	/// Base class typedef
	typedef ilog::CLogCompBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CStreamLogCompBase);
		I_ASSIGN(m_minPriorityAttrPtr, "MinCategory", "Minimal category of message to print it out:\n *1-Information\n *2-Warning\n *3-Error\n *4-Critical", true, 0);
		I_ASSIGN(m_isDotEnabledAttrPtr, "ShowDots", "If it's true, dot will be shown for each ignored message", true, false);
		I_ASSIGN(m_useCategoryAttrPtr, "UseCategory", "Use error category for the messsages (e.g warning or error)", true, true);
		I_ASSIGN(m_useCodeAttrPtr, "UseCode", "Use error code for the messsages", true, true);
		I_ASSIGN(m_useTimeStampAttrPtr, "UseTimeStamp", "Use time stamp for the messsages", true, false);
		I_ASSIGN(m_timeFormatAttrPtr, "TimeFormat", "Format used for timestamp output", false, "dd.MM hh:mm:ss:zzz");
	I_END_COMPONENT;

	CStreamLogCompBase();

	/**
		Get worse category (highest code) of all outputed messages.
	*/
	istd::IInformationProvider::InformationCategory GetWorseCategory() const;

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const override;

protected:
	/**
		Write formatted message to the output stream.
	*/
	virtual void WriteMessageToStream(const istd::IInformationProvider& message);
	
	/**
		Generate formatted message text for the output.
	*/
	virtual QString GenerateMessageText(const istd::IInformationProvider& message) const;

	// abstract methods

	/**
		Write a text line to the output stream.
	*/
	virtual void WriteText(const QString& text, istd::IInformationProvider::InformationCategory category) = 0;

	// reimplemented (ilog::CLogCompBase)
	virtual void WriteMessageToLog(const MessagePtr& messagePtr) override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed() override;

private:
	I_ATTR(int, m_minPriorityAttrPtr);
	I_ATTR(bool, m_isDotEnabledAttrPtr);
	I_ATTR(bool, m_useCategoryAttrPtr);
	I_ATTR(bool, m_useCodeAttrPtr);
	I_ATTR(bool, m_useTimeStampAttrPtr);
	I_ATTR(QByteArray, m_timeFormatAttrPtr); 

	bool m_isLastDotShown;
	istd::IInformationProvider::InformationCategory m_lastDotCategory;
	istd::IInformationProvider::InformationCategory m_worseCategory;
};


} // namespace ilog



