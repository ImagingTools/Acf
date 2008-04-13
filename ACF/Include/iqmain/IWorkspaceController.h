#ifndef iqmain_IWorkspaceController_included
#define iqmain_IWorkspaceController_included


#include "istd/IPolymorphic.h"


namespace iqmain
{


/**	
	Interface for controlling of views in a multiple document workspace.
*/
class IWorkspaceController: public istd::IPolymorphic
{
public:
	/**
		Do vertical tiling of all views in the workspace area.
	*/
	virtual void TileHorizontally() = 0;

	/**
		Do tiling of all views in the workspace area.
	*/
	virtual void Tile() = 0;

	/**
		Cascade all views in the workspace area.
	*/
	virtual void Cascade() = 0;

	/**
		Close all views in the workspace area.
	*/
	virtual void CloseAllViews() = 0;
};


} // namespace iqmain


#endif // !iqmain_IWorkspaceController_included

