// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CClassInfoTest.h"


// protected slots

void CClassInfoTest::initTestCase()
{
}


void CClassInfoTest::DefaultConstructorTest()
{
	istd::CClassInfo info;
	QVERIFY(!info.IsValid());
	QVERIFY(info.GetName().isEmpty());
}


void CClassInfoTest::TypeInfoConstructorTest()
{
	istd::CClassInfo info(typeid(int));
	QVERIFY(info.IsValid());
	QVERIFY(!info.GetName().isEmpty());
}


void CClassInfoTest::NameConstructorTest()
{
	QByteArray name = "TestClass";
	istd::CClassInfo info(name);
	QVERIFY(info.IsValid());
	QCOMPARE(info.GetName(), name);
}


void CClassInfoTest::GetNameTest()
{
	// Test GetInfo template method
	istd::CClassInfo info = istd::CClassInfo::GetInfo<int>();
	QVERIFY(info.IsValid());
	
	// Test GetName template method
	QByteArray name = istd::CClassInfo::GetName<double>();
	QVERIFY(!name.isEmpty());
}


void CClassInfoTest::IsValidTest()
{
	istd::CClassInfo invalidInfo;
	QVERIFY(!invalidInfo.IsValid());
	
	istd::CClassInfo validInfo(typeid(QString));
	QVERIFY(validInfo.IsValid());
}


void CClassInfoTest::ComparisonOperatorsTest()
{
	istd::CClassInfo info1(typeid(int));
	istd::CClassInfo info2(typeid(int));
	istd::CClassInfo info3(typeid(double));
	
	// Test equality
	QVERIFY(info1 == info2);
	QVERIFY(!(info1 == info3));
	
	// Test inequality
	QVERIFY(info1 != info3);
	QVERIFY(!(info1 != info2));
	
	// Test less than and greater than
	// Note: These comparisons are based on name comparison
	bool lessThan = (info1 < info3) || (info3 < info1);
	QVERIFY(lessThan); // At least one should be true for different types
}


void CClassInfoTest::IsTypeTest()
{
	istd::CClassInfo info(typeid(int));
	QVERIFY(info.IsType<int>());
	QVERIFY(!info.IsType<double>());
}


void CClassInfoTest::GetInfoStaticTest()
{
	istd::CClassInfo info1 = istd::CClassInfo::GetInfo<QString>();
	istd::CClassInfo info2(typeid(QString));
	
	QVERIFY(info1 == info2);
}


void CClassInfoTest::cleanupTestCase()
{
}


I_ADD_TEST(CClassInfoTest);
