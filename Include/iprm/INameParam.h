// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <iser/ISerializable.h>


namespace iprm
{


/**
	\brief Interface for an object containing a name.
	
	INameParam stores a human-readable name as a QString. It supports fixed (read-only)
	and editable names, making it suitable for both system-defined and user-defined
	naming scenarios.
	
	\section inameparam_usage Usage Example
	
	\code{.cpp}
	#include <iprm/CNameParam.h>
	
	// Create a name parameter
	iprm::CNameParam nameParam;
	
	// Set name
	nameParam.SetName("My Configuration");
	
	// Get name
	QString name = nameParam.GetName();
	qDebug() << "Name:" << name;
	
	// Check if name is fixed (read-only)
	bool isFixed = nameParam.IsNameFixed();
	if (!isFixed)
	{
	    nameParam.SetName("Updated Configuration");
	}
	\endcode
	
	\section inameparam_changenotification Change Notification Example
	
	\code{.cpp}
	// Monitoring name changes through change notification
	// (Requires observer implementation)
	
	// When name changes, CF_RENAME flag will be set in the change set
	if (changeSet.Contains(iprm::INameParam::CF_RENAME))
	{
	    QString newName = nameParam.GetName();
	    qDebug() << "Name changed to:" << newName;
	}
	\endcode
	
	\note The documentation mentions this interface should be renamed to ITextParam,
	      but currently serves as a named object interface.
	\note Inherits from iser::ISerializable for persistence support.
	
	\see CNameParam, ITextParam, IIdParam
*/
class INameParam: virtual public iser::ISerializable
{
public:
	/**
		\brief Data model change notification flags.
		
		Used with the observer pattern to detect when the name changes.
		
		\see istd::IChangeable::ChangeSet
	*/
	enum ChangeFlags
	{
		CF_RENAME = 0x15345e0  ///< Name has been changed
	};

	/**
		\brief Get the object name.
		
		\return Reference to the current name as QString.
		
		\code{.cpp}
		const QString& name = nameParam.GetName();
		qDebug() << "Name:" << name;
		\endcode
		
		\see SetName
	*/
	virtual const QString& GetName() const = 0;

	/**
		\brief Set the object name.
		
		Changes the name of this parameter.
		
		\param name New name value.
		
		\note If IsNameFixed() returns true, setting the name may be restricted.
		\note Triggers CF_RENAME notification on change.
		
		\code{.cpp}
		if (!nameParam.IsNameFixed())
		{
		    nameParam.SetName("New Name");
		}
		\endcode
		
		\see GetName, IsNameFixed, CF_RENAME
	*/
	virtual void SetName(const QString& name) = 0;

	/**
		\brief Return true if the name is fixed (non-editable).
		
		Indicates whether the name can be changed by users. Fixed names are typically
		used for system-defined or read-only objects.
		
		\return true if name cannot be modified, false if name is editable.
		
		\code{.cpp}
		bool canRename = !nameParam.IsNameFixed();
		if (canRename)
		{
		    // Allow user to edit name
		    nameParam.SetName(userInput);
		}
		\endcode
		
		\see SetName
	*/
	virtual bool IsNameFixed() const = 0;
};


} // namespace iprm


