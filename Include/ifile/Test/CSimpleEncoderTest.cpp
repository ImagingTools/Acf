// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CSimpleEncoderTest.h>


// ACF includes
#include <ifile/CSimpleEncoder.h>


void CSimpleEncoderTest::DoEncodeDecodeTest()
{
	// Test basic encode/decode roundtrip
	const int dataSize = 10;
	quint8 originalData[dataSize] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	quint8 encodedData[dataSize];
	quint8 decodedData[dataSize];

	// Encode the data
	bool encodeResult = ifile::CSimpleEncoder::Encode(originalData, encodedData, dataSize);
	QVERIFY(encodeResult);

	// Decode the data
	bool decodeResult = ifile::CSimpleEncoder::Decode(encodedData, decodedData, dataSize);
	QVERIFY(decodeResult);

	// Verify that decoded data matches original
	for (int i = 0; i < dataSize; i++)
	{
		QCOMPARE(decodedData[i], originalData[i]);
	}
}


void CSimpleEncoderTest::DoEmptyDataTest()
{
	// Test with zero-length data
	quint8 data[1] = {0};
	quint8 encoded[1];
	quint8 decoded[1];

	bool encodeResult = ifile::CSimpleEncoder::Encode(data, encoded, 0);
	QVERIFY(encodeResult);

	bool decodeResult = ifile::CSimpleEncoder::Decode(encoded, decoded, 0);
	QVERIFY(decodeResult);
}


void CSimpleEncoderTest::DoSingleByteTest()
{
	// Test with single byte
	quint8 originalByte = 42;
	quint8 encodedByte;
	quint8 decodedByte;

	bool encodeResult = ifile::CSimpleEncoder::Encode(&originalByte, &encodedByte, 1);
	QVERIFY(encodeResult);

	bool decodeResult = ifile::CSimpleEncoder::Decode(&encodedByte, &decodedByte, 1);
	QVERIFY(decodeResult);

	QCOMPARE(decodedByte, originalByte);
}


void CSimpleEncoderTest::DoLargeDataTest()
{
	// Test with larger data set
	const int dataSize = 256;
	quint8 originalData[dataSize];
	quint8 encodedData[dataSize];
	quint8 decodedData[dataSize];

	// Fill with sequential values
	for (int i = 0; i < dataSize; i++)
	{
		originalData[i] = static_cast<quint8>(i);
	}

	// Encode the data
	bool encodeResult = ifile::CSimpleEncoder::Encode(originalData, encodedData, dataSize);
	QVERIFY(encodeResult);

	// Decode the data
	bool decodeResult = ifile::CSimpleEncoder::Decode(encodedData, decodedData, dataSize);
	QVERIFY(decodeResult);

	// Verify that decoded data matches original
	for (int i = 0; i < dataSize; i++)
	{
		QCOMPARE(decodedData[i], originalData[i]);
	}
}


I_ADD_TEST(CSimpleEncoderTest);


