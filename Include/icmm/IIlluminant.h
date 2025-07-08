#pragma once


// ACF includes
#include <icmm/IWhitePointProvider.h>


namespace icmm
{


class IIlluminant: virtual public IWhitePointProvider
{
public:
	virtual void SetWhitePoint(const icmm::CVarColor& whitePoint) = 0;
	virtual QString GetIlluminantName() const = 0;
	virtual void SetIlluminantName(const QString& illuminantName) = 0;
	virtual StandardIlluminant GetIlluminantType() const = 0;
	virtual void SetIlluminantType(const StandardIlluminant& illuminantType) = 0;
};


} // namespace icmm


