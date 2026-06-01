// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "TNullableValueTest.h"


void TNullableValueTest::initTestCase()
{
}


void TNullableValueTest::ConstructorTest()
{
	istd::TNullableValue<QString> defaultValue;
	QVERIFY(!defaultValue.IsValid());
	QVERIFY(!defaultValue.IsNull());
	QVERIFY(!defaultValue.HasValue());

	const QString copiedText("copy");
	istd::TNullableValue<QString> copiedValue(copiedText);
	QVERIFY(copiedValue.IsValid());
	QVERIFY(copiedValue.HasValue());
	QCOMPARE(copiedValue.GetValue(), copiedText);

	QString movedText("move");
	istd::TNullableValue<QString> movedValue(std::move(movedText));
	QVERIFY(movedValue.IsValid());
	QVERIFY(movedValue.HasValue());
	QCOMPARE(movedValue.GetValue(), QString("move"));
}


void TNullableValueTest::ValidityMethodsTest()
{
	istd::TNullableValue<int> value;
	QVERIFY(!value.IsValid());
	QVERIFY(!value.isValid());
	QVERIFY(!value.IsNull());
	QVERIFY(!value.isNull());
	QVERIFY(!value.HasValue());

	value = 42;
	QVERIFY(value.IsValid());
	QVERIFY(value.isValid());
	QVERIFY(!value.IsNull());
	QVERIFY(!value.isNull());
	QVERIFY(value.HasValue());

	value.SetNull();
	QVERIFY(value.IsValid());
	QVERIFY(value.isValid());
	QVERIFY(value.IsNull());
	QVERIFY(value.isNull());
	QVERIFY(!value.HasValue());
}


void TNullableValueTest::PointerAccessTest()
{
	istd::TNullableValue<QString> value(QString("abc"));
	QVERIFY(value.GetPtr() != nullptr);
	QCOMPARE(*value.GetPtr(), QString("abc"));

	const istd::TNullableValue<QString>& constValue = value;
	QVERIFY(constValue.GetPtr() != nullptr);
	QCOMPARE(*constValue.GetPtr(), QString("abc"));

	value->append("d");
	QCOMPARE(value.GetValue(), QString("abcd"));
	QCOMPARE(constValue->size(), 4);
}


void TNullableValueTest::ValueAccessTest()
{
	istd::TNullableValue<QString> value(QString("alpha"));
	QCOMPARE(value.GetValue(), QString("alpha"));
	value.GetValue().append("1");
	QCOMPARE(value.GetValue(), QString("alpha1"));

	const istd::TNullableValue<QString> constValue(QString("beta"));
	QCOMPARE(constValue.GetValue(), QString("beta"));

	QCOMPARE(*value, QString("alpha1"));
	(*value).append("2");
	QCOMPARE(*value, QString("alpha12"));

	QCOMPARE(*constValue, QString("beta"));

	istd::TNullableValue<QString> movedDereference(QString("gamma"));
	QString&& movedDereferenceRef = std::move(movedDereference).operator*();
	QCOMPARE(movedDereferenceRef, QString("gamma"));
	movedDereferenceRef.append("!");
	QCOMPARE(movedDereference.value(), QString("gamma!"));

	const istd::TNullableValue<QString> constMovedDereference(QString("delta"));
	const QString&& constMovedDereferenceRef = std::move(constMovedDereference).operator*();
	QCOMPARE(constMovedDereferenceRef, QString("delta"));

	istd::TNullableValue<QString> valueRef(QString("epsilon"));
	QCOMPARE(valueRef.value(), QString("epsilon"));
	valueRef.value().append("!");
	QCOMPARE(valueRef.GetValue(), QString("epsilon!"));

	const istd::TNullableValue<QString> constValueRef(QString("zeta"));
	QCOMPARE(constValueRef.value(), QString("zeta"));

	istd::TNullableValue<QString> movedValue(QString("eta"));
	QString&& movedValueRef = std::move(movedValue).value();
	QCOMPARE(movedValueRef, QString("eta"));
	movedValueRef.append("!");
	QCOMPARE(movedValue.GetValue(), QString("eta!"));

	const istd::TNullableValue<QString> constMovedValue(QString("theta"));
	const QString&& constMovedValueRef = std::move(constMovedValue).value();
	QCOMPARE(constMovedValueRef, QString("theta"));
}


void TNullableValueTest::ResetMethodsTest()
{
	istd::TNullableValue<int> resetValue(12);
	resetValue.Reset();
	QVERIFY(!resetValue.IsValid());
	QVERIFY(!resetValue.IsNull());
	QVERIFY(!resetValue.HasValue());

	istd::TNullableValue<int> lowerResetValue(34);
	lowerResetValue.reset();
	QVERIFY(!lowerResetValue.IsValid());
	QVERIFY(!lowerResetValue.IsNull());
	QVERIFY(!lowerResetValue.HasValue());
}


void TNullableValueTest::EmplaceMethodsTest()
{
	istd::TNullableValue<int> value;
	int& emplacedRef = value.Emplace();
	emplacedRef = 77;
	QVERIFY(value.IsValid());
	QVERIFY(value.HasValue());
	QCOMPARE(value.GetValue(), 77);

	istd::TNullableValue<QString> lowerValue;
	QString& lowerEmplacedRef = lowerValue.emplace();
	lowerEmplacedRef = "text";
	QVERIFY(lowerValue.IsValid());
	QVERIFY(lowerValue.HasValue());
	QCOMPARE(lowerValue.GetValue(), QString("text"));

	istd::TNullableValue<QString> movedValue;
	QString&& movedInsertRef = std::move(movedValue).emplace(QString("moved"));
	QCOMPARE(movedInsertRef, QString("moved"));
	QVERIFY(movedValue.IsValid());
	QVERIFY(movedValue.HasValue());
	QCOMPARE(movedValue.GetValue(), QString("moved"));
}


void TNullableValueTest::SetNullAndValueOrTest()
{
	istd::TNullableValue<QString> value(QString("text"));
	QCOMPARE(value.value_or(QString("fallback")), QString("text"));

	value.SetNull();
	QVERIFY(value.IsNull());
	QCOMPARE(value.value_or(QString("fallback")), QString("fallback"));

	istd::TNullableValue<QString> uninitialized;
	QCOMPARE(uninitialized.value_or(QString("fallback")), QString("fallback"));
}


void TNullableValueTest::AssignmentOperatorsTest()
{
	istd::TNullableValue<QString> value;
	value = QString("copy");
	QVERIFY(value.HasValue());
	QCOMPARE(value.GetValue(), QString("copy"));

	QString movedText("move");
	value = std::move(movedText);
	QVERIFY(value.HasValue());
	QCOMPARE(value.GetValue(), QString("move"));

	value = nullptr;
	QVERIFY(value.IsNull());
	QVERIFY(!value.HasValue());

	istd::TNullableValue<QString> shared(QString("shared"));
	value = shared;
	QVERIFY(value == shared);
	QCOMPARE(value.GetValue(), QString("shared"));

	shared.GetValue().append("!");
	QCOMPARE(value.GetValue(), QString("shared!"));
}


void TNullableValueTest::BoolAndHasValueTest()
{
	istd::TNullableValue<int> value;
	QVERIFY(!static_cast<bool>(value));
	QVERIFY(!value.has_value());

	value = 9;
	QVERIFY(static_cast<bool>(value));
	QVERIFY(value.has_value());

	value = nullptr;
	QVERIFY(!static_cast<bool>(value));
	QVERIFY(!value.has_value());
}


void TNullableValueTest::ComparisonOperatorsTest()
{
	istd::TNullableValue<QString> shared(QString("same"));
	istd::TNullableValue<QString> alias;
	alias = shared;

	istd::TNullableValue<QString> other(QString("same"));
	istd::TNullableValue<QString> different(QString("different"));
	istd::TNullableValue<QString> empty;

	QVERIFY(shared == alias);
	QVERIFY(shared != other);
	QVERIFY(shared != different);

	QVERIFY(empty == nullptr);
	QVERIFY(shared != nullptr);

	QVERIFY(shared == QString("same"));
	QVERIFY(shared != QString("other"));
}


void TNullableValueTest::SwapAndLessTest()
{
	istd::TNullableValue<QString> left(QString("b"));
	istd::TNullableValue<QString> right(QString("a"));

	left.swap(right);
	QCOMPARE(left.GetValue(), QString("a"));
	QCOMPARE(right.GetValue(), QString("b"));

	QVERIFY(istd::Less(left, right));
	QCOMPARE(qHash(left, 17U), qHash(QString("a"), 17U));
}


void TNullableValueTest::cleanupTestCase()
{
}


I_ADD_TEST(TNullableValueTest);
