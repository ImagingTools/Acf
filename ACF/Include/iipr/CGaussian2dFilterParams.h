#ifndef iipr_CGaussian2dFilterParams_included
#define iipr_CGaussian2dFilterParams_included


#include "iser/ISerializable.h"


namespace iipr
{


/**
	Basic implementation for the local operators with a rectangular kernel
*/
class CGaussian2dFilterParams: virtual public iser::ISerializable
{
public:
	CGaussian2dFilterParams();

	virtual double GetSigma1() const;
	virtual void SetSigma1(double sigma1);
	virtual double GetSigma2() const;
	virtual void SetSigma2(double sigma2);
	virtual double GetAngle() const;
	virtual void SetAngle(double sigma1);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_sigma1;
	double m_sigma2;
	double m_angle;
};


} // namespace iipr


#endif // !iipr_CGaussian2dFilterParams_included

