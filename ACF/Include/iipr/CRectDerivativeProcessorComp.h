#ifndef iipr_CRectDerivativeProcessorComp_included
#define iipr_CRectDerivativeProcessorComp_included


#include "istd/CString.h"

#include "icomp/CComponentBase.h"

#include "iipr/CRectDerivativeProcessor.h"


namespace iipr
{


/**
	Component implemnetation of derivative processor using rectangular filter kernel.
*/
class CRectDerivativeProcessorComp: public icomp::CComponentBase, public CRectDerivativeProcessor
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CRectDerivativeProcessor BaseClass2;

	I_BEGIN_COMPONENT(CRectDerivativeProcessorComp);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_ASSIGN(m_filterParamsIdAttrPtr, "FilterParamsId", "ID of filter parameters in parameter set", true, "FilterParams");
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_ATTR(istd::CString, m_filterParamsIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CRectDerivativeProcessorComp_included


