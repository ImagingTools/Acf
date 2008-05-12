#ifndef iproc_IJob_included
#define iproc_IJob_included


#include <string>


#include "istd/TIFactory.h"

#include "istd/INamed.h"
#include "istd/IChangeable.h"

#include "iproc/IOperator.h"


namespace iproc
{


class IJob:	virtual public iproc::IOperator
{
public:
	typedef istd::TIFactory<iproc::IOperator> IOperatorFactory;

	virtual void RegisterElementFactory(IOperatorFactory* elementFactoryPtr) = 0;

	virtual iproc::IOperator* AddProcessingElement(const std::string elementFactoryKey, const std::string& elementId) = 0;
};


} // namespace iproc


#endif // !iproc_IJob_included


