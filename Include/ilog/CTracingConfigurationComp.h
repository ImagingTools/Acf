#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <ilog/ITracingConfiguration.h>
#include <iser/ISerializable.h>


namespace ilog
{


/**
	Component for configuring tracing verbosity levels.
	
	CTracingConfigurationComp provides a component-based implementation of
	ITracingConfiguration, allowing runtime control of verbose logging output.
	It integrates with the component system for serialization and configuration.
	
	This component is typically referenced by TLoggerCompWrap-based components
	through their "TracingConfiguration" reference, allowing centralized control
	of verbosity across multiple components.
	
	\ingroup Logging
	
	\par Features
	- Runtime tracing level control (-1=off, 0=all, N=selective)
	- Serialization support for persistent configuration
	- Change notification when tracing level changes
	- Default level configuration via component attribute
	
	\par Configuration
	Component attributes:
	- **DefaulTracingtLevel**: Initial tracing level (default: -1 = disabled)
	  - -1: Tracing disabled
	  - 0: Enable all verbose messages
	  - 1+: Enable messages with level <= value
	
	Component interfaces:
	- Implements ITracingConfiguration for level access
	- Implements ISerializable for persistence
	
	\par Usage Example
	\code{.cpp}
	// Create tracing configuration
	istd::TSharedInterfacePtr<ilog::CTracingConfigurationComp> tracing(
	    new ilog::CTracingConfigurationComp);
	
	// Configure default level (or via .acfc: DefaulTracingtLevel: 1)
	tracing->SetTracingLevel(1); // Enable levels 0 and 1
	
	// Components reference this for tracing control
	// In .acfc for components:
	//   <Reference Id="TracingConfiguration" Value="GlobalTracing"/>
	
	// Runtime control
	tracing->SetTracingLevel(0);  // Enable all tracing
	tracing->SetTracingLevel(-1); // Disable all tracing
	\endcode
	
	\par Component Configuration
	\code{.xml}
	<Component Id="GlobalTracing" Class="ilog::CTracingConfigurationComp">
	    <Attribute Id="DefaulTracingtLevel" Value="1"/>
	</Component>
	
	<Component Id="MyComponent" Class="MyComponentClass">
	    <Reference Id="TracingConfiguration" Value="GlobalTracing"/>
	</Component>
	\endcode
	
	\par Integration with TLoggerCompWrap
	Components using TLoggerCompWrap automatically use the tracing configuration:
	\code{.cpp}
	class MyComp : public ilog::CLoggerComponentBase
	{
	public:
	    void Process() {
	        // Only logged if tracing level >= 0
	        SendVerboseMessage("Processing started", QString(), 0);
	        
	        // Only logged if tracing level >= 1
	        if (IsVerboseEnabled(1)) {
	            SendVerboseMessage("Detailed step", QString(), 1);
	        }
	    }
	};
	\endcode
	
	\see ilog::ITracingConfiguration, ilog::TLoggerCompWrap
*/
class CTracingConfigurationComp:
			public icomp::CComponentBase,
			virtual public ilog::ITracingConfiguration,
			virtual public iser::ISerializable
{
public:
	/// Base class typedef
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTracingConfigurationComp);
		I_REGISTER_INTERFACE(ilog::ITracingConfiguration);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_defaultTracingLevel, "DefaulTracingtLevel", "Default tracing level, -1 tracing off, 0 tracing all", true, -1);
	I_END_COMPONENT;

	CTracingConfigurationComp();

	// reimplemented (ilog::ITracingConfiguration)
	virtual int GetTracingLevel() const override;
	virtual void SetTracingLevel(int tracingLevel) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(int, m_defaultTracingLevel);

	int m_tracingLevel;
};


} // namespace ilog

