// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtCore/QByteArray>

// ACF includes
#include <iprm/IParamsSet.h>
#include <iprm/ISelectionParam.h>


namespace iprm
{


/**
	\brief Manager of parameter sets.
	
	IParamsManager manages a collection of parameter sets (IParamsSet instances).
	It provides operations for creating, deleting, renaming, and organizing multiple
	parameter sets. The manager also acts as a selection parameter, allowing selection
	of the active parameter set.
	
	The manager supports various operation modes controlled by flags:
	- Fixed or dynamic number of parameter sets
	- Renameable parameter sets
	- Insertable/deletable parameter sets
	- Swappable parameter sets
	- Enable/disable parameter sets
	
	\section iparamsmanager_usage Usage Example
	
	\code{.cpp}
	// Assuming manager is obtained from a component
	iprm::IParamsManager* manager = component->GetInterface<iprm::IParamsManager>("ParamsManager");
	
	// Check capabilities
	int flags = manager->GetIndexOperationFlags(-1);
	bool canInsert = (flags & iprm::IParamsManager::MF_SUPPORT_INSERT) != 0;
	bool canDelete = (flags & iprm::IParamsManager::MF_SUPPORT_DELETE) != 0;
	bool canRename = (flags & iprm::IParamsManager::MF_SUPPORT_RENAME) != 0;
	bool canSwap = (flags & iprm::IParamsManager::MF_SUPPORT_SWAP) != 0;
	
	// Get current number of parameter sets
	int count = manager->GetParamsSetsCount();
	qDebug() << "Parameter sets:" << count;
	
	// Insert a new parameter set
	if (canInsert)
	{
	    int newIndex = manager->InsertParamsSet();
	    if (newIndex >= 0)
	    {
	        manager->SetParamsSetName(newIndex, "New Configuration");
	        manager->SetParamsSetDescription(newIndex, "Custom settings");
	    }
	}
	
	// Iterate through all parameter sets
	for (int i = 0; i < count; ++i)
	{
	    QString name = manager->GetParamsSetName(i);
	    iprm::IParamsSet* paramsSet = manager->GetParamsSet(i);
	    
	    // Work with the parameter set
	    iprm::IParamsSet::Ids ids = paramsSet->GetParamIds();
	    // ...
	}
	
	// Select active parameter set
	manager->SetSelectedOptionIndex(0);
	int activeIndex = manager->GetSelectedOptionIndex();
	
	// Remove a parameter set
	if (canDelete)
	{
	    manager->RemoveParamsSet(2);
	}
	
	// Swap two parameter sets
	if (canSwap)
	{
	    manager->SwapParamsSet(0, 1);
	}
	\endcode
	
	\note The manager inherits from ISelectionParam, allowing selection of active parameter set.
	\note Always check operation flags before attempting modifications.
	
	\see IParamsSet, ISelectionParam, IOptionsList
*/
class IParamsManager: virtual public iprm::ISelectionParam
{
public:
	typedef QSet<QByteArray> TypeIds;

	/**
		\brief Bitwise coded flags used to find out supported features.
		
		These flags indicate what operations are supported by the parameter manager
		or individual parameter sets. Use GetIndexOperationFlags() to retrieve these flags.
		
		\see GetIndexOperationFlags, SetIndexOperationFlags
	*/
	enum ParamsOperationFlags
	{
		/**
			Active if number of parameters is fixed.
			When set, insert and delete operations are not allowed.
		*/
		MF_COUNT_FIXED = 1,

		/**
			Active if rename of parameters is supported.
			When set, SetParamsSetName() can be used to rename parameter sets.
		*/
		MF_SUPPORT_RENAME = 2,

		/**
			Active if insert of parameters is possible.
			When set, InsertParamsSet() can add new parameter sets.
		*/
		MF_SUPPORT_INSERT = 4,

		/**
			Active if delete of parameters is possible.
			When set, RemoveParamsSet() can delete parameter sets.
		*/
		MF_SUPPORT_DELETE = 8,

		/**
			Active if swap of parameters with the other one is possible.
			When set, SwapParamsSet() can reorder parameter sets.
		*/
		MF_SUPPORT_SWAP = 16,

		/**
			Active if it is possible to get the disabled option.
			When set, parameter sets can be in disabled state.
		*/
		MF_DISABLE_ALLOWED = 32,

		/**
			Active if it is possible to edit parameter values.
			When set, parameters within the set can be modified.
		*/
		MF_SUPPORT_EDIT = 64,

		/**
			Option is inactive (i.e., cannot be activated or selected).
			When set, the parameter set cannot be selected as active.
		*/
		MF_INACTIVE = 128
	};

	/**
		\brief Possible changes of the manager's data model.
		
		These change flags are used with the change notification system to indicate
		what type of modification occurred in the parameter manager.
		
		\see istd::IChangeable::ChangeSet
	*/
	enum ChangeFlags
	{
		CF_SET_INSERTED = 0x5af60,  ///< A parameter set was inserted
		CF_SET_REMOVED,             ///< A parameter set was removed
		CF_SET_NAME_CHANGED,        ///< A parameter set name was changed
		CF_SET_ENABLE_CHANGED       ///< A parameter set enable state was changed
	};


	/**
		\brief Get operation control flags of some parameter set or whole manager.
		
		This method returns flags indicating which operations are supported, either
		for the entire manager (index < 0) or for a specific parameter set.
		
		\param index Index addressing position in parameter set list.
		             - Negative value: Returns general manager-level flags.
		             - Zero or positive: Returns flags for specific parameter set at that position.
		             - Value equal to count: Returns flags for potential insert at end position.
		
		\return Bitwise OR combination of ParamsOperationFlags values.
		
		\code{.cpp}
		// Check general manager capabilities
		int managerFlags = manager->GetIndexOperationFlags(-1);
		if (managerFlags & iprm::IParamsManager::MF_SUPPORT_INSERT)
		{
		    qDebug() << "Can insert new parameter sets";
		}
		
		// Check if first parameter set can be renamed
		int firstSetFlags = manager->GetIndexOperationFlags(0);
		if (firstSetFlags & iprm::IParamsManager::MF_SUPPORT_RENAME)
		{
		    manager->SetParamsSetName(0, "New Name");
		}
		
		// Check if we can insert at the end
		int count = manager->GetParamsSetsCount();
		int insertFlags = manager->GetIndexOperationFlags(count);
		if (insertFlags & iprm::IParamsManager::MF_SUPPORT_INSERT)
		{
		    manager->InsertParamsSet(-1, count);
		}
		\endcode
		
		\see ParamsOperationFlags, SetIndexOperationFlags
	*/
	virtual int GetIndexOperationFlags(int index = -1) const = 0;

	/**
		\brief Set operation control flags of some parameter-set.
		
		Sets operation flags for a specific parameter set. Only flags related to a single
		parameter set can be modified (not manager-level flags).
		
		\param flags Operation flags to be used (combination of ParamsOperationFlags).
		\param index Index addressing position in parameter set list.
		\return true if the operation succeeded, false otherwise.
		
		\note This method can only set per-set flags, not manager-level flags.
		
		\code{.cpp}
		// Try to make a parameter set editable
		bool success = manager->SetIndexOperationFlags(0, 
		    iprm::IParamsManager::MF_SUPPORT_EDIT);
		\endcode
		
		\see ParamsOperationFlags, GetIndexOperationFlags
	*/
	virtual bool SetIndexOperationFlags(int index, int flags) = 0;

	/**
		\brief Get number of managed parameter sets.
		
		\return The count of parameter sets currently managed by this manager.
		
		\code{.cpp}
		int count = manager->GetParamsSetsCount();
		qDebug() << "Managing" << count << "parameter sets";
		\endcode
	*/
	virtual int GetParamsSetsCount() const = 0;

	/**
		\brief Get selection constraints describing each supported type.
		
		Returns an options list that describes the different types of parameter sets
		that can be created. This is used when inserting new parameter sets to determine
		available types.
		
		\return Pointer to IOptionsList describing available types, or NULL if only one
		        anonymous type is supported.
		
		\code{.cpp}
		const iprm::IOptionsList* types = manager->GetParamsTypeConstraints();
		if (types)
		{
		    int typeCount = types->GetOptionsCount();
		    for (int i = 0; i < typeCount; ++i)
		    {
		        QString typeName = types->GetOptionName(i);
		        QByteArray typeId = types->GetOptionId(i);
		        qDebug() << "Type" << i << ":" << typeName;
		    }
		}
		\endcode
		
		\see InsertParamsSet
	*/
	virtual const IOptionsList* GetParamsTypeConstraints() const = 0;

	/**
		\brief Insert new parameter set at selected position.
		
		Creates and inserts a new parameter set into the manager. The type and position
		can be specified, or automatic selection can be used.
		
		\param typeIndex Index of parameter type from GetParamsTypeConstraints().
		                 - Negative value: Type will be automatically selected (default).
		                 - Zero or positive: Use specified type index.
		\param index Position in list where parameter set should be inserted.
		             - Negative value: Use default position (typically at end).
		             - Zero or positive: Insert at specified position.
		\return Position of the newly inserted parameter set, or negative value if
		        insertion was not possible.
		
		\note For specified index positions (non-negative index), returns the same index
		      on success or negative value on failure.
		\note Check MF_SUPPORT_INSERT flag before calling.
		
		\code{.cpp}
		// Insert at default position with automatic type selection
		int newIndex = manager->InsertParamsSet();
		if (newIndex >= 0)
		{
		    qDebug() << "Inserted at index" << newIndex;
		    manager->SetParamsSetName(newIndex, "New Set");
		}
		
		// Insert at specific position with specific type
		const iprm::IOptionsList* types = manager->GetParamsTypeConstraints();
		if (types && types->GetOptionsCount() > 1)
		{
		    int typeIdx = 1;
		    int position = 2;
		    int inserted = manager->InsertParamsSet(typeIdx, position);
		}
		\endcode
		
		\see GetParamsTypeConstraints, RemoveParamsSet, MF_SUPPORT_INSERT
	*/
	virtual int InsertParamsSet(int typeIndex = -1, int index = -1) = 0;

	/**
		\brief Remove parameter set at selected position.
		
		Deletes the parameter set at the specified index.
		
		\param index Position of the parameter set to remove (0-based).
		\return true if removal succeeded, false otherwise.
		
		\note Check MF_SUPPORT_DELETE flag before calling.
		
		\code{.cpp}
		int flags = manager->GetIndexOperationFlags(-1);
		if (flags & iprm::IParamsManager::MF_SUPPORT_DELETE)
		{
		    bool removed = manager->RemoveParamsSet(2);
		    if (removed)
		    {
		        qDebug() << "Parameter set removed successfully";
		    }
		}
		\endcode
		
		\see InsertParamsSet, MF_SUPPORT_DELETE
	*/
	virtual bool RemoveParamsSet(int index) = 0;

	/**
		\brief Swap two parameter sets.
		
		Exchanges the positions of two parameter sets in the manager's list.
		
		\param index1 Index of first parameter set.
		\param index2 Index of second parameter set.
		\return true if swap succeeded, false otherwise.
		
		\note Check MF_SUPPORT_SWAP flag before calling.
		
		\code{.cpp}
		// Swap first and third parameter sets
		int flags = manager->GetIndexOperationFlags(-1);
		if (flags & iprm::IParamsManager::MF_SUPPORT_SWAP)
		{
		    bool swapped = manager->SwapParamsSet(0, 2);
		    if (swapped)
		    {
		        qDebug() << "Parameter sets swapped";
		    }
		}
		\endcode
		
		\see MF_SUPPORT_SWAP
	*/
	virtual bool SwapParamsSet(int index1, int index2) = 0;

	/**
		\brief Get selected parameter set.
		
		Retrieves the parameter set at the specified index.
		
		\param index Index of the parameter set (0-based).
		\return Pointer to the parameter set, or NULL if index is out of range.
		
		\note The returned pointer is owned by the manager. Do not delete it.
		
		\code{.cpp}
		int count = manager->GetParamsSetsCount();
		for (int i = 0; i < count; ++i)
		{
		    iprm::IParamsSet* paramSet = manager->GetParamsSet(i);
		    if (paramSet)
		    {
		        // Access parameters in the set
		        iprm::IParamsSet::Ids ids = paramSet->GetParamIds();
		        for (const QByteArray& id : ids)
		        {
		            const iser::ISerializable* param = paramSet->GetParameter(id);
		            // Process parameter...
		        }
		    }
		}
		\endcode
		
		\see GetParamsSetsCount, IParamsSet
	*/
	virtual IParamsSet* GetParamsSet(int index) const = 0;

	/**
		\brief Create a new parameter set instance.
		
		Creates a new, independent parameter set instance. Optionally, data from an
		existing parameter set can be copied to the new instance.
		
		\param typeIndex Index of parameter type from GetParamsTypeConstraints().
		                 - Negative value: Type will be automatically selected (default).
		                 - Zero or positive: Use specified type index.
		\param index If non-negative, the parameter set data at the given position will
		             be copied to the created instance.
		
		\return Unique pointer to the newly created parameter set.
		
		\note The caller is responsible for the memory management of the created parameter set.
		\note The returned parameter set is independent and not managed by this manager.
		
		\code{.cpp}
		// Create a new empty parameter set
		iprm::IParamsSetUniquePtr newSet = manager->CreateParameterSet();
		
		// Create a copy of existing parameter set
		iprm::IParamsSetUniquePtr copy = manager->CreateParameterSet(-1, 0);
		
		// Create a new parameter set of specific type
		const iprm::IOptionsList* types = manager->GetParamsTypeConstraints();
		if (types && types->GetOptionsCount() > 1)
		{
		    iprm::IParamsSetUniquePtr typedSet = manager->CreateParameterSet(1);
		}
		\endcode
		
		\see GetParamsTypeConstraints
	*/
	virtual iprm::IParamsSetUniquePtr CreateParameterSet(int typeIndex = -1, int index = -1) const = 0;

	/**
		\brief Get name of specified parameter set.
		
		\param index Index of the parameter set (0-based).
		\return Name of the parameter set as QString.
		
		\code{.cpp}
		int count = manager->GetParamsSetsCount();
		for (int i = 0; i < count; ++i)
		{
		    QString name = manager->GetParamsSetName(i);
		    qDebug() << "Set" << i << ":" << name;
		}
		\endcode
		
		\see SetParamsSetName
	*/
	virtual QString GetParamsSetName(int index) const = 0;

	/**
		\brief Set name of specified parameter set.
		
		Changes the display name of a parameter set.
		
		\param index Index of the parameter set (0-based).
		\param name New name for the parameter set.
		\return true if name was changed successfully, false otherwise.
		
		\note Check MF_SUPPORT_RENAME flag before calling.
		
		\code{.cpp}
		int flags = manager->GetIndexOperationFlags(0);
		if (flags & iprm::IParamsManager::MF_SUPPORT_RENAME)
		{
		    bool renamed = manager->SetParamsSetName(0, "Updated Name");
		    if (renamed)
		    {
		        qDebug() << "Name changed successfully";
		    }
		}
		\endcode
		
		\see GetParamsSetName, MF_SUPPORT_RENAME
	*/
	virtual bool SetParamsSetName(int index, const QString& name) = 0;

	/**
		\brief Get the description of the specified parameter set.
		
		\param index Index of the parameter set (0-based).
		\return Description of the parameter set as QString.
		
		\code{.cpp}
		QString description = manager->GetParamsSetDescription(0);
		qDebug() << "Description:" << description;
		\endcode
		
		\see SetParamsSetDescription
	*/
	virtual QString GetParamsSetDescription(int index) const = 0;

	/**
		\brief Set the description of the specified parameter set.
		
		Changes the description/tooltip text for a parameter set.
		
		\param index Index of the parameter set (0-based).
		\param description New description text for the parameter set.
		
		\code{.cpp}
		manager->SetParamsSetDescription(0, "This set contains advanced options");
		\endcode
		
		\see GetParamsSetDescription
	*/
	virtual void SetParamsSetDescription(int index, const QString& description) = 0;
};


typedef istd::TUniqueInterfacePtr<IParamsManager> IParamsManagerUniquePtr;
typedef istd::TSharedInterfacePtr<IParamsManager> IParamsManagerSharedPtr;


} // namespace iprm


