#ifndef iipr_CMultidimensionalFilterParamsComp_included
#define iipr_CMultidimensionalFilterParamsComp_included


#include "iser/ISerializable.h"

#include "ibase/CSize.h"

#include "icomp/CComponentBase.h"

#include "iipr/IMultidimensionalFilterParams.h"
#include "iipr/IMultidimensionalFilterConstraints.h"


namespace iipr
{


/**
	Component implementating iipr::IMultidimensionalFilterParams interface.
	All parameter constraints can be defined directly or can use external implementation.
*/
class CMultidimensionalFilterParamsComp:
			public icomp::CComponentBase,
			virtual public IMultidimensionalFilterParams,
			virtual protected IMultidimensionalFilterConstraints
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMultidimensionalFilterParamsComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IMultidimensionalFilterParams);
		I_ASSIGN(m_dimensionsCountAttrPtr, "DimensionsCount", "Number of filter dimensions (will be used if no constraints set)", true, 2);
		I_ASSIGN(m_minFilterLengthAttrPtr, "MinFilterLength", "Minimal length of filter for all dimensions (will be used if no constraints set)", true, 1);
		I_ASSIGN(m_maxFilterLengthAttrPtr, "MaxFilterLength", "Maximal length of filter for all dimensions (will be used if no constraints set)", true, 10);
		I_ASSIGN_MULTI_0(m_filterLengthsAttrPtr, "FilterLengths", "Default values of filter lengths", false);
		I_ASSIGN(m_constraintsCompPtr, "Constraints", "Constraints object describing possible parameter values", false, "FilterConstraints");
	I_END_COMPONENT;

	CMultidimensionalFilterParamsComp();

	// reimplemented (iipr::IMultidimensionalFilterParams)
	virtual const IMultidimensionalFilterConstraints* GetConstraints() const;
	virtual imath::CVarVector GetFilterLengths() const;
	virtual bool SetFilterLengths(const imath::CVarVector& lengths);

	// reimplemented (iipr::IMultidimensionalFilterConstraints)
	virtual int GetFilterDimensionsCount() const;
	virtual istd::CRange GetFilterLengthRange(int dimension) const;
	virtual const imath::IDoubleManip& GetFilterLengthManip(int dimension) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	imath::CVarVector m_filterLengts;

	I_ATTR(int, m_dimensionsCountAttrPtr);
	I_ATTR(double, m_minFilterLengthAttrPtr);
	I_ATTR(double, m_maxFilterLengthAttrPtr);
	I_MULTIATTR(double, m_filterLengthsAttrPtr);
	I_REF(IMultidimensionalFilterConstraints, m_constraintsCompPtr);
};


} // namespace iipr


#endif // !iipr_CMultidimensionalFilterParamsComp_included

