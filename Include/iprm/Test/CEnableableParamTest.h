#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <iprm/CEnableableParam.h>


class CEnableableParamTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();

	void DefaultConstructorTest();
	void ConstructorWithEnabledTest();
	void ConstructorWithDisabledTest();
	void IsEnabledTest();
	void SetEnabledTest();
	void SetDisabledTest();
	void SetSameStateTest();
	void IsEnablingAllowedTest();
	void SerializeEnabledTest();
	void SerializeDisabledTest();
	void CopyFromTest();
	void CopyFromIncompatibleTest();
	void CloneTest();
	void CloneWithModeTest();

	void cleanupTestCase();
};


