// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/Test/TMsbWordTest.h>


// ACF includes
#include <ibase/TMsbWord.h>
#include <iser/CMemoryArchive.h>


void TMsbWordTest::initTestCase()
{
	// Setup
}


void TMsbWordTest::cleanupTestCase()
{
	// Cleanup
}


// Constructor tests

void TMsbWordTest::testDefaultConstructor()
{
	// Test default constructor for different sizes
	ibase::TMsbWord<1> byte;
	ibase::TMsbWord<2> word;
	ibase::TMsbWord<4> dword;
	
	// Just verify they compile and can be created
	Q_UNUSED(byte);
	Q_UNUSED(word);
	Q_UNUSED(dword);
}


void TMsbWordTest::testValueConstructor()
{
	// Test with a known value
	quint32 testValue = 0x12345678;
	ibase::TMsbWord<4> dword(testValue);
	
	// Verify it converts back correctly
	QCOMPARE(static_cast<quint32>(dword), testValue);
}


void TMsbWordTest::testCopyConstructor()
{
	quint32 testValue = 0xABCDEF01;
	ibase::TMsbWord<4> original(testValue);
	ibase::TMsbWord<4> copy(original);
	
	// Verify copy has the same value
	QCOMPARE(static_cast<quint32>(copy), testValue);
	QCOMPARE(static_cast<quint32>(copy), static_cast<quint32>(original));
}


// Conversion tests

void TMsbWordTest::testMsbConversion()
{
	// Test MSB (Most Significant Byte first) conversion
	// 0x12345678 should store bytes in order: 78, 56, 34, 12 in memory
	quint32 testValue = 0x12345678;
	ibase::TMsbWord<4> dword(testValue);
	
	// Converting back should give the original value
	QCOMPARE(static_cast<quint32>(dword), testValue);
}


void TMsbWordTest::testLsbConversion()
{
	// Test LSB (Least Significant Byte first) conversion
	// For 0x12345678, GetLsb() should reverse byte order
	quint32 testValue = 0x12345678;
	ibase::TMsbWord<4> dword(testValue);
	
	// GetLsb should return value with reversed byte order
	quint32 lsbValue = dword.GetLsb();
	QCOMPARE(lsbValue, 0x78563412u);
}


void TMsbWordTest::testLsbMsbRoundtrip()
{
	// Test that converting MSB -> LSB -> MSB works correctly
	quint32 testValue = 0xAABBCCDD;
	ibase::TMsbWord<4> dword(testValue);
	
	// Get LSB value
	quint32 lsbValue = dword.GetLsb();
	
	// Create a new TMsbWord from the LSB value
	ibase::TMsbWord<4> dwordFromLsb(lsbValue);
	
	// GetLsb on this should give us back the original value
	QCOMPARE(dwordFromLsb.GetLsb(), testValue);
}


// Different sizes

void TMsbWordTest::testByteSize()
{
	// Test 1-byte size (TMsbWord<1>)
	quint32 testValue = 0x42;
	ibase::TMsbWord<1> byte(testValue);
	
	QCOMPARE(static_cast<quint32>(byte), testValue);
	
	// Test with larger value (should be truncated to 1 byte)
	quint32 largeValue = 0x1234;
	ibase::TMsbWord<1> byteLarge(largeValue);
	QCOMPARE(static_cast<quint32>(byteLarge), 0x34u);
}


void TMsbWordTest::testWordSize()
{
	// Test 2-byte size (TMsbWord<2>)
	quint32 testValue = 0x1234;
	ibase::TMsbWord<2> word(testValue);
	
	QCOMPARE(static_cast<quint32>(word), testValue);
	
	// Test LSB conversion for 2-byte value
	QCOMPARE(word.GetLsb(), 0x3412u);
}


void TMsbWordTest::testDWordSize()
{
	// Test 4-byte size (TMsbWord<4>)
	quint32 testValue = 0x12345678;
	ibase::TMsbWord<4> dword(testValue);
	
	QCOMPARE(static_cast<quint32>(dword), testValue);
	
	// Test using typedefs
	ibase::CMsbDWord dwordTypedef(testValue);
	QCOMPARE(static_cast<quint32>(dwordTypedef), testValue);
	
	ibase::CMsbWord wordTypedef(0x1234);
	QCOMPARE(static_cast<quint32>(wordTypedef), 0x1234u);
}


// Serialization tests

void TMsbWordTest::testSerialization()
{
	// Create a TMsbWord with a known value
	quint32 testValue = 0xDEADBEEF;
	ibase::TMsbWord<4> original(testValue);
	
	// Serialize to memory
	iser::CMemoryArchive archiveWrite;
	archiveWrite.BeginStoring();
	QVERIFY(original.Serialize(archiveWrite));
	archiveWrite.EndStoring();
	
	// Deserialize from memory
	ibase::TMsbWord<4> deserialized;
	iser::CMemoryArchive archiveRead(archiveWrite.GetData());
	archiveRead.BeginRestoring();
	QVERIFY(deserialized.Serialize(archiveRead));
	archiveRead.EndRestoring();
	
	// Verify the deserialized value matches the original
	QCOMPARE(static_cast<quint32>(deserialized), testValue);
}
