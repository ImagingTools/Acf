// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/Test/CJsonMemoryWriteArchiveTest.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <imod/TModelWrap.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <iprm/CParamsSet.h>
#include <iprm/CTextParam.h>
#include <iprm/CIdParam.h>
#include <iprm/CNameParam.h>
#include <iser/CPrimitiveTypesSerializer.h>


void CJsonMemoryWriteArchiveTest::initTestCase()
{
	m_metaInfo.SetMetaInfo(0, QByteArray("Test ByteArray"));
	m_metaInfo.SetMetaInfo(1, QString("Test String"));
	m_metaInfo.SetMetaInfo(2, int(10));
	m_metaInfo.SetMetaInfo(3, double(10.0));
}


void CJsonMemoryWriteArchiveTest::WriteTest()
{
	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(m_metaInfo.Serialize(writeArchive));
	m_buffer = writeArchive.GetData();
}

void CJsonMemoryWriteArchiveTest::ReadTest()
{
	idoc::CStandardDocumentMetaInfo metaInfo;
	iser::CJsonMemReadArchive readArchive(m_buffer);
	QVERIFY(metaInfo.Serialize(readArchive));
	QVERIFY(m_metaInfo.IsEqual(metaInfo));
}


void CJsonMemoryWriteArchiveTest::ParamsSetSerializeTest()
{
	// create reference params
	imod::TModelWrap<iprm::CParamsSet> params;

	iprm::CNameParam nameParam;
	nameParam.SetName("Sam");
	params.SetEditableParameter("Name", &nameParam);

	iprm::CTextParam textParam;
	textParam.SetText("Qt Test");
	params.SetEditableParameter("Text", &textParam);

	iprm::CIdParam idParam;
	idParam.SetId("{GV5-E4YTGO5IY6T0EG45-45-TG54-G-F}");
	params.SetEditableParameter("ID", &idParam);

	iser::CJsonMemWriteArchive writeArchive;

	// serialize
	const bool written = params.Serialize(writeArchive);
	QVERIFY2(written, "Unable to save params");
	QByteArray buffer = writeArchive.GetData();

	QJsonParseError jsonParserror;
	QJsonObject jsonObject = QJsonDocument::fromJson(buffer, &jsonParserror).object();

	QVERIFY2(jsonParserror.error == QJsonParseError::NoError,
			 QString("Saved JSON is NOT valid. Error: '%1' at '%2'. \n DATA: \n %3").arg(jsonParserror.errorString(), QString::number(jsonParserror.offset), qPrintable(buffer)).toLocal8Bit());

	// create new params
	imod::TModelWrap<iprm::CParamsSet> newParams;
	iprm::CNameParam newNameParam;
	newParams.SetEditableParameter("Name", &newNameParam);
	iprm::CTextParam newTextParam;
	newParams.SetEditableParameter("Text", &newTextParam);
	iprm::CIdParam newIdParam;
	newParams.SetEditableParameter("ID", &newIdParam);

	iser::CJsonMemReadArchive readArchive(buffer);
	// restore serialized data in iew params
	const bool restored = newParams.Serialize(readArchive);
	QVERIFY2(restored, "Unable to load params");

	// compare saved and restored
	bool isEqual = newParams.IsEqual(params);
	QVERIFY2(isEqual, "Restored params is not equal to original");
}


void CJsonMemoryWriteArchiveTest::ObjectContainerSerializeTest()
{
	QList<iprm::CNameParam> listParams;
	for (int index = 0; index < 3; index++){
		iprm::CNameParam nameParam;
		nameParam.SetName(QString("Sam%0").arg(index));
		listParams << nameParam;
	}
	iser::CJsonMemWriteArchive writeArchive;
	// serialize
	const bool written = iser::CPrimitiveTypesSerializer::SerializeObjectContainer(writeArchive, listParams);
	QVERIFY2(written, "Unable to save params");
	QByteArray buffer = writeArchive.GetData();

	QJsonParseError jsonParserror;
	QJsonObject jsonObject = QJsonDocument::fromJson(buffer, &jsonParserror).object();

	QVERIFY2(jsonParserror.error == QJsonParseError::NoError,
			 QString("Saved JSON is NOT valid. Error: '%1' at '%2'. \n DATA: \n %3").arg(jsonParserror.errorString(), QString::number(jsonParserror.offset), qPrintable(buffer)).toLocal8Bit());

	QList<iprm::CNameParam> listReadParams;
	iser::CJsonMemReadArchive readArchive(buffer);

	// restore serialized data in iew params
	const bool restored = iser::CPrimitiveTypesSerializer::SerializeObjectContainer(readArchive, listReadParams);
	QVERIFY2(restored, "Unable to load params");

	// compare saved and restored
	bool isEqual = listParams.count() == listReadParams.count();
	for (int index = 0; index < listParams.count(); index++){
		isEqual = isEqual && listParams[index].IsEqual(listReadParams[index]);
	}
	QVERIFY2(isEqual, "Restored params is not equal to original");
}


void CJsonMemoryWriteArchiveTest::EscapingTest()
{
	static iser::CArchiveTag valueTag("Value", "Serialized value");

	QByteArray value;
	value += "Line 1\n";
	value += '\b';
	value += '\f';
	value += '\t';
	value += "\\\"";
	// Check escaping of generic ASCII control characters too (0x01 / SOH).
	value += char(0x01);
	const QByteArray originalValue = value;

	iser::CJsonMemWriteArchive writeArchive(nullptr, false);
	QVERIFY(writeArchive.BeginTag(valueTag));
	QVERIFY(writeArchive.Process(value));
	QVERIFY(writeArchive.EndTag(valueTag));
	QVERIFY(value == originalValue);

	const QByteArray buffer = writeArchive.GetData();

	QJsonParseError jsonParseError;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(buffer, &jsonParseError);
	QVERIFY2(jsonParseError.error == QJsonParseError::NoError,
			 QString("Saved JSON is NOT valid. Error: '%1' at '%2'. \n DATA: \n %3").arg(jsonParseError.errorString(), QString::number(jsonParseError.offset), qPrintable(buffer)).toLocal8Bit());
	QVERIFY(jsonDocument.isObject());
	QVERIFY(jsonDocument.object().contains(valueTag.GetId()));

	QString restoredValue;
	iser::CJsonMemReadArchive readArchive(buffer, false);
	QVERIFY(readArchive.BeginTag(valueTag));
	QVERIFY(readArchive.Process(restoredValue));
	QVERIFY(readArchive.EndTag(valueTag));
	QVERIFY(restoredValue == originalValue);
}


void CJsonMemoryWriteArchiveTest::InvalidReadStateTest()
{
	iser::CArchiveTag valueTag("Value", "Serialized value");
	iser::CArchiveTag multipleTag("Items", "Serialized list", iser::CArchiveTag::TT_MULTIPLE);

	iser::CJsonMemReadArchive readArchive("{}", false);
	QString restoredValue;
	int count = 0;

	QVERIFY(!readArchive.Process(restoredValue));
	QVERIFY(!readArchive.BeginMultiTag(multipleTag, valueTag, count));
}


void CJsonMemoryWriteArchiveTest::DoTest()
{
	m_buffer.clear();

	Model model;

	model.value = 10;

	{
		iser::CJsonMemWriteArchive writeArchive;
		QVERIFY(model.Serialize(writeArchive));
		m_buffer = writeArchive.GetData();
	}

	model.value = 0;

	iser::CJsonMemReadArchive readArchive(m_buffer);
	QVERIFY(model.Serialize(readArchive));
}


I_ADD_TEST(CJsonMemoryWriteArchiveTest);
