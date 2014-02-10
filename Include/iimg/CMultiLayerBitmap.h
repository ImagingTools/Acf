#ifndef iimg_CMultiLayerBitmap_included
#define iimg_CMultiLayerBitmap_included


// ACF includes
#include "iimg/IMultiLayerBitmap.h"
#include "iimg/CBitmapDocument.h"


namespace iimg
{


class CMultiLayerBitmap:
			public CBitmapDocument,
			virtual public iimg::IMultiLayerBitmap
{
public:
	typedef CBitmapDocument BaseClass;

	// reimplemented (iimg::IMultiLayerBitmap)
	virtual void Reset();
	virtual iimg::IBitmap* InsertBitmap(
		iimg::IBitmap::PixelFormat pixelFormat,
		const istd::CIndex2d& size);
	virtual iimg::IBitmap* InsertBitmap(
		iimg::IBitmap::PixelFormat pixelFormat,
		const istd::CIndex2d& size,
		void* dataPtr,
		bool releaseFlag,
		int linesDifference = 0);
	virtual void RemoveBitmap(int index);
};


} // namespace iimg


#endif // iimg_CMultiLayerBitmap_included


