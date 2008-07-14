#ifndef isig_ISamplingConstraints_included
#define isig_ISamplingConstraints_included


#include "istd/CRange.h"

#include "iser/ISerializable.h"

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
};


} // namespace isig


#endif // !isig_ISamplingConstraints_included


