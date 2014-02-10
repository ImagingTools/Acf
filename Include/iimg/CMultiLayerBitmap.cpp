#include "iimg/CMultiLayerBitmap.h"


namespace iimg
{


// public methods

// reimplemented (iimg::IMultiLayerBitmap)

void CMultiLayerBitmap::Reset()
{
	BaseClass::ResetPages();
}


iimg::IBitmap* CMultiLayerBitmap::InsertBitmap(
			iimg::IBitmap::PixelFormat pixelFormat, 
			const istd::CIndex2d& size)
{
	iimg::IBitmap* pagePtr = dynamic_cast<iimg::IBitmap*>(BaseClass::InsertPage());
	if (!pagePtr->CreateBitmap(pixelFormat, size)){
		RemovePage(GetPagesCount() - 1);

		return NULL;
	}

	return pagePtr;
}


iimg::IBitmap* CMultiLayerBitmap::InsertBitmap(
			iimg::IBitmap::PixelFormat pixelFormat, 
			const istd::CIndex2d& size, 
			void* dataPtr, 
			bool releaseFlag, 
			int linesDifference /*= 0*/)
{
	iimg::IBitmap* pagePtr = dynamic_cast<iimg::IBitmap*>(BaseClass::InsertPage());
	if (!pagePtr->CreateBitmap(pixelFormat, size, dataPtr, releaseFlag, linesDifference)){
		RemovePage(GetPagesCount() - 1);

		return NULL;
	}

	return pagePtr;
}


void CMultiLayerBitmap::RemoveBitmap(int index)
{
	BaseClass::RemovePage(index);
}


} // namespace iimg


