#ifndef iipr_CRectangularFilterCompBase_included
#define iipr_CRectangularFilterCompBase_included


#include "iproc/TSyncProcessorWrap.h"

#include "icomp/CComponentBase.h"

#include "iipr/IBitmapProcessor.h"
#include "iipr/CImageManipulation.h"
#include "iipr/CRectangularFilterParams.h"


namespace iipr
{


/**
	Basic implementation for the local operators with a rectangular kernel
*/
class CRectangularFilterCompBase:	public icomp::CComponentBase,
									public iproc::TSyncProcessorWrap<iipr::IBitmapProcessor>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iproc::TSyncProcessorWrap<iipr::IBitmapProcessor> BaseClass2;

	I_BEGIN_BASE_COMPONENT(CRectangularFilterCompBase)
		I_REGISTER_INTERFACE(iipr::IBitmapProcessor)
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of filter parameter", true, "FilterParams");
	I_END_COMPONENT

protected:
	// reimplemented (iproc::TSyncProcessorWrap<iipr::IBitmapProcessor>)
	virtual int DoSyncProcess(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap* inputPtr,
				iimg::IBitmap* outputPtr);

	// abstract methods
	virtual void DoBufferFilter(
				const CRectangularFilterParams& params,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) = 0;

protected:
	I_ATTR(istd::CString, m_paramsIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CRectangularFilterCompBase_included

