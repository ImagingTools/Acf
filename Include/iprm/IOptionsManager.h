#pragma once


// ACF includes
#include <iprm/IOptionsList.h>
#include <iprm/ISelectionParam.h>


namespace iprm
{


/**
	\brief Common interface for the management of dynamic selection constraints.
	
	IOptionsManager extends IOptionsList with capabilities to dynamically modify
	the options list. It allows adding, removing, renaming, enabling/disabling,
	and reordering options at runtime.
	
	This interface also combines ISelectionParam functionality, allowing both
	management of options and selection of an active option.
	
	\section ioptionsmanager_usage Usage Example
	
	\code{.cpp}
	#include <iprm/COptionsManager.h>
	
	// Create options manager
	iprm::COptionsManager manager;
	
	// Check capabilities
	int flags = manager.GetOptionOperationFlags(-1);
	bool canInsert = (flags & iprm::IOptionsManager::OOF_SUPPORT_INSERT) != 0;
	bool canDelete = (flags & iprm::IOptionsManager::OOF_SUPPORT_DELETE) != 0;
	bool canRename = (flags & iprm::IOptionsManager::OOF_SUPPORT_RENAME) != 0;
	
	// Insert options
	if (canInsert)
	{
	    manager.InsertOption("Option A", "opt_a", "First option", -1);
	    manager.InsertOption("Option B", "opt_b", "Second option", -1);
	    manager.InsertOption("Option C", "opt_c", "Third option", -1);
	}
	
	// Get options count
	int count = manager.GetOptionsCount();
	qDebug() << "Total options:" << count;
	
	// Modify an option
	if (canRename)
	{
	    manager.SetOptionName(0, "Updated Option A");
	    manager.SetOptionDescription(0, "Updated description");
	}
	
	// Enable/disable option
	manager.SetOptionEnabled(1, false);  // Disable second option
	
	// Select an option
	manager.SetSelectedOptionIndex(0);
	int selected = manager.GetSelectedOptionIndex();
	
	// Remove an option
	if (canDelete)
	{
	    manager.RemoveOption(2);
	}
	
	// Swap options
	manager.SwapOptions(0, 1);
	\endcode
	
	\section ioptionsmanager_iteration Iterating with State Check
	
	\code{.cpp}
	for (int i = 0; i < manager.GetOptionsCount(); ++i)
	{
	    QString name = manager.GetOptionName(i);
	    bool enabled = manager.IsOptionEnabled(i);
	    bool selected = (i == manager.GetSelectedOptionIndex());
	    
	    qDebug() << name 
	             << (enabled ? "[enabled]" : "[disabled]")
	             << (selected ? "[SELECTED]" : "");
	}
	\endcode
	
	\note Inherits from both IOptionsList (read) and ISelectionParam (selection).
	\note Check operation flags before attempting modifications.
	
	\see IOptionsList, ISelectionParam, COptionsManager
*/
class IOptionsManager: virtual public IOptionsList, virtual public ISelectionParam
{
public:
	/**
		\brief Data model change notification flags.
		
		Used with change notification to detect when options are added or removed.
		
		\see istd::IChangeable::ChangeSet, IOptionsList::ChangeFlags
	*/
	enum ChangeFlags
	{
		CF_OPTION_ADDED = 0x26be73a,  ///< An option was added to the list
		CF_OPTION_REMOVED             ///< An option was removed from the list
	};
	
	/**
		\brief Bitwise coded flags used to find out supported features.
		
		These flags indicate what operations are supported by the options manager
		or individual options.
		
		\see GetOptionOperationFlags
	*/
	enum OptionOperationFlags
	{
		/**
			Active if number of options is fixed.
			When set, insert and delete operations are not allowed.
		*/
		OOF_COUNT_FIXED = 1,

		/**
			Active if rename of options is supported.
			When set, SetOptionName() can be used to rename options.
		*/
		OOF_SUPPORT_RENAME = 2,

		/**
			Active if insert of options is possible.
			When set, InsertOption() can add new options.
		*/
		OOF_SUPPORT_INSERT = 4,

		/**
			Active if delete of options is possible.
			When set, RemoveOption() can delete options.
		*/
		OOF_SUPPORT_DELETE = 8,

		/**
			Active if swap of options is possible.
			When set, SwapOptions() can reorder options.
		*/
		OOF_SUPPORT_SWAP = 16,

		/**
			Active if it is possible to have disabled options.
			When set, options can be in disabled state.
		*/
		OOF_DISABLE_ALLOWED = 32,

		/**
			Active if enabling and disabling of options is possible.
			When set, SetOptionEnabled() can change option enable state.
		*/
		OOF_SUPPORT_ENABLING = 64
	};


	/**
		\brief Get operation control flags of some option or whole manager.
		
		Returns flags indicating which operations are supported, either for the
		entire manager (index < 0) or for a specific option.
		
		\param index Index addressing position in the option list.
		             - Negative value: Returns general manager-level flags.
		             - Zero or positive: Returns flags for specific option at that position.
		             - Value equal to count: Returns flags for potential insert at end.
		\return Bitwise OR combination of OptionOperationFlags values.
		
		\note TODO: Consider merging this with IOptionsList::GetOptionsFlags().
		
		\code{.cpp}
		// Check general manager capabilities
		int flags = manager.GetOptionOperationFlags(-1);
		if (flags & iprm::IOptionsManager::OOF_SUPPORT_INSERT)
		{
		    qDebug() << "Can insert new options";
		}
		
		// Check if first option can be renamed
		int firstFlags = manager.GetOptionOperationFlags(0);
		if (firstFlags & iprm::IOptionsManager::OOF_SUPPORT_RENAME)
		{
		    manager.SetOptionName(0, "New Name");
		}
		\endcode
		
		\see OptionOperationFlags
	*/
	virtual int GetOptionOperationFlags(int index = -1) const = 0;	// TODO: Merge this with iprm::IOptionsList::GetOptionsFlags()

	/**
		\brief Enables or disables a given option.
		
		Changes the enabled state of an option. Only enabled options can be selected.
		
		\param index Option index (0-based).
		\param isEnabled true to enable, false to disable.
		\return true if operation succeeded, false otherwise.
		
		\note Check OOF_SUPPORT_ENABLING flag before calling.
		\note Triggers change notification on success.
		
		\code{.cpp}
		// Disable second option
		bool success = manager.SetOptionEnabled(1, false);
		if (success)
		{
		    qDebug() << "Option disabled";
		}
		
		// Re-enable it
		manager.SetOptionEnabled(1, true);
		\endcode
		
		\see IsOptionEnabled, OOF_SUPPORT_ENABLING
	*/
	virtual bool SetOptionEnabled(int index, bool isEnabled = true) = 0;

	/**
		\brief Remove an option at the given index.
		
		Deletes the option at the specified position from the list.
		
		\param index Option index (0-based).
		\return true if the operation was successful, false otherwise.
		
		\note Check OOF_SUPPORT_DELETE flag before calling.
		\note If the removed option was selected, selection may be cleared or moved.
		\note Triggers CF_OPTION_REMOVED notification on success.
		
		\code{.cpp}
		int flags = manager.GetOptionOperationFlags(-1);
		if (flags & iprm::IOptionsManager::OOF_SUPPORT_DELETE)
		{
		    bool removed = manager.RemoveOption(2);
		    if (removed)
		    {
		        qDebug() << "Option removed successfully";
		    }
		}
		\endcode
		
		\see InsertOption, OOF_SUPPORT_DELETE, CF_OPTION_REMOVED
	*/
	virtual bool RemoveOption(int index) = 0;

	/**
		\brief Insert an option at some position.
		
		Adds a new option to the list at the specified position.
		
		\param optionName Name of the option (display text).
		\param optionId ID of the option. According to data model constraints, this
		                needs to be unique if SCF_SUPPORT_UNIQUE_ID flag is set.
		\param optionDescription Description/tooltip text for the option (optional).
		\param index Position where the new option will be inserted.
		             - Negative value: Option will be added to the end of the list.
		             - Zero or positive: Insert at specified position.
		\return true if the operation was successful, false otherwise.
		
		\note Check OOF_SUPPORT_INSERT flag before calling.
		\note Triggers CF_OPTION_ADDED notification on success.
		
		\code{.cpp}
		int flags = manager.GetOptionOperationFlags(-1);
		if (flags & iprm::IOptionsManager::OOF_SUPPORT_INSERT)
		{
		    // Add to end
		    bool added = manager.InsertOption(
		        "New Option",
		        "opt_new",
		        "Description of new option",
		        -1);
		    
		    if (added)
		    {
		        qDebug() << "Option added";
		    }
		    
		    // Insert at specific position
		    manager.InsertOption("First Option", "opt_first", "", 0);
		}
		\endcode
		
		\see RemoveOption, OOF_SUPPORT_INSERT, CF_OPTION_ADDED
	*/
	virtual bool InsertOption(
				const QString& optionName,
				const QByteArray& optionId,
				const QString& optionDescription = QString(),
				int index = -1) = 0;

	/**
		\brief Swap two options.
		
		Exchanges the positions of two options in the list.
		
		\param index1 Index of first option.
		\param index2 Index of second option.
		\return true if swap succeeded, false otherwise.
		
		\note Check OOF_SUPPORT_SWAP flag before calling.
		\note Option IDs remain stable; only list positions change.
		
		\code{.cpp}
		// Swap first and third options
		int flags = manager.GetOptionOperationFlags(-1);
		if (flags & iprm::IOptionsManager::OOF_SUPPORT_SWAP)
		{
		    bool swapped = manager.SwapOptions(0, 2);
		    if (swapped)
		    {
		        qDebug() << "Options swapped";
		    }
		}
		\endcode
		
		\see OOF_SUPPORT_SWAP
	*/
	virtual bool SwapOptions(int index1, int index2) = 0;

	/**
		\brief Set a new name for the option at the given index.
		
		Changes the display name of an option.
		
		\param optionIndex Option index (0-based).
		\param optionName New name for the option.
		\return true if rename succeeded, false otherwise.
		
		\note Check OOF_SUPPORT_RENAME flag before calling.
		\note Triggers CF_OPTION_RENAMED notification on success.
		
		\code{.cpp}
		int flags = manager.GetOptionOperationFlags(0);
		if (flags & iprm::IOptionsManager::OOF_SUPPORT_RENAME)
		{
		    bool renamed = manager.SetOptionName(0, "Updated Name");
		    if (renamed)
		    {
		        qDebug() << "Option renamed";
		    }
		}
		\endcode
		
		\see GetOptionName, OOF_SUPPORT_RENAME, CF_OPTION_RENAMED
	*/
	virtual bool SetOptionName(int optionIndex, const QString& optionName) = 0;

	/**
		\brief Set a new description for the option at the given index.
		
		Changes the description/tooltip text of an option.
		
		\param optionIndex Option index (0-based).
		\param optionDescription New description for the option.
		\return true if operation succeeded, false otherwise.
		
		\code{.cpp}
		bool success = manager.SetOptionDescription(0, "Updated description text");
		if (success)
		{
		    qDebug() << "Description updated";
		}
		\endcode
		
		\see GetOptionDescription
	*/
	virtual bool SetOptionDescription(int optionIndex, const QString& optionDescription) = 0;
};


} // namespace iprm




