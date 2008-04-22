#include "iqt/CBitmap.h"


#include <QVector>
#include <QColor>

#include "istd/TChangeNotifier.h"


namespace iqt
{


CBitmap::CBitmap()
{
}


CBitmap::CBitmap(const CBitmap& bitmap)
:	BaseClass(bitmap)
{
}


// reimplemented (iimg::IBitmap)

bool CBitmap::CreateBitmap(const istd::CIndex2d& size, int pixelBitsCount, int componentsCount)
{
	QImage::Format imageFormat = CalcQtFormat(pixelBitsCount, componentsCount);
	if (imageFormat != QImage::Format_Invalid){
		QImage image(size.GetX(), size.GetY(), imageFormat);

		m_externalBuffer.Reset();

		return SetQImage(image);
	}

	return false;
}


bool CBitmap::CreateBitmap(const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference, int pixelBitsCount, int componentsCount)
{
	QImage::Format imageFormat = CalcQtFormat(pixelBitsCount, componentsCount);
	if (imageFormat != QImage::Format_Invalid){
		QImage image((I_BYTE*)dataPtr, size.GetX(), size.GetY(), imageFormat);
		if ((linesDifference != 0) && (linesDifference != image.scanLine(1) - image.scanLine(0))){
			return false;
		}

		m_externalBuffer.SetPtr((I_BYTE*)dataPtr, releaseFlag);

		return SetQImage(image);
	}

	return false;
}


int CBitmap::GetLinesDifference() const
{
	return scanLine(1) - scanLine(0);
}


int CBitmap::GetPixelBitsCount() const
{
	return GetComponentsCount() * 8;
}


const void* CBitmap::GetLinePtr(int positionY) const
{
	I_ASSERT(positionY >= 0);
	I_ASSERT(positionY < GetImageSize().GetY());

	return scanLine(positionY);
}


void* CBitmap::GetLinePtr(int positionY)
{
	I_ASSERT(positionY >= 0);
	I_ASSERT(positionY < GetImageSize().GetY());

	return scanLine(positionY);
}


// reimplemented (iimg::IRasterImage)

istd::CIndex2d CBitmap::GetImageSize() const
{
	return GetCIndex2d(size());
}


int CBitmap::GetComponentsCount() const
{
	switch (BaseClass::format()){
	case QImage::Format_Indexed8:
		return 1;

	case QImage::Format_RGB32:
	case QImage::Format_ARGB32:
		return 4;

	default:
		return 0;
	}
}


bool CBitmap::CopyImageFrom(const IRasterImage& image)
{
	const CBitmap* bitmapPtr = dynamic_cast<const CBitmap*>(&image);

	if (bitmapPtr != NULL){
		istd::CChangeNotifier notifier(this);

		BaseClass::operator=(*bitmapPtr);

		return true;
	}

	return false;
}


// protected methods

QImage::Format CBitmap::CalcQtFormat(int pixelBitsCount, int componentsCount) const
{
	switch (pixelBitsCount){
	case 8:
		if (componentsCount == 1){
			return QImage::Format_Indexed8;
		}
		break;

	case 32:	
		if (componentsCount == 4){
			return QImage::Format_ARGB32;
		}
		break;
	}

	return QImage::Format_Invalid;
}


bool CBitmap::SetQImage(const QImage& image)
{
	BaseClass::operator=(image);

	if (image.format() == QImage::Format_Indexed8){
		QVector<QRgb> colorTable(256);

		for (int colorIndex = 0; colorIndex < 256; ++colorIndex){
			colorTable[colorIndex] = ::qRgb(colorIndex, colorIndex, colorIndex);
		}

		BaseClass::setColorTable(colorTable);
	}

	return true;
}


} // namespace iqt


