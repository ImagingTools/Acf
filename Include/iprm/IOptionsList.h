// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QString>


// ACF includes
#include <istd/IChangeable.h>

#include <iprm/iprm.h>


namespace iprm
{


/**
	\brief Constraints of selection from set of possibilities.
	
	IOptionsList provides a read-only interface to a list of options that can be
	used as constraints for selection parameters. Each option has a name, optional
	description, and optional unique ID.
	
	This interface is used by ISelectionParam to define available choices. The list
	supports change notifications so observers can react to option list modifications.
	
	\section ioptionslist_usage Usage Example
	
	\code{.cpp}
	// Assuming optionsList is obtained from a selection parameter
	const iprm::IOptionsList* options = selectionParam.GetSelectionConstraints();
	
	if (options)
	{
	    // Get flags
	    int flags = options->GetOptionsFlags();
	    bool hasUniqueIds = (flags & iprm::IOptionsList::SCF_SUPPORT_UNIQUE_ID) != 0;
	    bool canDisable = (flags & iprm::IOptionsList::SFC_DISABLE_ALLOWED) != 0;
	    
	    // Get number of options
	    int count = options->GetOptionsCount();
	    qDebug() << "Available options:" << count;
	    
	    // Iterate through options
	    for (int i = 0; i < count; ++i)
	    {
	        QString name = options->GetOptionName(i);
	        QString desc = options->GetOptionDescription(i);
	        QByteArray id = options->GetOptionId(i);
	        bool enabled = options->IsOptionEnabled(i);
	        
	        qDebug() << i << ":" << name;
	        qDebug() << "   ID:" << id;
	        qDebug() << "   Description:" << desc;
	        qDebug() << "   Enabled:" << enabled;
	    }
	}
	\endcode
	
	\section ioptionslist_helper Helper Functions
	
	The iprm namespace provides helper functions for working with options lists:
	
	\code{.cpp}
	// Find option index by ID
	int index = iprm::FindOptionIndexById("option_id", *options);
	if (index >= 0)
	{
	    QString name = options->GetOptionName(index);
	    qDebug() << "Found option:" << name;
	}
	else
	{
	    qDebug() << "Option not found";
	}
	\endcode
	
	\note This is a read-only interface. For dynamic option management, see IOptionsManager.
	\note Inherits from istd::IChangeable for change notification support.
	
	\see IOptionsManager, ISelectionParam, FindOptionIndexById
*/
class IOptionsList: virtual public istd::IChangeable
{
public:
	/**
		\brief Flags for changes related to this interface.
		
		Used with change notification to detect modifications to the options list.
		
		\see istd::IChangeable::ChangeSet
	*/
	enum ChangeFlags
	{
		/**
			Option list updated (options added, removed, or reordered).
		*/
		CF_OPTIONS_CHANGED = 0x8263d86,

		/**
			One or more options were renamed.
		*/
		CF_OPTION_RENAMED
	};

	/**
		\brief Flags for controlling constraints behavior.
		
		These flags describe capabilities and requirements of the options list.
		
		\see GetOptionsFlags
	*/
	enum OptionsFlags
	{
		SCF_NONE = 0,  ///< No special flags

		/**
			If set, every option must have a unique ID.
			Option IDs can be used for stable references to options.
		*/
		SCF_SUPPORT_UNIQUE_ID = 256,

		/**
			If set, some options can be disabled.
			Disabled options cannot be selected but remain in the list.
		*/
		SFC_DISABLE_ALLOWED = 32
	};

	/**
		\brief Get constraints flags.
		
		Returns flags that describe how the constraints object behaves and what
		features it supports.
		
		\return Bitwise OR combination of OptionsFlags values.
		
		\code{.cpp}
		int flags = options->GetOptionsFlags();
		
		if (flags & iprm::IOptionsList::SCF_SUPPORT_UNIQUE_ID)
		{
		    qDebug() << "Options have unique IDs";
		}
		
		if (flags & iprm::IOptionsList::SFC_DISABLE_ALLOWED)
		{
		    qDebug() << "Options can be disabled";
		}
		\endcode
		
		\see OptionsFlags
	*/
	virtual int GetOptionsFlags() const = 0;

	/**
		\brief Get number of managed options.
		
		\return Total count of options in the list (including disabled options).
		
		\code{.cpp}
		int count = options->GetOptionsCount();
		for (int i = 0; i < count; ++i)
		{
		    // Process each option
		}
		\endcode
		
		\see GetOptionName
	*/
	virtual int GetOptionsCount() const = 0;

	/**
		\brief Get name of specified option.
		
		Returns the display name for an option.
		
		\param index Option index (0-based).
		\return Option name as QString, or empty string if index is out of range.
		
		\code{.cpp}
		QString name = options->GetOptionName(0);
		qDebug() << "First option:" << name;
		\endcode
		
		\see GetOptionDescription, GetOptionId
	*/
	virtual QString GetOptionName(int index) const = 0;

	/**
		\brief Get human-readable description for an option.
		
		Returns detailed description or tooltip text for an option.
		
		\param index Option index (0-based).
		\return Option description as QString, or empty string if no description exists
		        or index is out of range.
		
		\code{.cpp}
		QString description = options->GetOptionDescription(2);
		if (!description.isEmpty())
		{
		    qDebug() << "Description:" << description;
		}
		\endcode
		
		\see GetOptionName
	*/
	virtual QString GetOptionDescription(int index) const = 0;

	/**
		\brief Get option ID.
		
		Returns the unique identifier for an option. The ID is stable and doesn't
		change when options are reordered, making it suitable for persistent references.
		
		\param index Option index (0-based).
		\return Option ID as QByteArray, or empty array if index is out of range.
		
		\note The ID must be unique if SCF_SUPPORT_UNIQUE_ID flag is set.
		\note Use FindOptionIndexById() to find an option by its ID.
		
		\code{.cpp}
		QByteArray id = options->GetOptionId(1);
		qDebug() << "Option ID:" << id;
		
		// Later, find the option by ID
		int index = iprm::FindOptionIndexById(id, *options);
		\endcode
		
		\see OptionsFlags, FindOptionIndexById
	*/
	virtual QByteArray GetOptionId(int index) const = 0;

	/**
		\brief Return true if the option is enabled and can be selected.
		
		Checks whether an option is currently enabled. Disabled options cannot be
		selected but remain visible in the list.
		
		\param index Option index (0-based).
		\return true if option is enabled and can be selected, false if disabled
		        or index is out of range.
		
		\code{.cpp}
		for (int i = 0; i < options->GetOptionsCount(); ++i)
		{
		    if (options->IsOptionEnabled(i))
		    {
		        QString name = options->GetOptionName(i);
		        qDebug() << "Enabled option:" << name;
		    }
		}
		\endcode
		
		\see SFC_DISABLE_ALLOWED
	*/
	virtual bool IsOptionEnabled(int index) const = 0;
};


// Helper methods

/**
	\brief Find the option index by the given ID.
	
	Searches through the options list to find an option with a matching ID.
	This is useful for converting stable option IDs to current indices.
	
	\param id ID to search for.
	\param list Options list to search in.
	\return Found index (0-based), or -1 if no option with the given ID was found.
	
	\code{.cpp}
	// Save option ID for later reference
	QByteArray savedId = options->GetOptionId(selectedIndex);
	
	// ... later, after options might have been reordered ...
	
	// Find the option again by its stable ID
	int currentIndex = iprm::FindOptionIndexById(savedId, *options);
	if (currentIndex >= 0)
	{
	    qDebug() << "Option found at index:" << currentIndex;
	    selectionParam.SetSelectedOptionIndex(currentIndex);
	}
	else
	{
	    qDebug() << "Option no longer exists";
	}
	\endcode
	
	\see IOptionsList::GetOptionId
*/
int FindOptionIndexById(const QByteArray& id, const iprm::IOptionsList& list);


} // namespace iprm




