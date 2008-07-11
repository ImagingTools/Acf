#ifndef iimg_TBitmapIterator_included
#define iimg_TBitmapIterator_included


#include "iimg/IBitmap.h"


namespace iimg
{


/**
	Help class for iteration through bitmap pixels. 
*/
template <typename PixelType>
class TBitmapIterator
{
public:
	typedef PixelType ValueType;

	/**
		Implementation of an sub-iterator in x-direction.
	*/
	class XIterator
	{
	public:
		XIterator(TBitmapIterator& iterator);
		TBitmapIterator operator++(int);
		TBitmapIterator& operator++();
		TBitmapIterator operator--(int);
		TBitmapIterator& operator--();
		TBitmapIterator& operator+=(int difference);
		TBitmapIterator& operator-=(int difference);
		TBitmapIterator operator+(int difference);
		TBitmapIterator operator-(int difference);
	private:
		TBitmapIterator& m_iterator;
	};

	/**
		Implementation of an sub-iterator in y-direction.
	*/
	class YIterator
	{
	public:
		YIterator(TBitmapIterator& iterator);
		TBitmapIterator operator++(int);
		TBitmapIterator& operator++();
		TBitmapIterator operator--(int);
		TBitmapIterator& operator--();
		TBitmapIterator& operator+=(int difference);
		TBitmapIterator& operator-=(int difference);
		TBitmapIterator operator+(int difference);
		TBitmapIterator operator-(int difference);
	private:
		TBitmapIterator& m_iterator;
	};

	/**
		Construct image iterator from a bitmap.
	*/
	TBitmapIterator(const iimg::IBitmap* bitmapPtr);

	/**
		Construct image iterator from another interator.
	*/
	TBitmapIterator(const TBitmapIterator& iterator);

	/**
		Access operator
	*/
	ValueType& operator*() const;

	bool operator < (const TBitmapIterator& iterator);
	bool operator > (const TBitmapIterator& iterator);
	bool operator == (const TBitmapIterator& iterator);

	/**
		Returns \c true if the index is valid in the image domain.
	*/
	bool IsValid() const;

	/**
		Move iterator to the position (x, y).
	*/
	void MoveTo(int x, int y);

	/**
		\overload
	*/
	void MoveTo(const istd::CIndex2d& index);

	// Sub-iterators for each dimension.
	XIterator x;
	YIterator y;

private:
	const PixelType* m_imageBufferBeginPtr;
	const PixelType* m_imageBufferEndPtr;
	PixelType* m_imageBufferPtr;
	int m_linesDifference;
	int m_imageWidth;
};



template <typename PixelType>
TBitmapIterator<PixelType>::TBitmapIterator(const iimg::IBitmap* bitmapPtr)
:	x(*this),
	y(*this)
{
	I_ASSERT(bitmapPtr != NULL);

	m_imageBufferPtr = (PixelType*)(bitmapPtr->GetLinePtr(0));
	m_imageBufferBeginPtr = m_imageBufferPtr;
	m_linesDifference = bitmapPtr->GetLinesDifference();
	m_imageWidth = bitmapPtr->GetImageSize().GetX();

	int imageHeight = bitmapPtr->GetImageSize().GetY();

	m_imageBufferEndPtr = m_imageBufferPtr + m_linesDifference * imageHeight;
}


template <typename PixelType>
TBitmapIterator<PixelType>::TBitmapIterator(const TBitmapIterator& iterator)
:	m_imageBufferBeginPtr(iterator.m_imageBufferBeginPtr),
	m_imageBufferEndPtr(iterator.m_imageBufferEndPtr),
	m_imageBufferPtr(iterator.m_imageBufferPtr),
	m_linesDifference(iterator.m_linesDifference),
	m_imageWidth(iterator.m_imageWidth),
	x(*this),
	y(*this)
{
}


template <typename PixelType>
inline typename TBitmapIterator<PixelType>::ValueType& TBitmapIterator<PixelType>::operator*() const
{
	I_ASSERT(IsValid());
		
	return *m_imageBufferPtr;
}


template <typename PixelType>
inline bool TBitmapIterator<PixelType>::IsValid() const
{
	return	((m_imageBufferPtr >= m_imageBufferBeginPtr) &&
			(m_imageBufferPtr < m_imageBufferEndPtr));
}


template <typename PixelType>
inline void TBitmapIterator<PixelType>::MoveTo(int x, int y)
{
	m_imageBufferPtr = const_cast<PixelType*>(m_imageBufferBeginPtr) + x + y * m_linesDifference;
}


template <typename PixelType>
inline void TBitmapIterator<PixelType>::MoveTo(const istd::CIndex2d& index)
{
	MoveTo(index.GetX(), index.GetY());
}


template <typename PixelType>
inline bool TBitmapIterator<PixelType>::operator < (const TBitmapIterator& iterator)
{
	I_ASSERT(m_imageBufferBeginPtr == iterator.m_imageBufferBeginPtr);

	return (m_imageBufferPtr < iterator.m_imageBufferPtr);
}


template <typename PixelType>
inline bool TBitmapIterator<PixelType>::operator > (const TBitmapIterator& iterator)
{
	I_ASSERT(m_imageBufferBeginPtr == iterator.m_imageBufferBeginPtr);

	return (m_imageBufferPtr > iterator.m_imageBufferPtr);
}


template <typename PixelType>
inline bool TBitmapIterator<PixelType>::operator == (const TBitmapIterator& iterator)
{
	I_ASSERT(m_imageBufferBeginPtr == iterator.m_imageBufferBeginPtr);

	return (m_imageBufferPtr == iterator.m_imageBufferPtr);
}


// public methods of embedded class XIterator

template <typename PixelType>
TBitmapIterator<PixelType>::XIterator::XIterator(TBitmapIterator& iterator)
:	m_iterator(iterator)
{
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::XIterator::operator++(int difference)
{
	TBitmapIterator<PixelType> temp(m_iterator);
	
	m_iterator.m_imageBufferPtr++;

	return temp;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::XIterator::operator++()
{
	m_iterator.m_imageBufferPtr++;

	return m_iterator;
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::XIterator::operator--(int difference)
{
	TBitmapIterator<PixelType> temp(m_iterator);
	
	m_iterator.m_imageBufferPtr--;

	return temp;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::XIterator::operator--()
{
	m_iterator.m_imageBufferPtr--;

	return m_iterator;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::XIterator::operator+=(int difference)
{
	m_iterator.m_imageBufferPtr += difference;

	return m_iterator;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::XIterator::operator-=(int difference)
{
	m_iterator.m_imageBufferPtr -= difference;

	return m_iterator;
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::XIterator::operator-(int difference)
{
	TBitmapIterator<PixelType> output(m_iterator);

	output.m_imageBufferPtr -= difference;

	return output;
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::XIterator::operator+(int difference)
{
	TBitmapIterator<PixelType> output(m_iterator);

	output.m_imageBufferPtr += difference;

	return output;
}


// public methods of embedded class YIterator

template <typename PixelType>
TBitmapIterator<PixelType>::YIterator::YIterator(TBitmapIterator& iterator)
:	m_iterator(iterator)
{
}

template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::YIterator::operator++(int)
{
	TBitmapIterator<PixelType> temp = m_iterator;

	m_iterator.m_imageBufferPtr += m_iterator.m_linesDifference;

	return temp;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::YIterator::operator++()
{
	m_iterator.m_imageBufferPtr += m_iterator.m_linesDifference;

	return m_iterator;
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::YIterator::operator--(int)
{
	TBitmapIterator<PixelType> temp = m_iterator;

	m_iterator.m_imageBufferPtr -= m_iterator.m_linesDifference;

	return temp;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::YIterator::operator--()
{
	m_iterator.m_imageBufferPtr -= m_iterator.m_linesDifference;

	return m_iterator;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::YIterator::operator+=(int difference)
{
	m_iterator.m_imageBufferPtr += (difference * m_iterator.m_linesDifference);

	return m_iterator;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::YIterator::operator-=(int difference)
{
	m_iterator.m_imageBufferPtr -= (difference * m_iterator.m_linesDifference);

	return m_iterator;
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::YIterator::operator-(int difference)
{
	TBitmapIterator<PixelType> temp(m_iterator);

	temp.m_imageBufferPtr -= difference;

	return temp;
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::YIterator::operator+(int difference)
{
	TBitmapIterator<PixelType> temp(m_iterator);

	temp.m_imageBufferPtr += difference;

	return temp;
}


} // namespace iimg


#endif // !iimg_TBitmapIterator_included

