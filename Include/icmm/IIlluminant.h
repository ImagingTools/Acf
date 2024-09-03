#pragma once


// ACF includes
#include <icmm/IWhitePointProvider.h>


namespace icmm
{


class IIlluminant: virtual public IWhitePointProvider
{
public:
	virtual void SetWhitePoint(const icmm::CVarColor& whitePoint) = 0;
	virtual QString GetIllumninantName() const = 0;
	virtual void SetIllumninantName(const QString& illuminantName) = 0;
	virtual StandardIlluminant GetIllumninantType() const = 0;
	virtual void SetIllumninantType(const StandardIlluminant& illuminantType) = 0;
};


} // namespace icmm


