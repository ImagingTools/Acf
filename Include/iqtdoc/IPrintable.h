#ifndef iqtdoc_IPrintable_included
#define iqtdoc_IPrintable_included


// Qt includes
#include <QPrinter>


#include "istd/IPolymorphic.h"

#include "iqtdoc/iqtdoc.h"


namespace iqtdoc
{


/**
	Common interface for printable objects.
*/
class IPrintable: public istd::IPolymorphic
{
public:
	/**
		Print some contents to the printer \c printer.
		Printer object is already initialized from framework.
	*/
	virtual void Print(QPrinter* printerPtr) const = 0;
};


} // namespace iqtdoc


#endif // !iqtdoc_IPrintable_included


