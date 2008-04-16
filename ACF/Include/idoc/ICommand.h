#ifndef idoc_ICommand_included
#define idoc_ICommand_included


#include "istd/INamed.h"
#include "istd/IEnableable.h"
#include "istd/TIHierarchical.h"


namespace idoc
{


/**
	Provides single command for interaction with user.
*/
class ICommand:
			virtual public istd::INamed, 
			virtual public istd::IEnableable
{
public:
	enum StaticFlags
	{
		/**
			If this is enabled command will be permanent accessible, even if context is not active.
			For example if view command is permanent, this command will be also accessible if other view is active.
		*/
		CF_PERMANENT = 0x0001,
		/**
			Enable to use this command in global application menu.
		*/
		CF_GLOBAL_MENU = 0x0010,
		/**
			Enable to use this command in context menu.
		*/
		CF_CONTEXT_MENU = 0x0020,
		/**
			Enable to use this command in application toolbar.
		*/
		CF_TOOLBAR = 0x0040,
		/**
			Enable to use this command in separated toolbox.
		*/
		CF_TOOLBOX = 0x0080,
		CF_ALL_ENABLED = CF_GLOBAL_MENU | CF_CONTEXT_MENU | CF_TOOLBAR | CF_TOOLBOX
	};

	/**
		Get priority in the same group.
	*/
	virtual int GetPriority() const = 0;

	/**
		Get static flags of this command.
		This flags should not change its values during application run and they don't belong to data model.
	*/
	virtual int GetStaticFlags() const = 0;

	/**
		Execute this command.
		\param	contextPtr	implementation specific context parameter.
		\return	true if this command was consumed.
	*/
	virtual bool Execute(istd::IPolymorphic* contextPtr) = 0;
};


/**
	Hierarchical command structure used to create dynamic menu structures.
*/
typedef istd::TIHierarchical<ICommand> IHierarchicalCommand;


} // namespace idoc


#endif // !idoc_ICommand_included


