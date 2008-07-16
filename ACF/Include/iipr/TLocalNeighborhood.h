#ifndef iipr_TLocalNeighborhood_included
#define iipr_TLocalNeighborhood_included


namespace iipr
{


/**
	Implementation of local neighborhood in a bitmap.
	Instance of this object is initialized with the some position in a bitmap (represented as a bitmap iterator) 
	and some kernel.
	Begin() and End() functions returns the boundary iterator for moving through the local neighborhood 
	and accessing to the pixel values.
*/
template <class BitmapIterator, class Kernel> 
class TLocalNeighborhood
{
public:
	typedef typename BitmapIterator::ValueType PixelValue;
	friend class Iterator;

	class Iterator
	{
	public:
		typedef typename TLocalNeighborhood::PixelValue PixelValue;
		typedef typename Kernel::Iterator KernelIterator;

		Iterator(const PixelValue* imageBufferPtr, const KernelIterator& kernelIterator);

		const PixelValue& operator*() const;
		Iterator& operator++();
		Iterator operator++(int);

	private:
		const PixelValue* m_imageBufferPtr;
		KernelIterator m_kernelIterator;
	};

	TLocalNeighborhood(const BitmapIterator& bitmapIterator, const Kernel& kernel);

	Iterator Begin() const;
	Iterator End() const;

private:
	BitmapIterator m_bitmapIterator;
	const Kernel& m_kernel;
};


template <class BitmapIterator, class Kernel> 
TLocalNeighborhood<BitmapIterator, Kernel>::TLocalNeighborhood(const BitmapIterator& bitmapIterator, const Kernel& kernel)
:	m_bitmapIterator(bitmapIterator),
	m_kernel(kernel)
{
}


template <class BitmapIterator, class Kernel> 
typename TLocalNeighborhood<BitmapIterator, Kernel>::Iterator TLocalNeighborhood<BitmapIterator, Kernel>::Begin() const
{
	return Iterator(&(*m_bitmapIterator), m_kernel.Begin());
}


template <class BitmapIterator, class Kernel> 
typename TLocalNeighborhood<BitmapIterator, Kernel>::Iterator TLocalNeighborhood<BitmapIterator, Kernel>::End() const
{
	return Iterator(&(*m_bitmapIterator), m_kernel.End());
}


// public methods of embedded class Iterator

template <class BitmapIterator, class Kernel> 
TLocalNeighborhood<BitmapIterator, Kernel>::Iterator::Iterator(const PixelValue* imageBufferPtr, const KernelIterator& kernelIterator)
:	m_kernelIterator(kernelIterator),
	m_imageBufferPtr(imageBufferPtr)
{
}


template <class BitmapIterator, class Kernel> 
inline const typename TLocalNeighborhood<BitmapIterator, Kernel>::Iterator::PixelValue& TLocalNeighborhood<BitmapIterator, Kernel>::Iterator::operator*() const
{
	int offset = (*m_kernelIterator).first;

	return *(m_imageBufferPtr + offset);
}


template <class BitmapIterator, class Kernel> 
inline typename TLocalNeighborhood<BitmapIterator, Kernel>::Iterator& TLocalNeighborhood<BitmapIterator, Kernel>::Iterator::operator++()
{
	++m_kernelIterator;

	return *this;
}


template <class BitmapIterator, class Kernel> 
inline typename TLocalNeighborhood<BitmapIterator, Kernel>::Iterator TLocalNeighborhood<BitmapIterator, Kernel>::Iterator::operator++(int)
{
	Iterator temp(*this);

	++m_kernelIterator;

	return temp;
}


} // namespace iipr


#endif // !iipr_TLocalNeighborhood_included

