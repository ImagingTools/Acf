#ifndef iipr_CLocalDifferenceFilterParams_included
#define iipr_CLocalDifferenceFilterParams_included


#include "iser/ISerializable.h"

#include "ibase/CSize.h"


namespace iipr
{


/**
	Basic implementation for the local operators with a rectangular kernel
*/
class CLocalDifferenceFilterParams: virtual public iser::ISerializable
{
public:
	CLocalDifferenceFilterParams();

	virtual void SetAlpha(int alpha);
	virtual int GetAlpha() const;

	virtual void SetBeta(double beta);
	virtual double GetBeta() const;

	virtual void SetGamma(int gamma);
	virtual int GetGamma() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	int m_alpha;
	double m_beta;
	int m_gamma;
};


} // namespace iipr


#endif // !iipr_CLocalDifferenceFilterParams_included

