#ifndef IMeilhausAccess_included
#define IMeilhausAccess_included


#include "istd/IPolymorphic.h"
#include "istd/CRange.h"

#include "isig/ISamplingConstraints.h"


namespace imebase
{


class CChannelSelectionNode;


class IMeilhausAccess: public isig::ISamplingConstraints
{
public:
	virtual bool CreateSelectionTree(CChannelSelectionNode& result) const = 0;
};


} // namespace imebase


#endif // !IMeilhausAccess_included


