// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


#include <iser/ISerializable.h>


namespace iprm
{


/**
	\brief Interface for objects which can be enabled/disabled.
	
	IEnableableParam provides enable/disable functionality for parameters or features.
	This is commonly used for optional features, conditional parameters, or togglable
	settings in user interfaces.
	
	\section ienableableparam_usage Usage Example
	
	\code{.cpp}
	#include <iprm/CEnableableParam.h>
	
	// Create an enableable parameter
	iprm::CEnableableParam param;
	
	// Check if enabled
	bool isEnabled = param.IsEnabled();
	qDebug() << "Enabled:" << isEnabled;
	
	// Check if enabling is allowed
	bool canEnable = param.IsEnablingAllowed();
	if (canEnable)
	{
	    // Enable the parameter
	    bool success = param.SetEnabled(true);
	    if (success)
	    {
	        qDebug() << "Parameter enabled";
	    }
	    
	    // Disable the parameter
	    param.SetEnabled(false);
	}
	\endcode
	
	\section ienableableparam_conditional Conditional Parameter Example
	
	\code{.cpp}
	// Use with other parameters to create conditional logic
	iprm::CEnableableParam useAdvancedMode;
	iprm::CTextParam advancedOption;
	
	// Only process advanced option if enabled
	if (useAdvancedMode.IsEnabled())
	{
	    QString option = advancedOption.GetText();
	    // Process advanced option...
	}
	\endcode
	
	\section ienableableparam_notification Change Notification Example
	
	\code{.cpp}
	// Monitoring enable state changes
	// (Requires observer implementation)
	
	if (changeSet.Contains(iprm::IEnableableParam::CF_ENABLED))
	{
	    bool newState = param.IsEnabled();
	    qDebug() << "Enable state changed to:" << newState;
	}
	\endcode
	
	\note Inherits from iser::ISerializable for persistence support.
	\note Some implementations may restrict enabling/disabling based on context.
	
	\see CEnableableParam
*/
class IEnableableParam: virtual public iser::ISerializable
{
public:

	/**
		\brief Change notification flags.
		
		Used with the observer pattern to detect enable state changes.
		
		\see istd::IChangeable::ChangeSet
	*/
	enum ChangeFlags
	{
		CF_ENABLED = 0x089347  ///< Enable state has changed
	};

	/**
		\brief Return true if something is enabled.
		
		\return true if currently enabled, false if disabled.
		
		\code{.cpp}
		if (param.IsEnabled())
		{
		    // Perform action that requires enabled state
		}
		\endcode
		
		\see SetEnabled, IsEnablingAllowed
	*/
	virtual bool IsEnabled() const = 0;

	/**
		\brief Return true if enabling/disabling is allowed.
		
		Indicates whether the enable state can be changed. Some parameters may have
		fixed enable states based on system constraints or dependencies.
		
		\return true if SetEnabled() can change the state, false if state is fixed.
		
		\code{.cpp}
		if (param.IsEnablingAllowed())
		{
		    // User can toggle the enable state
		    param.SetEnabled(!param.IsEnabled());
		}
		else
		{
		    // Enable state is fixed by system constraints
		    qDebug() << "Cannot change enable state";
		}
		\endcode
		
		\see SetEnabled
	*/
	virtual bool IsEnablingAllowed() const = 0;

	/**
		\brief Set the enabled state.
		
		Changes whether the parameter is enabled or disabled.
		
		\param isEnabled New enable state. true to enable, false to disable.
		\return true if the state change was allowed and successful, false otherwise.
		
		\note Returns false if IsEnablingAllowed() returns false.
		\note Triggers CF_ENABLED notification on successful change.
		
		\code{.cpp}
		// Enable
		if (param.SetEnabled(true))
		{
		    qDebug() << "Successfully enabled";
		}
		
		// Disable
		if (param.SetEnabled(false))
		{
		    qDebug() << "Successfully disabled";
		}
		
		// Toggle
		param.SetEnabled(!param.IsEnabled());
		\endcode
		
		\see IsEnabled, IsEnablingAllowed, CF_ENABLED
	*/
	virtual bool SetEnabled(bool isEnabled = true) = 0;
};


} // namespace iprm




