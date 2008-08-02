#ifndef iipr_TGaussianKernel_included
#define iipr_TGaussianKernel_included


#include "iipr/TKernel2d.h"


namespace iipr
{


template <typename PixelType, typename KernelType> 
class TGaussianKernel2d: public iipr::TKernel2d<PixelType, KernelType>
{
public:
	typedef iipr::TKernel2d<PixelType, KernelType> BaseClass;

	TGaussianKernel2d();
	TGaussianKernel2d(double sigma1, double sigma2, double angle, const iimg::IBitmap* bitmapPtr = NULL);

protected:
	void CreateKernel(double sigma1, double sigma2, double angle);
};


template <typename PixelType, typename KernelType> 
TGaussianKernel2d<PixelType, KernelType>::TGaussianKernel2d()
{
	I_CRITICAL();
}


template <typename PixelType, typename KernelType> 
TGaussianKernel2d<PixelType, KernelType>::TGaussianKernel2d(double sigma1, 
															double sigma2, 
															double angle, 
															const iimg::IBitmap* bitmapPtr)
{
	CreateKernel(sigma1, sigma2, angle);

	if (bitmapPtr != NULL){
		InitForBitmap(*bitmapPtr);
	}
}


// protected methods

template <typename PixelType, typename KernelType> 
void TGaussianKernel2d<PixelType, KernelType>::CreateKernel(double /*sigma1*/, double /*sigma2*/, double /*angle*/)
{
//	I_ASSERT(sigma1 != 0);
//	I_ASSERT(sigma2 != 0);

	m_values.resize(25, std::make_pair(0, KernelType(1)));
	m_kernelSize = ibase::CSize(5, 5);
	m_sumWeights = 25;
}


typedef TGaussianKernel2d<I_BYTE, float> CGaussianKernel2d;


} // namespace iipr


#endif // !iipr_TGaussianKernel_included

