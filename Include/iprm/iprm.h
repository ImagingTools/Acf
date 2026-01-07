#ifndef iprm_included
#define iprm_included


#include <istd/istd.h>
#include <imod/imod.h>
#include <iser/iser.h>
#include <icomp/icomp.h>


/**
	\namespace iprm
	\brief Contains interfaces and implementations of flexible parameter concept.

	The iprm namespace provides a comprehensive parameter management framework that enables
	applications to create, manage, and manipulate structured parameter collections with
	support for various parameter types, validation, serialization, and change notification.

	\section iprm_overview Overview

	The iprm library is designed to handle complex parameter management scenarios in applications
	that require:
	- Multiple parameter types (text, numeric, selection, etc.)
	- Dynamic parameter sets that can be created, modified, and deleted at runtime
	- Parameter validation and consistency checking
	- Serialization and deserialization of parameter data
	- Change notification for parameter modifications
	- Hierarchical parameter organization

	\section iprm_core_concepts Core Concepts

	\subsection iprm_parameters Parameters

	Individual parameter types in the iprm library:
	- **ITextParam**: Stores simple text strings
	- **IIdParam**: Stores unique identifiers (QByteArray)
	- **INameParam**: Stores names with fixed/editable state
	- **ISelectionParam**: Allows selection from a list of options
	- **IEnableableParam**: Can be enabled or disabled
	- **IVariableParam**: Can change its underlying type dynamically

	All parameters implement iser::ISerializable for persistence.

	\subsection iprm_paramsets Parameter Sets

	**IParamsSet** is a container that groups related parameters together. Each parameter
	in a set is identified by a unique QByteArray ID. Parameter sets distinguish between:
	- Read-only parameters (for display or calculated values)
	- Editable parameters (that users can modify)

	\subsection iprm_paramsmanager Parameter Manager

	**IParamsManager** manages multiple parameter sets and provides:
	- CRUD operations (Create, Read, Update, Delete) for parameter sets
	- Naming and describing parameter sets
	- Operation flags to control what operations are allowed
	- Type constraints for creating new parameter sets
	- Selection of active parameter set

	\subsection iprm_options Options and Constraints

	Selection parameters are constrained by option lists:
	- **IOptionsList**: Read-only list of available options with names, IDs, and descriptions
	- **IOptionsManager**: Extends IOptionsList with dynamic add/remove/modify capabilities

	Options can be:
	- Enabled or disabled (affecting selectability)
	- Given unique IDs for stable references
	- Organized hierarchically with sub-selections

	\section iprm_usage_patterns Usage Patterns

	\subsection iprm_pattern_simple Simple Parameter Usage

	\code{.cpp}
	// Create a text parameter
	iprm::CTextParam param;
	param.SetText("Hello");
	QString value = param.GetText();
	\endcode

	\subsection iprm_pattern_selection Selection Parameter Usage

	\code{.cpp}
	// Create options and selection
	iprm::COptionsManager options;
	options.InsertOption("Option A", "opt_a", "First option");
	options.InsertOption("Option B", "opt_b", "Second option");

	iprm::CSelectionParam selection;
	selection.SetSelectionConstraints(&options);
	selection.SetSelectedOptionIndex(0);
	\endcode

	\subsection iprm_pattern_manager Parameter Manager Usage

	\code{.cpp}
	// Work with parameter manager
	iprm::IParamsManager* manager = /* obtained from component */;
	
	// Check capabilities
	int flags = manager->GetIndexOperationFlags(-1);
	if (flags & iprm::IParamsManager::MF_SUPPORT_INSERT)
	{
	    // Insert new parameter set
	    int index = manager->InsertParamsSet();
	    manager->SetParamsSetName(index, "My Parameters");
	}
	\endcode

	\section iprm_serialization Serialization

	All parameter types support serialization through the iser framework:

	\code{.cpp}
	iser::CMemoryWriteArchive writeArchive(nullptr);
	param.Serialize(writeArchive);  // Save

	iser::CMemoryReadArchive readArchive(writeArchive);
	param.Serialize(readArchive);   // Load
	\endcode

	\section iprm_change_notification Change Notification

	Parameters support the observer pattern through the istd::IChangeable interface.
	Each parameter type defines ChangeFlags to identify specific changes:

	\code{.cpp}
	// ISelectionParam::CF_SELECTION_CHANGED
	// INameParam::CF_RENAME
	// IEnableableParam::CF_ENABLED
	// IParamsManager::CF_SET_INSERTED, CF_SET_REMOVED, etc.
	\endcode

	\section iprm_validation Validation

	Parameter sets can be validated using IParamsSetValidator:

	\code{.cpp}
	iprm::IParamsSetValidator* validator = /* ... */;
	bool isValid = validator->IsParamsSetConsistent(
	    "contextId",
	    paramSet,
	    &messageConsumer);
	\endcode

	\section iprm_best_practices Best Practices

	-# Always check operation flags before attempting operations
	-# Use smart pointers (TUniqueInterfacePtr, TSharedInterfacePtr) for memory management
	-# Validate parameter sets before persisting or processing
	-# Use option IDs rather than indices for stable selection references
	-# Implement observers for reactive parameter updates
	-# Clone parameters when you need independent copies

	\section iprm_classes Main Classes

	**Interfaces:**
	- iprm::IParamsSet - Parameter set container
	- iprm::IParamsManager - Multiple parameter set manager
	- iprm::ITextParam - Text parameter
	- iprm::IIdParam - ID parameter
	- iprm::INameParam - Name parameter
	- iprm::ISelectionParam - Selection parameter
	- iprm::IEnableableParam - Enableable parameter
	- iprm::IVariableParam - Variable type parameter
	- iprm::IOptionsList - Options list
	- iprm::IOptionsManager - Dynamic options manager
	- iprm::IParameterStateProvider - Parameter state provider
	- iprm::IParamsSetValidator - Parameter set validator

	**Implementations:**
	- iprm::CTextParam - Text parameter implementation
	- iprm::CIdParam - ID parameter implementation
	- iprm::CNameParam - Name parameter implementation
	- iprm::CSelectionParam - Selection parameter implementation
	- iprm::CEnableableParam - Enableable parameter implementation
	- iprm::COptionsManager - Options manager implementation
	- iprm::CParamsSet - Parameter set implementation
	- iprm::CParamsManagerComp - Component-based parameter manager

	\ingroup ParameterManagement
*/
namespace iprm
{


} // namespace iprm


#endif // !iprm_included

