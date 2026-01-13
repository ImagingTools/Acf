#pragma once


// Qt includes
#include <QtCore/QSet>
#include <QtCore/QByteArray>

// ACF includes
#include <iser/IObject.h>

#include <iprm/iprm.h>


namespace iprm
{


/**
	\brief Set of general parameters.
	
	IParamsSet is a container that groups related parameters together. Each parameter
	is identified by a unique QByteArray ID. The set distinguishes between read-only
	parameters (e.g., calculated or display values) and editable parameters that users
	can modify.
	
	Parameter sets are commonly used to represent:
	- Configuration settings for an algorithm or tool
	- Properties of an object or entity
	- Input/output parameters for a computation
	- User preferences or settings groups
	
	\section iparamset_usage Usage Example
	
	\code{.cpp}
	// Assuming paramsSet is obtained from a component or manager
	iprm::IParamsSet* paramsSet = manager->GetParamsSet(0);
	
	// Get all parameter IDs
	iprm::IParamsSet::Ids allIds = paramsSet->GetParamIds();
	
	// Get only editable parameter IDs
	iprm::IParamsSet::Ids editableIds = paramsSet->GetParamIds(true);
	
	// Access a parameter for reading
	const iser::ISerializable* param = paramsSet->GetParameter("threshold");
	const iprm::ITextParam* textParam = dynamic_cast<const iprm::ITextParam*>(param);
	if (textParam)
	{
	    QString value = textParam->GetText();
	}
	
	// Access a parameter for editing
	iser::ISerializable* editableParam = paramsSet->GetEditableParameter("threshold");
	iprm::ITextParam* editableTextParam = dynamic_cast<iprm::ITextParam*>(editableParam);
	if (editableTextParam)
	{
	    editableTextParam->SetText("5.0");
	}
	
	// Iterate through all parameters
	for (const QByteArray& id : allIds)
	{
	    const iser::ISerializable* p = paramsSet->GetParameter(id);
	    // Process parameter...
	}
	\endcode
	
	\note Parameters can be editable or not (e.g., automatically calculated parameters).
	\note Always use dynamic_cast to determine the actual parameter type.
	
	\see IParamsManager, ITextParam, ISelectionParam
*/
class IParamsSet: virtual public iser::IObject
{
public:
	typedef QSet<QByteArray> Ids;

	/**
		\brief Get list of used parameter IDs in the parameter set.
		
		This method returns the IDs of all parameters contained in this set.
		Optionally, it can return only editable parameter IDs.
		
		\param editableOnly If true, returns only IDs of editable parameters.
		                    If false (default), returns all parameter IDs.
		\return Set of parameter IDs (QByteArray identifiers).
		
		\code{.cpp}
		// Get all parameter IDs
		iprm::IParamsSet::Ids allIds = paramsSet->GetParamIds();
		qDebug() << "Total parameters:" << allIds.size();
		
		// Get only editable parameter IDs
		iprm::IParamsSet::Ids editableIds = paramsSet->GetParamIds(true);
		qDebug() << "Editable parameters:" << editableIds.size();
		\endcode
	*/
	virtual Ids GetParamIds(bool editableOnly = false) const = 0;

	/**
		\brief Get any parameter (read-only access).
		
		Retrieves a parameter by its ID for read-only access. This method returns
		both editable and non-editable parameters.
		
		\param id ID of parameter. This is an application-specific identifier that
		          uniquely identifies the parameter within this set.
		\return Pointer to parameter instance (as iser::ISerializable*) or NULL if
		        no parameter exists with the given ID.
		
		\note Use dynamic_cast to determine the actual parameter type (ITextParam,
		      ISelectionParam, etc.).
		\note The returned pointer is owned by the parameter set. Do not delete it.
		
		\code{.cpp}
		const iser::ISerializable* param = paramsSet->GetParameter("myParam");
		if (param)
		{
		    // Try casting to expected type
		    const iprm::ITextParam* textParam = dynamic_cast<const iprm::ITextParam*>(param);
		    if (textParam)
		    {
		        QString value = textParam->GetText();
		    }
		}
		\endcode
		
		\see GetEditableParameter
	*/
	virtual const iser::ISerializable* GetParameter(const QByteArray& id) const = 0;

	/**
		\brief Get access to editable parameter (read-write access).
		
		Retrieves a parameter by its ID for modification. This method returns NULL
		if the parameter does not exist or is not editable.
		
		\param id ID of parameter. This is an application-specific identifier that
		          uniquely identifies the parameter within this set.
		\return Pointer to editable parameter instance (as iser::ISerializable*) or
		        NULL if no editable parameter exists with the given ID.
		
		\note Returns NULL for read-only (calculated) parameters even if they exist.
		\note Use dynamic_cast to determine the actual parameter type before modifying.
		\note The returned pointer is owned by the parameter set. Do not delete it.
		
		\code{.cpp}
		iser::ISerializable* param = paramsSet->GetEditableParameter("threshold");
		if (param)
		{
		    // Try casting to expected type
		    iprm::ITextParam* textParam = dynamic_cast<iprm::ITextParam*>(param);
		    if (textParam)
		    {
		        textParam->SetText("10.5");
		    }
		}
		else
		{
		    // Parameter doesn't exist or is read-only
		}
		\endcode
		
		\see GetParameter
	*/
	virtual iser::ISerializable* GetEditableParameter(const QByteArray& id) = 0;
};


typedef istd::TUniqueInterfacePtr<iprm::IParamsSet> IParamsSetUniquePtr;
typedef istd::TSharedInterfacePtr<iprm::IParamsSet> IParamsSetSharedPtr;


} // namespace iprm




