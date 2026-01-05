#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <iprm/IOptionsList.h>


class COptionsListTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();

	void FindOptionIndexById_ValidId_Test();
	void FindOptionIndexById_InvalidId_Test();
	void FindOptionIndexById_EmptyId_Test();
	void FindOptionIndexById_EmptyList_Test();
	void FindOptionIndexById_MultipleOptions_Test();

	void cleanupTestCase();
};


