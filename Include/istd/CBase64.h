#ifndef istd_CBase64_included
#define istd_CBase64_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QVector>

// ACF includes
#include <istd/istd.h>


namespace istd
{


/**
	Base64 converter.
*/
class CBase64
{
public:
	/**
		Convert a memory block of a given size to Base64 string.
	*/
	static QByteArray ConvertToBase64(const void* dataPtrPtr, int size);

	/**
		Convert Base64 encoded string to raw data.
	*/
	static QVector<quint8> ConvertFromBase64(const QByteArray& base64String);

private:
	static char SixBitToChar(quint8 b);
	static quint8 CharToSixBit(char c);
};


} // namespace istd


#endif // !istd_CBase64_included


