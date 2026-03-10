// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "TNullableTest.h"


// protected slots

void TNullableTest::initTestCase()
{
}


void TNullableTest::DefaultConstructorTest()
{
	istd::TNullable<int> nullable;
	
	QVERIFY(!nullable.IsValid());
	QVERIFY(!nullable.IsNull());
	QVERIFY(!nullable.HasValue());
	QVERIFY(!nullable.has_value());
}


void TNullableTest::ValueConstructorTest()
{
	istd::TNullable<int> nullable(42);
	
	QVERIFY(nullable.IsValid());
	QVERIFY(!nullable.IsNull());
	QVERIFY(nullable.HasValue());
	QCOMPARE(nullable.GetValue(), 42);
}


void TNullableTest::IsValidTest()
{
	istd::TNullable<int> nullable1;
	QVERIFY(!nullable1.IsValid());
	
	istd::TNullable<int> nullable2(10);
	QVERIFY(nullable2.IsValid());
	
	istd::TNullable<int> nullable3;
	nullable3.SetNull();
	QVERIFY(nullable3.IsValid());
}


void TNullableTest::IsNullTest()
{
	istd::TNullable<int> nullable;
	QVERIFY(!nullable.IsNull());
	
	nullable = 50;
	QVERIFY(!nullable.IsNull());
	
	nullable.SetNull();
	QVERIFY(nullable.IsNull());
}


void TNullableTest::HasValueTest()
{
	istd::TNullable<int> nullable;
	QVERIFY(!nullable.HasValue());
	
	nullable = 50;
	QVERIFY(nullable.HasValue());
	
	nullable.SetNull();
	QVERIFY(!nullable.HasValue());
}


void TNullableTest::SetNullTest()
{
	istd::TNullable<int> nullable(42);
	
	QVERIFY(nullable.HasValue());
	QVERIFY(!nullable.IsNull());
	
	nullable.SetNull();
	
	QVERIFY(!nullable.HasValue());
	QVERIFY(nullable.IsNull());
	QVERIFY(nullable.IsValid());
}


void TNullableTest::ResetTest()
{
	istd::TNullable<int> nullable(100);
	
	QVERIFY(nullable.IsValid());
	QVERIFY(nullable.HasValue());
	
	nullable.Reset();
	
	QVERIFY(!nullable.IsValid());
	QVERIFY(!nullable.HasValue());
	QVERIFY(!nullable.IsNull());
}


void TNullableTest::EmplaceTest()
{
	istd::TNullable<int> nullable;
	
	QVERIFY(!nullable.HasValue());
	
	int& ref = nullable.Emplace();
	ref = 77;
	
	QVERIFY(nullable.HasValue());
	QVERIFY(nullable.IsValid());
	QCOMPARE(nullable.GetValue(), 77);
}


void TNullableTest::GetValueTest()
{
	istd::TNullable<int> nullable(123);
	
	QCOMPARE(nullable.GetValue(), 123);
	QCOMPARE(nullable.value(), 123);
	
	// Test reference modification
	nullable.GetValue() = 456;
	QCOMPARE(nullable.GetValue(), 456);
}


void TNullableTest::ValueOrTest()
{
	istd::TNullable<int> nullable1(42);
	QCOMPARE(nullable1.value_or(100), 42);
	
	istd::TNullable<int> nullable2;
	QCOMPARE(nullable2.value_or(100), 100);
	
	istd::TNullable<int> nullable3;
	nullable3.SetNull();
	QCOMPARE(nullable3.value_or(200), 200);
}


void TNullableTest::ComparisonOperatorsTest()
{
	istd::TNullable<int> nullable1(42);
	istd::TNullable<int> nullable2(42);
	istd::TNullable<int> nullable3(99);
	istd::TNullable<int> nullable4;
	
	// Test TNullable comparison
	QVERIFY(nullable1 == nullable2);
	QVERIFY(nullable1 != nullable3);
	QVERIFY(nullable1 != nullable4);
	
	// Test value comparison
	QVERIFY(nullable1 == 42);
	QVERIFY(nullable1 != 99);
	
	// Test nullptr comparison
	QVERIFY(nullable4 == nullptr);
	QVERIFY(nullable1 != nullptr);
}


void TNullableTest::AssignmentOperatorTest()
{
	istd::TNullable<int> nullable1;
	
	// Assign value
	nullable1 = 42;
	QVERIFY(nullable1.HasValue());
	QCOMPARE(nullable1.GetValue(), 42);
	
	// Assign nullptr
	nullable1 = nullptr;
	QVERIFY(nullable1.IsNull());
	QVERIFY(!nullable1.HasValue());
	
	// Assign from another TNullable
	istd::TNullable<int> nullable2(100);
	nullable1 = nullable2;
	QVERIFY(nullable1.HasValue());
	QCOMPARE(nullable1.GetValue(), 100);
}


void TNullableTest::cleanupTestCase()
{
}


I_ADD_TEST(TNullableTest);
