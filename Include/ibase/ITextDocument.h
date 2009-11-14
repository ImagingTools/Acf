#ifndef ibase_ITextDocument_included
#define ibase_ITextDocument_included


#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace ibase
{


/**
	Simple interface for a text document.
*/
class ITextDocument: virtual public iser::ISerializable
{
public:
	/**
		Get document text.
	*/
	virtual istd::CString GetText() const = 0;

	/**
		Set document text.
	*/
	virtual void SetText(const istd::CString& text) = 0;
};


} // namespace ibase


#endif // !ibase_ITextDocument_included


