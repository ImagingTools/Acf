#ifndef iview_included
#define iview_included


/**
	In this library is defined 2D view concept and standard visualisation objects.
*/
namespace iview
{


/**
	Define possible display changes.
	Use binary or operator, to combine this flags.
*/
enum ChangeFlags
{
	/**
		All was changed.
	*/
	CF_ALL = -1,
	
	/**	
		No changes.
	*/
	CF_NONE = 0,
	
	/**
		View transform is changed.
	*/
	CF_TRANSFORM = 1,
	
	/**
		View colors are changed.
	*/
	CF_COLORS = 2,
	
	/**
		View size is changed.
	*/
	CF_SIZE = 4,
	
	/**
		Edit mode is changed.
	*/
	CF_EDIT_MODE = 8
};


enum ChangeSource
{
	/**	
		Indicate, that changes come from display console shapes.
	*/
	CS_CONSOLE = 32
};


} // namespace iview



#endif // !iview_included


