// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iser/CJsonMemReadArchive.h>
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


class CSubstractiveColorModelTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void DoSerializeTest();
	void DoAppendColorantTest();
	void DoCreateModelFromNamesTest();
	void DoAppendModelTest();
	void CheckColorantsTest();
	void RemoveColorantTest();
	void EqualTest();
};


