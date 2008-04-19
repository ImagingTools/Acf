#include "iimg/CBitmapBase.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iimg
{


// reimplemented (iimg::IBitmap)

int CBitmapBase::GetLineBytesCount() const
{
	istd::CIndex2d size = GetImageSize();
	int bytesPerPixel = ((GetPixelBitsCount() + 7) / 8);
	return bytesPerPixel * size.GetX();
}


int CBitmapBase::GetComponentBitsCount(int /*componentIndex*/) const
{
	return GetPixelBitsCount() / GetComponentBitsCount();
}


// reimplemented (iimg::IRasterImage)

bool CBitmapBase::IsEmpty() const
{
	istd::CIndex2d size = GetImageSize();

	return (size.GetX() > 0) && (size.GetY() > 0);
}


icmm::CVarColor CBitmapBase::GetColorAt(const istd::CIndex2d& position) const
{
	I_ASSERT(position.IsValid());
	I_ASSERT(position.IsInside(GetImageSize()));

	int componentsCount = GetComponentsCount();

	icmm::CVarColor retVal(componentsCount);

	int bitsOffsetX = GetPixelBitsCount() * position.GetX();
	const I_BYTE* linePtr = (const I_BYTE*)GetLinePtr(position.GetY());
	I_ASSERT(linePtr != NULL);

	for (int i = 0; i < componentsCount; ++i){
		int bitsPerComponent = GetComponentBitsCount(i);

		if (bitsPerComponent > 0){
			int restComponentBits = bitsPerComponent;
			int componentValue = 0;
			int pixelByteOffsetX = bitsOffsetX >> 3;
			int pixelBitOffsetX = bitsOffsetX & 7;
			while (restComponentBits){
				componentValue <<= (8 - pixelBitOffsetX);
				componentValue = (linePtr[pixelByteOffsetX] >> pixelBitOffsetX) & ((1 << restComponentBits) - 1);
				restComponentBits -= 8 - pixelBitOffsetX;
				pixelBitOffsetX = 0;
				++pixelByteOffsetX;
			}

			retVal[i] = double(componentValue) / ((1 << bitsPerComponent) - 1);
		}
		else{
			retVal[i] = 0.0;
		}
	}

	return retVal;
}


bool CBitmapBase::SetColorAt(const istd::CIndex2d& /*position*/, const icmm::CVarColor& /*color*/)
{
	return false;	// TODO: implement setting of general color value to bitmap.
}


// reimplemented (iser::ISerializable)

bool CBitmapBase::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	bool isStoring = archive.IsStoring();

	static iser::CArchiveTag headerTag("BitmapHeader", "Header of bitmap");
	retVal = retVal && archive.BeginTag(headerTag);

	istd::CIndex2d size;
	int pixelBitsCount = 0;
	int componentsCount = 0;

	if (isStoring){
		size = GetImageSize();
		pixelBitsCount = GetPixelBitsCount();
		componentsCount = GetComponentsCount();
	}

	static iser::CArchiveTag sizeTag("Size", "Size of bitmap");
	retVal = retVal && archive.BeginTag(sizeTag);

	static iser::CArchiveTag sizeXTag("X", "Bitmap width");
	retVal = retVal && archive.BeginTag(sizeXTag);
	retVal = retVal && archive.Process(size[0]);
	retVal = retVal && archive.EndTag(sizeXTag);

	static iser::CArchiveTag sizeYTag("Y", "Bitmap height");
	retVal = retVal && archive.BeginTag(sizeYTag);
	retVal = retVal && archive.Process(size[1]);
	retVal = retVal && archive.EndTag(sizeYTag);

	retVal = retVal && archive.EndTag(sizeTag);

	static iser::CArchiveTag pixelBitsCountTag("PixelBitsCount", "Number of bits per single pixel");
	retVal = retVal && archive.BeginTag(pixelBitsCountTag);
	retVal = retVal && archive.Process(pixelBitsCount);
	retVal = retVal && archive.EndTag(pixelBitsCountTag);

	static iser::CArchiveTag componentsCountTag("ComponentsCount", "Number of components per pixel");
	retVal = retVal && archive.BeginTag(componentsCountTag);
	retVal = retVal && archive.Process(componentsCount);
	retVal = retVal && archive.EndTag(componentsCountTag);

	retVal = retVal && archive.EndTag(headerTag);

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	if (!isStoring){
		if (!CreateBitmap(size, pixelBitsCount, componentsCount)){
			return false;
		}
	}

	static iser::CArchiveTag dataTag("BitmapData", "Bitmap data section");
	retVal = retVal && archive.BeginTag(dataTag);

	int lineBytesCount = GetLineBytesCount();

	for (int lineIndex = 0; lineIndex < size.GetY(); ++lineIndex){
		void* linePtr = GetLinePtr(lineIndex);

		static iser::CArchiveTag lineTag("Line", "Single bitmap line");
		retVal = retVal && archive.BeginTag(lineTag);
		retVal = retVal && archive.ProcessData(linePtr, lineBytesCount);
		retVal = retVal && archive.EndTag(lineTag);
	}

	retVal = retVal && archive.EndTag(dataTag);

	return retVal;
}


} // namespace iimg


