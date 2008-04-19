#include "iqt/CBitmap.h"


#include <QVector>
#include <QColor>


namespace iqt
{


CBitmap::CBitmap()
:	m_pixelBitsCount(0),
	m_componentsCount(0)
{
}


CBitmap::CBitmap(const CBitmap& bitmap)
:	BaseClass(bitmap),
	m_pixelBitsCount(bitmap.m_pixelBitsCount),
	m_componentsCount(bitmap.m_componentsCount)
{
}


// reimplemented (iimg::IBitmap)

bool CBitmap::CreateBitmap(const istd::CIndex2d& size, int pixelBitsCount, int componentsCount)
{
	QImage::Format imageFormat = CalcQtFormat(pixelBitsCount, componentsCount);
	if (imageFormat != QImage::Format_Invalid){
		QImage image(size.GetX(), size.GetY(), imageFormat);

		m_externalBuffer.Reset();

		return SetQImage(image, pixelBitsCount, componentsCount);
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

		return SetQImage(image, pixelBitsCount, componentsCount);
	}

	return false;
}


int CBitmap::GetLinesDifference() const
{
	return scanLine(1) - scanLine(0);
}


int CBitmap::GetPixelBitsCount() const
{
	return m_pixelBitsCount;
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
	return m_componentsCount;
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

	case 24:	
		if (componentsCount == 3){
			return QImage::Format_RGB32;
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


bool CBitmap::SetQImage(const QImage& image, int pixelBitsCount, int componentsCount)
{
	BaseClass::operator=(image);

	m_pixelBitsCount = pixelBitsCount;
	m_componentsCount = componentsCount;

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


