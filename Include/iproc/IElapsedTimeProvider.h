#ifndef iproc_IElapsedTimeProvider_included
#define iproc_IElapsedTimeProvider_included


#include "istd/IChangeable.h"


namespace iproc
{


/**
	Provider of the elapsed time
	E.g. used for processing time measuring.
*/
class IElapsedTimeProvider: virtual public istd::IChangeable
{
public:
	/**
		Get the elapsed time.
	*/
	virtual double GetElapsedTime() const = 0;
};


} // namespace iproc


#endif // !iproc_IElapsedTimeProvider_included


