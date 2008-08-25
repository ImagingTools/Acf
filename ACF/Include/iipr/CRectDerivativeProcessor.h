#ifndef iipr_CRectDerivativeProcessor_included
#define iipr_CRectDerivativeProcessor_included


#include "iproc/TSyncProcessorWrap.h"

#include "iipr/CProjectionData.h"


namespace iipr
{


/**
	Calculate derivative of projection using rectangular filter kernel.
*/
class CRectDerivativeProcessor: public iproc::CSyncProcessorBase
{
public:
	/**
		Do extremum features analyze.
	*/
	virtual bool DoDerivativeProcessing(const CProjectionData& source, double filterLength, CProjectionData& results);

	/**
		Get parameter ID used to extract caliper parameter object from parameter set.
	*/
	const std::string& GetFilterParamsId() const;

	/**
		Set parameter ID used to extract caliper parameter object from parameter set.
		It is only needed while using general processing interface iproc::IProcessor.
	*/
	void SetFilterParamsId(const std::string& id);

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

protected:
	bool DoBitmapProcessing(const iimg::IBitmap& sourceBitmap, double filterLength, iimg::IBitmap& resultBitmap);

private:
	std::string m_filterParamsId;
};


// inline methods

inline const std::string& CRectDerivativeProcessor::GetFilterParamsId() const
{
	return m_filterParamsId;
}


inline void CRectDerivativeProcessor::SetFilterParamsId(const std::string& id)
{
	m_filterParamsId = id;
}


} // namespace iipr


#endif // !iipr_CRectDerivativeProcessor_included


