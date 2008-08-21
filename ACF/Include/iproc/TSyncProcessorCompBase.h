#ifndef iproc_TSyncProcessorCompBase_included
#define iproc_TSyncProcessorCompBase_included


#include "ibase/TMessageProducerWrap.h"

#include "iproc/TSyncProcessorWrap.h"

#include "icomp/CComponentBase.h"

#include "iwin/CTimer.h"


namespace iproc
{


/**
	Simple definition macro for a syncronious processor.
*/
template <class Base>
class TSyncProcessorCompBase:
			public ibase::TMessageProducerWrap<icomp::CComponentBase>,
			public iproc::TSyncProcessorWrap<Base>
{
public:
	typedef ibase::TMessageProducerWrap<icomp::CComponentBase> BaseClass;
	typedef iproc::TSyncProcessorWrap<Base> BaseClass2;

	I_BEGIN_BASE_COMPONENT(TSyncProcessorCompBase)
		I_REGISTER_INTERFACE(Base)
		I_REGISTER_INTERFACE(iproc::IProcessor)
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of processor parameter", true, "ParamsId");
		I_ASSIGN(m_preprocessorCompPtr, "Preprocessor", "Pre-processing operator", false, "Preprocessor");
	I_END_COMPONENT

protected:
	I_ATTR(istd::CString, m_paramsIdAttrPtr);
	I_TREF(Base, m_preprocessorCompPtr);
};


// global typedefs

typedef TSyncProcessorCompBase<iproc::IProcessor> CSyncProcessorCompBase;


} // namespace iproc


#endif // !iproc_TSyncProcessorCompBase_included

