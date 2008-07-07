#ifndef iipr_CLocalDifferenceProcessorComp_included
#define iipr_CLocalDifferenceProcessorComp_included


#include "iipr/TImageProcessorCompBase.h"
#include "iipr/CLocalDifferenceFilterParams.h"


namespace iipr
{
	

/**
	Implementation of moving average operator.
*/
class CLocalDifferenceProcessorComp: public iipr::TImageProcessorCompBase<iipr::CLocalDifferenceFilterParams>
{
public:
	typedef iipr::TImageProcessorCompBase<iipr::CLocalDifferenceFilterParams> BaseClass;

	I_BEGIN_COMPONENT(CLocalDifferenceProcessorComp)
		I_ASSIGN(m_alphaAttrPtr, "Alpha", "Alpha", true, 3);
		I_ASSIGN(m_betaAttrPtr, "Beta", "Beta", true, 0.5);
		I_ASSIGN(m_gammaAttrPtr, "Gamma", "Gamma", true, 128);
	I_END_COMPONENT


protected:		
	// reimplemented (iipr::TImageProcessorCompBase<iipr::CLocalDifferenceFilterParams>)
	virtual bool ProcessImage(
				const iipr::CLocalDifferenceFilterParams* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

private:
	I_ATTR(int, m_alphaAttrPtr);
	I_ATTR(double, m_betaAttrPtr);
	I_ATTR(int, m_gammaAttrPtr);
};


} // namespace iipr


#endif // !iipr_CLocalDifferenceProcessorComp_included

