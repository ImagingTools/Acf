#pragma once


// Qt includes
#include <QtCore/QString>


// ACF includes
#include <iser/ISerializable.h>


namespace iprm
{


class IOptionsList;


/**
	\brief Interface allowing to select single option from list of options.
	
	ISelectionParam provides a mechanism for selecting one option from a list of
	available choices. It's similar to a combo box or radio button group in UI terms.
	
	The selection is constrained by an IOptionsList that defines available options.
	Each option can have sub-selections, enabling hierarchical selection structures
	(e.g., selecting a category, then a sub-category).
	
	\section iselectionparam_usage Usage Example
	
	\code{.cpp}
	#include <iprm/CSelectionParam.h>
	#include <iprm/COptionsManager.h>
	
	// Create options manager
	iprm::COptionsManager options;
	options.InsertOption("Small", "size_small", "Small size");
	options.InsertOption("Medium", "size_medium", "Medium size");
	options.InsertOption("Large", "size_large", "Large size");
	
	// Create selection parameter
	iprm::CSelectionParam selection;
	selection.SetSelectionConstraints(&options);
	
	// Set selection by index
	selection.SetSelectedOptionIndex(1);  // Select "Medium"
	
	// Get selected index
	int index = selection.GetSelectedOptionIndex();  // Returns 1
	
	// Check for no selection
	if (index == iprm::ISelectionParam::NO_SELECTION)
	{
	    qDebug() << "Nothing selected";
	}
	
	// Get the constraints to examine options
	const iprm::IOptionsList* constraints = selection.GetSelectionConstraints();
	QString selectedName = constraints->GetOptionName(index);
	\endcode
	
	\section iselectionparam_hierarchical Hierarchical Selection Example
	
	\code{.cpp}
	// Main selection
	iprm::CSelectionParam mainSelection;
	mainSelection.SetSelectionConstraints(&mainOptions);
	
	// Sub-selection for first option
	iprm::CSelectionParam* subSelection = new iprm::CSelectionParam();
	subSelection->SetSelectionConstraints(&subOptions);
	mainSelection.SetSubselection(0, subSelection);
	
	// Select main option
	mainSelection.SetSelectedOptionIndex(0);
	
	// Access active sub-selection
	iprm::ISelectionParam* activeSub = mainSelection.GetSubselection(0);
	if (activeSub)
	{
	    activeSub->SetSelectedOptionIndex(2);
	}
	\endcode
	
	\note Inherits from iser::ISerializable, so selections can be saved/loaded.
	\note Supports change notification via CF_SELECTION_CHANGED flag.
	
	\see IOptionsList, IOptionsManager, CSelectionParam
*/
class ISelectionParam: virtual public iser::ISerializable
{
public:
	/**
		\brief Data model change notification flags.
		
		Used with the observer pattern to detect when selection changes.
		
		\see istd::IChangeable::ChangeSet
	*/
	enum ChangeFlags
	{
		CF_SELECTION_CHANGED = 0x67364be  ///< Selection index has changed
	};

	enum 
	{
		NO_SELECTION = -1,  ///< Indicates no option is currently selected
	};

	/**
		\brief Get constraints of this parameter.
		
		Returns the options list that defines available choices for this selection.
		Constraints describe allowed parameter ranges and related information.
		
		\return Pointer to IOptionsList defining available options, owned by the
		        parameter or external manager. Do not delete.
		
		\code{.cpp}
		const iprm::IOptionsList* options = selection.GetSelectionConstraints();
		if (options)
		{
		    int count = options->GetOptionsCount();
		    for (int i = 0; i < count; ++i)
		    {
		        QString name = options->GetOptionName(i);
		        qDebug() << "Option" << i << ":" << name;
		    }
		}
		\endcode
		
		\see IOptionsList
	*/
	virtual const IOptionsList* GetSelectionConstraints() const = 0;

	/**
		\brief Get selected index.
		
		Returns the index of the currently selected option.
		
		\return Index of selected option (0-based), or NO_SELECTION (-1) if no option
		        is selected.
		
		\code{.cpp}
		int index = selection.GetSelectedOptionIndex();
		if (index == iprm::ISelectionParam::NO_SELECTION)
		{
		    qDebug() << "No selection";
		}
		else
		{
		    qDebug() << "Selected index:" << index;
		}
		\endcode
		
		\see SetSelectedOptionIndex, NO_SELECTION
	*/
	virtual int GetSelectedOptionIndex() const = 0;

	/**
		\brief Set index of selected option.
		
		Changes the selected option to the specified index.
		
		\param index Index of the option to select (0-based), or NO_SELECTION to
		             clear the selection.
		\return true if selection change was possible and successful, false otherwise.
		
		\note Returns false if index is out of range or the option is disabled.
		\note Triggers CF_SELECTION_CHANGED notification on success.
		
		\code{.cpp}
		// Select third option
		bool success = selection.SetSelectedOptionIndex(2);
		if (success)
		{
		    qDebug() << "Selection changed";
		}
		
		// Clear selection
		selection.SetSelectedOptionIndex(iprm::ISelectionParam::NO_SELECTION);
		\endcode
		
		\see GetSelectedOptionIndex, CF_SELECTION_CHANGED
	*/
	virtual bool SetSelectedOptionIndex(int index) = 0;

	/**
		\brief Get sub-selection for specified option.
		
		Retrieves the hierarchical sub-selection associated with a specific option.
		This enables multi-level selection structures (e.g., Category -> Subcategory).
		
		\param index Index of the option whose sub-selection to retrieve.
		\return Pointer to sub-selection parameter, or NULL if no sub-selection exists
		        for this option.
		
		\note The returned pointer belongs to the selection's data model. Do not delete.
		\note Sub-selections are independent selection parameters that can have their
		      own constraints and further sub-selections.
		
		\code{.cpp}
		// Get sub-selection for currently selected option
		int selectedIndex = selection.GetSelectedOptionIndex();
		if (selectedIndex >= 0)
		{
		    iprm::ISelectionParam* subSelection = selection.GetSubselection(selectedIndex);
		    if (subSelection)
		    {
		        // Work with sub-selection
		        subSelection->SetSelectedOptionIndex(0);
		    }
		}
		
		// Check all options for sub-selections
		const iprm::IOptionsList* options = selection.GetSelectionConstraints();
		int count = options->GetOptionsCount();
		for (int i = 0; i < count; ++i)
		{
		    iprm::ISelectionParam* sub = selection.GetSubselection(i);
		    if (sub)
		    {
		        qDebug() << "Option" << i << "has a sub-selection";
		    }
		}
		\endcode
	*/
	virtual ISelectionParam* GetSubselection(int index) const = 0;
};


} // namespace iprm




