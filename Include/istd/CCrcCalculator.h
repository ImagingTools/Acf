// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QVector>
#include <QtCore/QString>


namespace istd
{


/**
	Helper class for CRC-32 checksum calculation.
*/
class CCrcCalculator
{
public:
	typedef QVector<quint8> ByteStream;

	/**
		Get 32-bit CRC value for the given memory block.
	*/
	static quint32 GetCrcFromData(const quint8* dataPtr, int dataSize);

	/**
		\overload
	*/
	static quint32 GetCrcFromStream(const ByteStream& byteStream);

	/**
		Get 32-bit CRC value for a given file.
	*/
	static quint32 GetCrcFromFile(const QString& fileName);

protected:
	/**
		Update CRC value for the new input.
	*/
	static void UpdateCrc(const quint8& byte, quint32& dwCrc32);

private:
	static quint32 s_crcTable[256];
};


// protected inline methods

inline void CCrcCalculator::UpdateCrc(const quint8& byte, quint32& crcValue)
{
	crcValue = (crcValue >> 8) ^ s_crcTable[byte ^ (crcValue & 0x000000FF)];
}


} // namespace istd



