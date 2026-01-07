#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QList>


// ACF includes
#include <istd/IChangeable.h>


namespace iprm
{


class IParamsSet;


/**
	\brief Provider of state information for a parameter inside a parameter set.
	
	IParameterStateProvider determines the state (such as editability) of parameters
	within a parameter set based on application logic or context. This enables dynamic
	control over which parameters are editable in different scenarios.
	
	This interface is commonly used to:
	- Enable/disable parameter editing based on application state
	- Implement conditional parameter visibility or editability
	- Provide context-sensitive parameter behavior
	
	\section iparameterstateprovider_usage Usage Example
	
	\code{.cpp}
	// Assuming stateProvider is obtained from a component
	iprm::IParameterStateProvider* stateProvider = /* ... */;
	
	// Assuming paramsSet is obtained
	iprm::IParamsSet* paramsSet = /* ... */;
	
	// Check if a parameter is editable
	QByteArray paramId = "threshold";
	iprm::IParameterStateProvider::ParameterState editState = 
	    stateProvider->GetState(
	        *paramsSet,
	        paramId,
	        iprm::IParameterStateProvider::ST_EDIT);
	
	switch (editState)
	{
	    case iprm::IParameterStateProvider::PS_ON:
	        qDebug() << "Parameter is editable";
	        // Allow user to edit
	        break;
	    
	    case iprm::IParameterStateProvider::PS_OFF:
	        qDebug() << "Parameter is read-only";
	        // Disable editing UI
	        break;
	    
	    case iprm::IParameterStateProvider::PS_UNKNOWN:
	        qDebug() << "State not determined by provider";
	        // Use default behavior
	        break;
	}
	\endcode
	
	\section iparameterstateprovider_iteration Checking Multiple Parameters
	
	\code{.cpp}
	iprm::IParamsSet::Ids paramIds = paramsSet->GetParamIds();
	
	for (const QByteArray& id : paramIds)
	{
	    auto state = stateProvider->GetState(
	        *paramsSet, id, iprm::IParameterStateProvider::ST_EDIT);
	    
	    if (state == iprm::IParameterStateProvider::PS_ON)
	    {
	        qDebug() << "Parameter" << id << "is editable";
	    }
	}
	\endcode
	
	\note Inherits from istd::IChangeable for change notification support.
	\note State providers enable separation of UI logic from data model.
	
	\see IParamsSet
*/
class IParameterStateProvider: virtual public istd::IChangeable
{
public:
	/**
		\brief Type of state to query.
		
		Different state types can be checked for parameters.
	*/
	enum StateType
	{
		/**
			Get the "edit" status of the parameter.
			Determines whether the parameter can be modified.
		*/
		ST_EDIT
	};

	/**
		\brief Possible parameter states.
		
		States that a parameter can be in for a given state type.
	*/
	enum ParameterState
	{
		PS_UNKNOWN,  ///< State is not determined by this provider
		PS_ON,       ///< State is active (e.g., parameter is editable)
		PS_OFF       ///< State is inactive (e.g., parameter is read-only)
	};

	/**
		\brief Return state for the given parameter.
		
		Queries the state of a specific parameter within a parameter set.
		
		\param paramSet The parameter set containing the parameter.
		\param parameterId ID of the parameter to check.
		\param stateType Type of state to query (e.g., ST_EDIT).
		\return The state of the parameter: PS_ON, PS_OFF, or PS_UNKNOWN.
		
		\note PS_UNKNOWN indicates this provider doesn't determine the state,
		      allowing other providers or default behavior to apply.
		
		\code{.cpp}
		// Check if parameter can be edited
		auto editState = stateProvider->GetState(
		    *paramsSet,
		    "paramId",
		    iprm::IParameterStateProvider::ST_EDIT);
		
		bool canEdit = (editState == iprm::IParameterStateProvider::PS_ON);
		\endcode
		
		\see StateType, ParameterState
	*/
	virtual ParameterState GetState(
				const iprm::IParamsSet& paramSet,
				const QByteArray& parameterId,
				StateType stateType) const = 0;
};


} // namespace iprm




