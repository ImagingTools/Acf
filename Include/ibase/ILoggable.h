#ifndef ibase_ILoggable_included
#define ibase_ILoggable_included


#include "ibase/IMessageConsumer.h"

#include "istd/IPolymorphic.h"


namespace ibase
{


/**
	\ingroup ACF

	Inteface for object, which support the 
*/
class ILoggable: virtual public istd::IPolymorphic
{
public:
	/**
		Install log.
	*/
	virtual void SetLogPtr(ibase::IMessageConsumer* logPtr);
	
	/**
		Get log.
	*/
	virtual ibase::IMessageConsumer* GetLogPtr() const;
};


} // namespace ibase


#endif // !ibase_ILoggable_included


