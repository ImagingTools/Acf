// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace iprm
{


/**
	\brief Provider of parameter information (name and description).
	
	IParamsInfoProvider is an interface that provides metadata about parameters
	in a parameter set. Each parameter can have a human-readable name and description.
	
	\section iparaminfoprovider_usage Usage Example
	
	\code{.cpp}
	// Get parameter info provider from a parameter set
	const iprm::IParamsInfoProvider* infoProvider = paramsSet->GetParamsInfoProvider();
	if (infoProvider != nullptr)
	{
	    // Get information about a specific parameter
	    std::unique_ptr<iprm::IParamsInfoProvider::ParamInfo> info = 
	        infoProvider->GetParamInfo("threshold");
	    if (info != nullptr)
	    {
	        QString displayName = info->name;
	        QString description = info->description;
	        
	        qDebug() << "Parameter:" << displayName;
	        qDebug() << "Description:" << description;
	    }
	}
	\endcode
	
	\see IParamsSet
*/
class IParamsInfoProvider: virtual public istd::IChangeable
{
public:
	/**
		\brief Structure containing parameter metadata.
		
		Contains human-readable information about a parameter.
	*/
	struct ParamInfo
	{
		QString name;			///< Human-readable parameter name
		QString description;	///< Detailed parameter description
	};

	/**
		\brief Get parameter information by parameter ID.
		
		Retrieves metadata (name and description) for a parameter identified by its ID.
		
		\param paramId ID of the parameter to get information for.
		\return Unique pointer to ParamInfo structure if parameter was found, nullptr otherwise.
		
		\code{.cpp}
		std::unique_ptr<iprm::IParamsInfoProvider::ParamInfo> info = 
		    infoProvider->GetParamInfo("myParam");
		if (info != nullptr)
		{
		    // Use info->name and info->description
		}
		\endcode
	*/
	virtual std::unique_ptr<ParamInfo> GetParamInfo(const QByteArray& paramId) const = 0;
};


} // namespace iprm




