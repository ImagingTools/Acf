#ifndef iipr_TKernel2d_included
#define iipr_TKernel2d_included


#include "iimg/TBitmapIterator2d.h"


namespace iipr
{

/**
	Implementation of a general 2D convolution kernel. 
	Internally the kernel is represented by an array of filter weights and offset positions
	for fast access to the corresponding bitmap values.

	For access and iterating through the kernel values you can use the Begin() and End() function, 
	which returns an iterator/accessor.

	\sa TLocalNeighborhood
*/
template <typename PixelType, typename KernelType> 
class TKernel2d
{
public:
	typedef iimg::TBitmapIterator2d<PixelType> BitmapIterator;
	typedef std::pair<int /*pixel offset*/, KernelType /*kernel weight*/> KernelValue;
	typedef std::vector<KernelValue> KernelValues;
	typedef typename KernelValues::const_iterator Iterator;

	/**
		Create kernel offsets with respect to image size.
	*/
	void InitForBitmap(const iimg::IBitmap& bitmap) const;

	Iterator Begin() const;
	Iterator End() const;

	double GetWeightsSum() const;
	i2d::CRectangle GetBoundingBox() const;

protected:
	KernelValues m_values;
	ibase::CSize m_kernelSize;

	double m_sumWeights;
};


template <typename PixelType, typename KernelType> 
void TKernel2d<PixelType, KernelType>::InitForBitmap(const iimg::IBitmap& bitmap) const
{
	I_ASSERT(!m_kernelSize.IsNull());
	if (m_kernelSize.IsNull()){
		return;
	}

	int linesDifference = bitmap.GetLinesDifference();

	int halfWidth = m_kernelSize.GetX() / 2;
	int halfHeight = m_kernelSize.GetY() / 2;

	I_ASSERT((m_kernelSize.GetX() * m_kernelSize.GetY()) == int(m_values.size()));

	KernelValues::const_iterator elementIter = m_values.begin(); 

	for (int y = -halfHeight; y <= halfHeight; y++){
		for (int x = -halfWidth; x <= halfWidth; x++){
			KernelValue& kernelValue = const_cast<KernelValue&>(*elementIter);

			kernelValue.first = x + y * linesDifference;

			++elementIter;
		}
	}
}


template <typename PixelType, typename KernelType> 
inline typename TKernel2d<PixelType, KernelType>::Iterator TKernel2d<PixelType, KernelType>::Begin() const
{
	return m_values.begin();
}


template <typename PixelType, typename KernelType> 
inline typename TKernel2d<PixelType, KernelType>::Iterator TKernel2d<PixelType, KernelType>::End() const
{
	return m_values.end();
}


template <typename PixelType, typename KernelType> 
inline double TKernel2d<PixelType, KernelType>::GetWeightsSum() const
{
	return m_sumWeights;
}


template <typename PixelType, typename KernelType> 
inline i2d::CRectangle TKernel2d<PixelType, KernelType>::GetBoundingBox() const
{
	return i2d::CRectangle(m_kernelSize);
}


} // namespace iipr


#endif // !iipr_TKernel2d_included

