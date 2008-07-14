#ifndef isig_ISamplingParams_included
#define isig_ISamplingParams_included


#include "istd/CRange.h"

#include "iser/ISerializable.h"

#include "isig/isig.h"


namespace isig
{


class ISamplingConstraints;


/**
	Sampler parameters.
*/
class ISamplingParams: virtual public iser::ISerializable
{
public:
	enum SamplingMode
	{
		SM_SINGLE,
		SM_PERIODIC,
		SM_SYNCHRONIZED,
		SM_LAST = SM_SYNCHRONIZED
	};

	/**
		Get access to constraints information about possible value ranges.
	*/
	virtual const ISamplingConstraints* GetConstraints() const = 0;

	/**
		Get sampling interval in seconds.
	*/
	virtual double GetInterval() const = 0;
	/**
		Set sampling interval in seconds.
	*/
	virtual void SetInterval(double value) = 0;

	/**
		Get mode of sampling.
		\sa	SamplingMode.
	*/
	virtual int GetSamplingMode() const = 0;

	/**
		Set mode of sampling.
		\sa	SamplingMode.
		\return	true if this mode was supported and was set correctly.
	*/
	virtual bool SetSamplingMode(int mode) = 0;
};


} // namespace isig


#endif // !isig_ISamplingParams_included


