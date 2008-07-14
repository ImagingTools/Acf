#ifndef iipr_CIterativeProcessorComp_included
#define iipr_CIterativeProcessorComp_included


#include "iproc/TSyncProcessorWrap.h"

#include "icomp/CComponentBase.h"

#include "iimg/CGeneralBitmap.h"

#include "iipr/IBitmapProcessor.h"

#include "iipr/CIterativeProcessorParams.h"


namespace iipr
{
	

/**
	Processor wrapper for multiple executing of a slave processor.
	Input at k-iteration for the slave operator is processor output at (k-1)-iteration.
*/
class CIterativeProcessorComp:	public icomp::CComponentBase,
								public iproc::TSyncProcessorWrap<iipr::IBitmapProcessor>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iproc::TSyncProcessorWrap<iipr::IBitmapProcessor> BaseClass2;

	I_BEGIN_COMPONENT(CIterativeProcessorComp)
		I_REGISTER_INTERFACE(iipr::IBitmapProcessor)
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of processor parameter", true, "ParamsId");
		I_ASSIGN(m_slaveProcessorCompPtr, "SlaveProcessor", "Slave image processor", true, "SlaveProcessor");
	I_END_COMPONENT

protected:		
	// reimplemented (iproc::TSyncProcessorWrap<iipr::IBitmapProcessor>)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap* inputPtr,
				iimg::IBitmap* outputPtr);

private:
	int ProcessSlave(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap* inputPtr,
				iimg::IBitmap* outputPtr);
private:
	I_ATTR(istd::CString, m_paramsIdAttrPtr);
	I_REF(iipr::IBitmapProcessor, m_slaveProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CIterativeProcessorComp_included

