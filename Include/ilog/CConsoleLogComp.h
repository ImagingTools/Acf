// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <ilog/CStreamLogCompBase.h>
#include <istd/IInformationProvider.h>


namespace ilog
{


/**
	Console logging component with colored output.
	
	CConsoleLogComp outputs log messages to the console (std::wcout) with
	platform-specific colored output based on message severity. This makes it
	easy to visually distinguish between info, warnings, and errors when running
	applications from the command line.
	
	The component uses CStreamLogCompBase for message formatting and filtering,
	and adds color support for both Windows (via Windows console API) and
	Unix-like systems (via ANSI escape codes).
	
	\ingroup Logging
	
	\par Color Scheme
	- **Info/None**: Default console color (typically white/gray)
	- **Warning**: Yellow/amber text
	- **Error**: Red text
	- **Critical**: Bright red text
	
	\par Platform Support
	- **Windows**: Uses SetConsoleTextAttribute API
	- **Unix/Linux/macOS**: Uses ANSI escape codes (when terminal supports them)
	
	\par Usage Example
	\code{.cpp}
	// Create console logger
	istd::TSharedInterfacePtr<ilog::CConsoleLogComp> console(
	    new ilog::CConsoleLogComp);
	
	// Configure formatting
	// In .acc:
	//   MinCategory: 1        (info and above)
	//   UseCategory: true     (show "Warning:", "Error:")
	//   UseCode: true         (show message IDs)
	//   UseTimeStamp: false   (no timestamps)
	
	// Send messages
	console->AddMessage(istd::TSharedInterfacePtr<ilog::CMessage>(
	    new ilog::CMessage(istd::IInformationProvider::IC_INFO,
	                      1001, "Application started", "Main")));
	
	console->AddMessage(istd::TSharedInterfacePtr<ilog::CMessage>(
	    new ilog::CMessage(istd::IInformationProvider::IC_ERROR,
	                      5001, "Connection failed", "Network")));
	
	// Console output:
	// Info (1001): Application started - Main
	// Error (5001): Connection failed - Network    [in red]
	\endcode
	
	\par Component Configuration
	\code{.xml}
	<Component Id="ConsoleLog" Class="ilog::CConsoleLogComp">
	    <Attribute Id="MinCategory" Value="1"/>
	    <Attribute Id="UseCategory" Value="true"/>
	    <Attribute Id="UseCode" Value="true"/>
	    <Attribute Id="UseTimeStamp" Value="true"/>
	    <Attribute Id="TimeFormat" Value="hh:mm:ss"/>
	</Component>
	\endcode
	
	\note Color support depends on terminal capabilities. Some terminals or
	      redirected output may not display colors correctly.
	
	\see ilog::CStreamLogCompBase, ilog::CLogCompBase
*/
class CConsoleLogComp: public CStreamLogCompBase
{
public:
	/// Base class typedef
	typedef CStreamLogCompBase BaseClass;

	I_BEGIN_COMPONENT(CConsoleLogComp);
	I_END_COMPONENT;

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteText(const QString& text, istd::IInformationProvider::InformationCategory category) override;

private:
	void SetConsoleColor(istd::IInformationProvider::InformationCategory category);
};


} // namespace ilog



