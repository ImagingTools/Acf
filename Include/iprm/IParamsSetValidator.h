// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <ilog/IMessageConsumer.h>


namespace iprm
{


class IParamsSet;


/**
	\brief Interface for consistency checking of a parameter set.
	
	IParamsSetValidator provides validation functionality for parameter sets,
	ensuring that parameters meet required constraints and business rules before
	processing or persistence.
	
	Validators can check:
	- Value ranges and formats
	- Inter-parameter dependencies
	- Required vs. optional parameters
	- Business logic constraints
	- Data consistency rules
	
	\section iparamssetvalidator_usage Usage Example
	
	\code{.cpp}
	#include <iprm/IParamsSetValidator.h>
	#include <ilog/CMessageConsumer.h>
	
	// Assuming validator is obtained from a component or factory
	iprm::IParamsSetValidator* validator = /* ... */;
	
	// Get supported type IDs
	iprm::IParamsSetValidator::Ids supportedTypes = validator->GetSupportedTypeIds();
	qDebug() << "Validator supports" << supportedTypes.size() << "type(s)";
	
	for (const QByteArray& typeId : supportedTypes)
	{
	    qDebug() << "  - Type:" << typeId;
	}
	
	// Validate a parameter set
	iprm::IParamsSet* paramsSet = /* ... */;
	QByteArray validationContext = "save_operation";
	
	// Create message consumer to collect validation messages
	ilog::CMessageConsumer messageConsumer;
	
	// Perform validation
	bool isValid = validator->IsParamsSetConsistent(
	    validationContext,
	    *paramsSet,
	    &messageConsumer);
	
	if (isValid)
	{
	    qDebug() << "Parameter set is valid";
	    // Proceed with save or processing
	}
	else
	{
	    qDebug() << "Validation failed";
	    // Handle validation errors
	    // Check messageConsumer for detailed error messages
	}
	\endcode
	
	\section iparamssetvalidator_context Context-Specific Validation
	
	Different validation contexts can have different rules:
	
	\code{.cpp}
	// Validate for saving (strict rules)
	bool validForSave = validator->IsParamsSetConsistent(
	    "save_context",
	    *paramsSet,
	    &messageConsumer);
	
	// Validate for preview (relaxed rules)
	bool validForPreview = validator->IsParamsSetConsistent(
	    "preview_context",
	    *paramsSet,
	    nullptr);  // No detailed messages needed
	
	if (validForSave)
	{
	    SaveParameterSet(paramsSet);
	}
	else if (validForPreview)
	{
	    ShowPreview(paramsSet);
	}
	\endcode
	
	\note Validators should be stateless for thread-safety.
	\note Multiple validators can be combined for different parameter types.
	
	\see IParamsSet, ilog::IMessageConsumer
*/
class IParamsSetValidator: virtual public istd::IPolymorphic
{
public:
	typedef QSet<QByteArray> Ids;

	/**
		\brief Get list of parameter type IDs which can be checked by the validator.
		
		Returns the set of parameter set type identifiers that this validator
		can validate. This allows routing different parameter set types to
		appropriate validators.
		
		\return Set of QByteArray type IDs supported by this validator.
		
		\code{.cpp}
		iprm::IParamsSetValidator::Ids supportedTypes = 
		    validator->GetSupportedTypeIds();
		
		QByteArray paramSetType = "algorithm_params";
		if (supportedTypes.contains(paramSetType))
		{
		    qDebug() << "Validator can check this type";
		    bool isValid = validator->IsParamsSetConsistent(
		        "default", *paramsSet, nullptr);
		}
		else
		{
		    qDebug() << "Need a different validator";
		}
		\endcode
		
		\see IsParamsSetConsistent
	*/
	virtual Ids	GetSupportedTypeIds() const = 0;

	/**
		\brief Return true if the parameter set is consistent, false otherwise.
		
		Validates a parameter set against constraints and business rules. Optionally,
		detailed validation messages can be collected through a message consumer.
		
		\param validationContextId ID identifying the validation context. Different
		                           contexts may apply different validation rules
		                           (e.g., "save", "execute", "preview").
		\param paramsSet Parameter set to be validated.
		\param validationMessagesConsumerPtr Optional consumer for validation messages.
		                                     If provided, detailed error/warning messages
		                                     will be sent to it. Can be NULL if messages
		                                     aren't needed.
		\return true if parameter set is consistent and passes validation,
		        false if validation failed.
		
		\note When validation fails, check validationMessagesConsumerPtr for detailed
		      error messages if one was provided.
		\note The same parameter set may be valid in one context and invalid in another.
		
		\code{.cpp}
		ilog::CMessageConsumer messageConsumer;
		
		bool isValid = validator->IsParamsSetConsistent(
		    "save_operation",
		    *paramsSet,
		    &messageConsumer);
		
		if (!isValid)
		{
		    qDebug() << "Validation failed:";
		    // Process messages from messageConsumer
		    // Messages contain details about what failed
		}
		\endcode
		
		\see GetSupportedTypeIds, ilog::IMessageConsumer
	*/
	virtual bool IsParamsSetConsistent(
				const QByteArray& validationContextId,
				const IParamsSet& paramsSet,
				ilog::IMessageConsumer* validationMessagesConsumerPtr = NULL) const = 0;
};


} // namespace iprm


