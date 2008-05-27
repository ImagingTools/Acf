#ifndef iproc_IElementSelector_included
#define iproc_IElementSelector_included


#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace iproc
{


/**
	Interface representing selector of element from element list.
*/
class IElementSelector: public iser::ISerializable
{
public:
	/**
		Get number of selectable elements.
	*/
	virtual int GetElementsCount() const = 0;

	/**
		Get name of element.
	*/
	virtual istd::CString GetElementName(int elementIndex) const = 0;

	/**
		Get index of selected element.
	*/
	virtual int GetSelectedIndex() const = 0;

	/**
		Set index of selected element.
		\return		true if it was possible to select this element.
	*/
	virtual bool SetSelectedIndex(int index) = 0;
};


} // namespace iproc


#endif // !iproc_IElementSelector_included


