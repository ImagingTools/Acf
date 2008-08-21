#ifndef isig_ISamplingConstraints_included
#define isig_ISamplingConstraints_included


#include "istd/CRange.h"

#include "iser/ISerializable.h"

#include "iprm/IParamsSet.h"

#include "isig/isig.h"


namespace isig
{


/**
	Define constrains of sampling parameters.
*/
class ISamplingConstraints: virtual public istd::IPolymorphic
{
public:
	/**
		Return range of possible sampling interval values.
	*/
	virtual istd::CRange GetIntervalRange() const = 0;

	/**
		Check if specified sampling mode is supported.
		\sa	isig::ISamplingParams::SamplingMode
	*/
	virtual bool IsSamplingModeSupported(int mode) const = 0;

	/**
		Get range of possible/representable sample values.
	*/
	virtual istd::CRange GetValueRange(bool forInput = true, bool forOutput = true, const iprm::IParamsSet* paramsSetPtr = NULL) const = 0;

	/**
		Get maximal number of samples can be received/send at one time.
		\return	number of samples can be processed at one time in single sample container, or -1 if there is no restrictions.
	*/
	virtual int GetMaximalSamplesCount(bool forInput = true, bool forOutput = true, const iprm::IParamsSet* paramsSetPtr = NULL) const = 0;
};


} // namespace isig


#endif // !isig_ISamplingConstraints_included


