// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>
#include <QtCore/QByteArray>

// ACF includes
#include <istd/IPolymorphic.h>

#include <iprm/iprm.h>


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
	if (infoProvider)
	{
	    // Get information about a specific parameter
	    const iprm::IParamsInfoProvider::ParamInfo* info = 
	        infoProvider->GetParamInfo("threshold");
	    
	    if (info)
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
class IParamsInfoProvider: virtual public istd::IPolymorphic
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
		\param[out] info Reference to ParamInfo structure that will be filled with
		                 parameter metadata.
		\return True if parameter information was found and filled in, false otherwise.
		
		\code{.cpp}
		iprm::IParamsInfoProvider::ParamInfo info;
		if (infoProvider->GetParamInfo("myParam", info))
		{
		    // Use info.name and info.description
		}
		\endcode
	*/
	virtual bool GetParamInfo(const QByteArray& paramId, ParamInfo& info) const = 0;
};


} // namespace iprm




