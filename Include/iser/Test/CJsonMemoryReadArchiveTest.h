#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iser/CJsonMemReadArchive.h>
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


class CJsonMemoryReadArchiveTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void DoTest();

private:
	class Model: virtual public iser::ISerializable
	{
	public:
		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag typeTag("Type", "Type of the meta information");
			bool retVal = archive.BeginTag(typeTag);
			retVal = retVal && archive.Process(value);
			retVal = retVal && archive.EndTag(typeTag);

			return retVal;
		}

		int value = 42;
	};
};


