#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QSet>

// ACF includes
#include <iser/ISerializable.h>
#include <iprm/iprm.h>


namespace iprm
{


/**
	\brief Represents a parameter which can change its type.
	
	IVariableParam provides a dynamic parameter that can hold different types of
	parameters at runtime. It's useful for scenarios where the parameter type needs
	to be determined or changed based on context, user selection, or configuration.
	
	Each type is identified by a unique QByteArray type ID. The parameter can be
	assigned different types, and the current parameter instance is accessible through
	the interface.
	
	\section ivariableparam_usage Usage Example
	
	\code{.cpp}
	#include <iprm/CVariableParamComp.h>
	
	// Assuming variableParam is obtained from a component
	iprm::IVariableParam* variableParam = /* ... */;
	
	// Get list of supported type IDs
	iprm::IVariableParam::TypeIds knownTypes = variableParam->GetKnownTypeIds();
	for (const QByteArray& typeId : knownTypes)
	{
	    qDebug() << "Supported type:" << typeId;
	}
	
	// Assign a specific type
	bool assigned = variableParam->AssignTypeId("text_param");
	if (assigned)
	{
	    // Get the current parameter instance
	    iser::ISerializable* param = variableParam->GetParameter();
	    
	    // Cast to expected type
	    iprm::ITextParam* textParam = dynamic_cast<iprm::ITextParam*>(param);
	    if (textParam)
	    {
	        textParam->SetText("Value");
	    }
	}
	
	// Check current type
	QByteArray currentType = variableParam->GetParameterTypeId();
	if (!currentType.isEmpty())
	{
	    qDebug() << "Current type:" << currentType;
	}
	
	// Change to different type
	variableParam->AssignTypeId("selection_param");
	iser::ISerializable* newParam = variableParam->GetParameter();
	iprm::ISelectionParam* selectionParam = dynamic_cast<iprm::ISelectionParam*>(newParam);
	if (selectionParam)
	{
	    selectionParam->SetSelectedOptionIndex(0);
	}
	\endcode
	
	\note When type is changed, the previous parameter instance is typically destroyed.
	\note Inherits from iser::ISerializable for persistence support.
	\note Useful for plugin systems or configurable parameter types.
	
	\see CVariableParamComp
*/
class IVariableParam: virtual public iser::ISerializable
{
public:
	typedef QSet<QByteArray> TypeIds;

	/**
		\brief Get list of all known type IDs.
		
		Returns the set of type identifiers that this variable parameter can be
		assigned to.
		
		\return Set of QByteArray type IDs representing available parameter types.
		
		\code{.cpp}
		iprm::IVariableParam::TypeIds types = variableParam->GetKnownTypeIds();
		qDebug() << "Available types:" << types.size();
		
		for (const QByteArray& typeId : types)
		{
		    qDebug() << "  -" << typeId;
		}
		\endcode
		
		\see AssignTypeId
	*/
	virtual TypeIds GetKnownTypeIds() const = 0;

	/**
		\brief Get type ID of current stored parameter.
		
		Returns the type identifier of the parameter currently instantiated.
		
		\return Type ID of current parameter, or empty QByteArray if no type is assigned.
		
		\code{.cpp}
		QByteArray currentType = variableParam->GetParameterTypeId();
		if (currentType.isEmpty())
		{
		    qDebug() << "No type assigned yet";
		}
		else
		{
		    qDebug() << "Current type:" << currentType;
		}
		\endcode
		
		\see AssignTypeId, GetParameter
	*/
	virtual QByteArray GetParameterTypeId() const = 0;
	/**
		\brief Get current parameter instance.
		
		Returns a pointer to the actual parameter object currently instantiated.
		The returned pointer must be cast to the appropriate interface based on
		the current type ID.
		
		\return Pointer to current parameter instance (as iser::ISerializable*),
		        or NULL if no type is assigned.
		
		\note The returned pointer is owned by the variable parameter. Do not delete.
		\note Use GetParameterTypeId() to determine the type before casting.
		
		\code{.cpp}
		iser::ISerializable* param = variableParam->GetParameter();
		if (param)
		{
		    QByteArray typeId = variableParam->GetParameterTypeId();
		    
		    if (typeId == "text_param")
		    {
		        iprm::ITextParam* textParam = dynamic_cast<iprm::ITextParam*>(param);
		        if (textParam)
		        {
		            QString text = textParam->GetText();
		        }
		    }
		    else if (typeId == "selection_param")
		    {
		        iprm::ISelectionParam* selParam = dynamic_cast<iprm::ISelectionParam*>(param);
		        if (selParam)
		        {
		            int index = selParam->GetSelectedOptionIndex();
		        }
		    }
		}
		\endcode
		
		\see GetParameterTypeId, AssignTypeId
	*/
	virtual iser::ISerializable* GetParameter() const = 0;

	/**
		\brief Assign type ID to this object.
		
		Changes the type of this variable parameter by creating an internal instance
		of the specified parameter type.
		
		\param typeId Type identifier from the set of known types. Must be one of the
		              IDs returned by GetKnownTypeIds().
		\return true if type assignment succeeded, false if typeId is not recognized
		        or assignment failed.
		
		\note Assigning a new type typically destroys the previous parameter instance.
		\note The new parameter instance is accessible via GetParameter().
		
		\code{.cpp}
		// Assign to text parameter type
		if (variableParam->AssignTypeId("text_param"))
		{
		    iser::ISerializable* param = variableParam->GetParameter();
		    iprm::ITextParam* textParam = dynamic_cast<iprm::ITextParam*>(param);
		    if (textParam)
		    {
		        textParam->SetText("Initial value");
		    }
		}
		
		// Switch to different type
		if (variableParam->AssignTypeId("numeric_param"))
		{
		    // Previous text parameter is now destroyed
		    // New numeric parameter is created
		}
		\endcode
		
		\see GetKnownTypeIds, GetParameterTypeId, GetParameter
	*/
	virtual bool AssignTypeId(const QByteArray& typeId) = 0;
};


} // namespace iprm




