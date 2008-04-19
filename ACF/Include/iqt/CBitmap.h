#ifndef iqt_CBitmap_included
#define iqt_CBitmap_included


// Qt includes
#include <QImage>

#include "istd/TOptDelPtr.h"

#include "iimg/CBitmapBase.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Bitmap implementation based on Qt QImage.
*/
class CBitmap: public QImage, public iimg::CBitmapBase
{
public:
	typedef QImage BaseClass;
	typedef iimg::CBitmapBase BaseClass2;

	CBitmap();
	CBitmap(const CBitmap& bitmap);

	// reimplemented (iimg::IBitmap)
	virtual bool CreateBitmap(const istd::CIndex2d& size, int pixelBitsCount = 8, int componentsCount = 1);
	virtual bool CreateBitmap(const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference = 0, int pixelBitsCount = 8, int componentsCount = 1);
	virtual int GetLinesDifference() const;
	virtual int GetPixelBitsCount() const;
	virtual const void* GetLinePtr(int positionY) const;
	virtual void* GetLinePtr(int positionY);

	// reimplemented (iimg::IRasterImage)
	virtual istd::CIndex2d GetImageSize() const;
	virtual int GetComponentsCount() const;

protected:
	QImage::Format CalcQtFormat(int pixelBitsCount, int componentsCount) const;
	bool SetQImage(const QImage& image, int pixelBitsCount, int componentsCount);

private:
	istd::TOptDelPtr<I_BYTE, true> m_externalBuffer;
	int m_pixelBitsCount;
	int m_componentsCount;
};


} // namespace iqt


#endif iqt_CBitmap_included


