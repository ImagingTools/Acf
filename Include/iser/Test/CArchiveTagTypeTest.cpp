// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/Test/CArchiveTagTypeTest.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtXml/QDomDocument>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <iser/CJsonMemWriteArchive.h>
#include <iser/CJsonMemReadArchive.h>
#include <iser/CXmlStringWriteArchive.h>
#include <iser/CXmlStringReadArchive.h>
#include <iser/CCompactXmlMemWriteArchive.h>
#include <iser/CCompactXmlMemReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <iser/CMemoryReadArchive.h>


Q_DECLARE_METATYPE(iser::CArchiveTag::TagType)


namespace
{


// Text used by the legacy JSON fixtures below: "Line<LF>"quoted" \ <&> Grüße".
const QString s_legacyText = QString::fromUtf8("Line\n\"quoted\" \\ <&> Gr\xC3\xBC\xC3\x9F" "e");


} // namespace


// public methods of embedded class TextFieldsModel

CArchiveTagTypeTest::TextFieldsModel::TextFieldsModel(iser::CArchiveTag::TagType tagType)
	:m_textTag("Text", "String value", tagType),
	m_dataTag("Data", "Byte array value", tagType),
	m_numberTag("Number", "Integer value", tagType)
{
}


// reimplemented (iser::ISerializable)

bool CArchiveTagTypeTest::TextFieldsModel::Serialize(iser::IArchive& archive)
{
	bool retVal = archive.BeginTag(m_textTag);
	retVal = retVal && archive.Process(text);
	retVal = retVal && archive.EndTag(m_textTag);

	retVal = retVal && archive.BeginTag(m_dataTag);
	retVal = retVal && archive.Process(data);
	retVal = retVal && archive.EndTag(m_dataTag);

	retVal = retVal && archive.BeginTag(m_numberTag);
	retVal = retVal && archive.Process(number);
	retVal = retVal && archive.EndTag(m_numberTag);

	return retVal;
}


bool CArchiveTagTypeTest::TextFieldsModel::operator==(const TextFieldsModel& other) const
{
	return (text == other.text) && (data == other.data) && (number == other.number);
}


// public methods of embedded class NestedModel

// reimplemented (iser::ISerializable)

bool CArchiveTagTypeTest::NestedModel::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag groupTag("Group", "Group of values", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag nameTag("Name", "Name", iser::CArchiveTag::TT_UNKNOWN, &groupTag);
	static iser::CArchiveTag flagTag("Flag", "Flag", iser::CArchiveTag::TT_LEAF, &groupTag);
	static iser::CArchiveTag emptyTag("Empty", "Tag without content", iser::CArchiveTag::TT_UNKNOWN, &groupTag);
	static iser::CArchiveTag itemsTag("Items", "List of strings", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag itemTag("Item", "Single string", iser::CArchiveTag::TT_UNKNOWN, &itemsTag);
	static iser::CArchiveTag pointsTag("Points", "List of points", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag pointTag("Point", "Single point", iser::CArchiveTag::TT_GROUP, &pointsTag);
	static iser::CArchiveTag xTag("X", "X coordinate", iser::CArchiveTag::TT_LEAF, &pointTag);
	static iser::CArchiveTag yTag("Y", "Y coordinate", iser::CArchiveTag::TT_UNKNOWN, &pointTag);

	bool retVal = archive.BeginTag(groupTag);
	retVal = retVal && archive.BeginTag(nameTag);
	retVal = retVal && archive.Process(name);
	retVal = retVal && archive.EndTag(nameTag);
	retVal = retVal && archive.BeginTag(flagTag);
	retVal = retVal && archive.Process(flag);
	retVal = retVal && archive.EndTag(flagTag);
	retVal = retVal && archive.BeginTag(emptyTag);
	retVal = retVal && archive.EndTag(emptyTag);
	retVal = retVal && archive.EndTag(groupTag);

	int itemsCount = items.count();
	retVal = retVal && archive.BeginMultiTag(itemsTag, itemTag, itemsCount);
	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		items.resize(itemsCount);
	}

	for (QString& item : items){
		retVal = retVal && archive.BeginTag(itemTag);
		retVal = retVal && archive.Process(item);
		retVal = retVal && archive.EndTag(itemTag);
	}
	retVal = retVal && archive.EndTag(itemsTag);

	int pointsCount = points.count();
	retVal = retVal && archive.BeginMultiTag(pointsTag, pointTag, pointsCount);
	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		points.resize(pointsCount);
	}

	for (Point& point : points){
		retVal = retVal && archive.BeginTag(pointTag);
		retVal = retVal && archive.BeginTag(xTag);
		retVal = retVal && archive.Process(point.x);
		retVal = retVal && archive.EndTag(xTag);
		retVal = retVal && archive.BeginTag(yTag);
		retVal = retVal && archive.Process(point.y);
		retVal = retVal && archive.EndTag(yTag);
		retVal = retVal && archive.EndTag(pointTag);
	}
	retVal = retVal && archive.EndTag(pointsTag);

	return retVal;
}


bool CArchiveTagTypeTest::NestedModel::operator==(const NestedModel& other) const
{
	return (name == other.name) && (flag == other.flag) && (items == other.items) && (points == other.points);
}


// private slots

void CArchiveTagTypeTest::DumpLayoutTest()
{
	const QList<QPair<const char*, ArchiveKind>> archives = {
		{"JSON", AK_JSON},
		{"XML", AK_XML},
		{"Compact XML", AK_COMPACT_XML}
	};

	const QList<QPair<const char*, iser::CArchiveTag::TagType>> tagTypes = {
		{"TT_LEAF", iser::CArchiveTag::TT_LEAF},
		{"TT_UNKNOWN", iser::CArchiveTag::TT_UNKNOWN},
		{"TT_GROUP", iser::CArchiveTag::TT_GROUP}
	};

	for (const auto& archive : archives){
		for (const auto& tagType : tagTypes){
			TextFieldsModel model(tagType.second);
			model.text = "Hello";
			model.data = "World";
			model.number = 42;

			const QByteArray archiveData = Write(archive.second, model, false);
			QVERIFY(!archiveData.isEmpty());

			qInfo().noquote() << QString("%1 / %2:\n%3").arg(archive.first, tagType.first, QString::fromUtf8(archiveData).trimmed());
		}
	}
}


void CArchiveTagTypeTest::RoundTripTest_data()
{
	AddValueRows(true);
}


void CArchiveTagTypeTest::RoundTripTest()
{
	QFETCH(ArchiveKind, archiveKind);
	QFETCH(iser::CArchiveTag::TagType, tagType);
	QFETCH(QString, value);
	QFETCH(QString, knownIssue);

	TextFieldsModel writeModel(tagType);
	writeModel.text = value;
	writeModel.data = value.toUtf8();
	writeModel.number = 42;

	const QByteArray archiveData = Write(archiveKind, writeModel);
	QVERIFY(!archiveData.isEmpty());

	TextFieldsModel readModel(tagType);
	QVERIFY2(Read(archiveKind, archiveData, readModel), archiveData.constData());

	if (!knownIssue.isEmpty()){
		QEXPECT_FAIL("", qPrintable(knownIssue), Continue);
	}

	QVERIFY2(
				readModel == writeModel,
				qPrintable(QString("Text: '%1', data: '%2'\n%3").arg(readModel.text, QString::fromUtf8(readModel.data), QString::fromUtf8(archiveData))));
}


void CArchiveTagTypeTest::JsonLayoutTest_data()
{
	AddValueRows(false);
}


void CArchiveTagTypeTest::JsonLayoutTest()
{
	QFETCH(iser::CArchiveTag::TagType, tagType);
	QFETCH(QString, value);

	TextFieldsModel writeModel(tagType);
	writeModel.text = value;
	writeModel.data = value.toUtf8();
	writeModel.number = 42;

	const QByteArray archiveData = Write(AK_JSON, writeModel);

	QJsonParseError parseError;
	const QJsonDocument document = QJsonDocument::fromJson(archiveData, &parseError);
	QVERIFY2(parseError.error == QJsonParseError::NoError, qPrintable(parseError.errorString() + "\n" + archiveData));
	QVERIFY(document.isObject());

	// A single primitive value inside a tag must be stored as a plain JSON value, not wrapped into an object with the same key.
	const QJsonObject rootObject = document.object();
	QVERIFY2(rootObject.value("Text").isString(), archiveData.constData());
	QCOMPARE(rootObject.value("Text").toString(), value);
	QVERIFY2(rootObject.value("Data").isString(), archiveData.constData());
	QCOMPARE(rootObject.value("Data").toString(), value);
	QVERIFY2(rootObject.value("Number").isDouble(), archiveData.constData());
	QCOMPARE(rootObject.value("Number").toInt(), 42);
}


void CArchiveTagTypeTest::XmlLayoutTest_data()
{
	AddTagTypeRows();
}


void CArchiveTagTypeTest::XmlLayoutTest()
{
	QFETCH(iser::CArchiveTag::TagType, tagType);

	TextFieldsModel model(tagType);
	model.text = "Hello";
	model.data = "World";
	model.number = 42;

	const QByteArray archiveData = Write(AK_XML, model, false);

	QDomDocument document;
	QVERIFY2(document.setContent(archiveData), archiveData.constData());

	// Every tag type is written as a child element containing the value as text.
	const QDomElement rootElement = document.documentElement();
	QCOMPARE(rootElement.attributes().count(), 0);
	QCOMPARE(rootElement.firstChildElement("Text").text().trimmed(), QString("Hello"));
	QCOMPARE(rootElement.firstChildElement("Data").text().trimmed(), QString("World"));
	QCOMPARE(rootElement.firstChildElement("Number").text().trimmed(), QString("42"));
}


void CArchiveTagTypeTest::CompactXmlLayoutTest_data()
{
	AddTagTypeRows();
}


void CArchiveTagTypeTest::CompactXmlLayoutTest()
{
	QFETCH(iser::CArchiveTag::TagType, tagType);

	TextFieldsModel model(tagType);
	model.text = "Hello";
	model.data = "World";
	model.number = 42;

	const QByteArray archiveData = Write(AK_COMPACT_XML, model, false);

	QDomDocument document;
	QVERIFY2(document.setContent(archiveData), archiveData.constData());

	const QDomElement rootElement = document.documentElement();

	if (tagType == iser::CArchiveTag::TT_LEAF){
		// Leaf tags are written as attributes of the parent element.
		QVERIFY2(rootElement.firstChildElement().isNull(), archiveData.constData());
		QCOMPARE(rootElement.attribute("Text"), QString("Hello"));
		QCOMPARE(rootElement.attribute("Data"), QString("World"));
		QCOMPARE(rootElement.attribute("Number"), QString("42"));
	}
	else{
		// Other tags are written as child elements containing the value as text.
		QCOMPARE(rootElement.attributes().count(), 0);
		QCOMPARE(rootElement.firstChildElement("Text").text(), QString("Hello"));
		QCOMPARE(rootElement.firstChildElement("Data").text(), QString("World"));
		QCOMPARE(rootElement.firstChildElement("Number").text(), QString("42"));
	}
}


void CArchiveTagTypeTest::JsonNestedLayoutTest()
{
	NestedModel writeModel;
	FillNestedModel(writeModel);

	iser::CJsonMemWriteArchive writeArchive(nullptr, false);
	QVERIFY(writeModel.Serialize(writeArchive));
	const QByteArray archiveData = writeArchive.GetData();

	QJsonParseError parseError;
	const QJsonDocument document = QJsonDocument::fromJson(archiveData, &parseError);
	QVERIFY2(parseError.error == QJsonParseError::NoError, qPrintable(parseError.errorString() + "\n" + archiveData));

	const QJsonObject expectedObject = QJsonDocument::fromJson(R"({
		"Group": {"Name": "Group \"name\"", "Flag": true, "Empty": {}},
		"Items": ["first", "", "third, with comma"],
		"Points": [{"X": 1, "Y": 2}, {"X": -3, "Y": 4}]
	})").object();
	QVERIFY(!expectedObject.isEmpty());
	QVERIFY2(document.object() == expectedObject, archiveData.constData());

	NestedModel readModel;
	iser::CJsonMemReadArchive readArchive(archiveData, false);
	QVERIFY(readModel.Serialize(readArchive));
	QVERIFY(readModel == writeModel);
}


void CArchiveTagTypeTest::JsonLegacyValuesReadTest_data()
{
	QTest::addColumn<QByteArray>("archiveData");
	QTest::addColumn<bool>("serializeHeader");

	// Written by the JSON archive before single values of non-leaf tags were stored directly.
	QTest::newRow("with header") << QByteArray(
				R"({"AcfHeader":{"VersionInfos":[]},)"
				R"("Text":{"Text": "Line\n\"quoted\" \\ <&> Gr)" "\xC3\xBC\xC3\x9F" R"(e"},)"
				R"("Data":{"Data": "Line\n\"quoted\" \\ <&> Gr)" "\xC3\xBC\xC3\x9F" R"(e"},)"
				R"("Number":{"Number": 42}})") << true;
	QTest::newRow("without header") << QByteArray(
				R"({"Text":{"Text": "Line\n\"quoted\" \\ <&> Gr)" "\xC3\xBC\xC3\x9F" R"(e"},)"
				R"("Data":{"Data": "Line\n\"quoted\" \\ <&> Gr)" "\xC3\xBC\xC3\x9F" R"(e"},)"
				R"("Number":{"Number": 42}})") << false;
}


void CArchiveTagTypeTest::JsonLegacyValuesReadTest()
{
	QFETCH(QByteArray, archiveData);
	QFETCH(bool, serializeHeader);

	TextFieldsModel readModel(iser::CArchiveTag::TT_UNKNOWN);
	iser::CJsonMemReadArchive readArchive(archiveData, serializeHeader);
	QVERIFY(readModel.Serialize(readArchive));
	QCOMPARE(readModel.text, s_legacyText);
	QCOMPARE(readModel.data, s_legacyText.toUtf8());
	QCOMPARE(readModel.number, 42);
}


void CArchiveTagTypeTest::JsonLegacyNestedReadTest()
{
	// Written by the JSON archive before single values of non-leaf tags were stored directly.
	const QByteArray archiveData(
				R"({"Group":{"Name":{"Name": "Group \"name\""},"Flag":true,"Empty":{}},)"
				R"("Items":[{"Item": "first"},{"Item": ""},{"Item": "third, with comma"}],)"
				R"("Points":[{"X":1,"Y":{"Y": 2}},{"X":-3,"Y":{"Y": 4}}]})");

	NestedModel expectedModel;
	FillNestedModel(expectedModel);

	NestedModel readModel;
	iser::CJsonMemReadArchive readArchive(archiveData, false);
	QVERIFY(readModel.Serialize(readArchive));
	QVERIFY(readModel == expectedModel);
}


// private static methods

void CArchiveTagTypeTest::AddValueRows(bool includeAllArchives)
{
	QTest::addColumn<ArchiveKind>("archiveKind");
	QTest::addColumn<iser::CArchiveTag::TagType>("tagType");
	QTest::addColumn<QString>("value");
	QTest::addColumn<QString>("knownIssue");

	const QList<QPair<const char*, QString>> values = {
		{"plain", "Hello"},
		{"empty", ""},
		{"quotes and backslashes", R"(He said "hi" \ C:\path\)"},
		{"control characters", QString("a\nb\rc\td\be\ff") + QChar(0x01) + QChar(0x1F)},
		{"markup characters", "<tag attr='x'>&amp; ]]> </tag>"},
		{"json-like", R"({"Text": ["fake"]})"},
		{"surrounding spaces", "  padded  "},
		{"unicode", QString::fromUtf8("Gr\xC3\xBC\xC3\x9F" "e \xE6\x97\xA5\xE6\x9C\xAC \xF0\x9F\x98\x80")}
	};

	const QList<QPair<const char*, ArchiveKind>> archives = {
		{"json", AK_JSON},
		{"xml", AK_XML},
		{"compactxml", AK_COMPACT_XML},
		{"binary", AK_BINARY}
	};

	const QList<QPair<const char*, iser::CArchiveTag::TagType>> tagTypes = {
		{"leaf", iser::CArchiveTag::TT_LEAF},
		{"unknown", iser::CArchiveTag::TT_UNKNOWN},
		{"group", iser::CArchiveTag::TT_GROUP}
	};

	for (const auto& archive : archives){
		if (!includeAllArchives && (archive.second != AK_JSON)){
			continue;
		}

		for (const auto& tagType : tagTypes){
			for (const auto& value : values){
				QTest::addRow("%s/%s/%s", archive.first, tagType.first, value.first)
							<< archive.second << tagType.second << value.second << GetKnownIssue(archive.second, value.first);
			}
		}
	}
}


void CArchiveTagTypeTest::AddTagTypeRows()
{
	QTest::addColumn<iser::CArchiveTag::TagType>("tagType");

	QTest::newRow("leaf") << iser::CArchiveTag::TT_LEAF;
	QTest::newRow("unknown") << iser::CArchiveTag::TT_UNKNOWN;
	QTest::newRow("group") << iser::CArchiveTag::TT_GROUP;
}


QString CArchiveTagTypeTest::GetKnownIssue(ArchiveKind kind, const QByteArray& valueName)
{
	if (kind == AK_XML){
		if (valueName == "markup characters"){
			return "XML archive reads escaped '<' and '>' back as '&lt;' and '&gt;'";
		}

		if (valueName == "surrounding spaces"){
			return "XML archive trims leading and trailing spaces";
		}
	}

	if (kind == AK_COMPACT_XML){
		if (valueName == "control characters"){
			return "Compact XML archive turns CR into LF and drops other control characters";
		}

		if (valueName == "unicode"){
			return "Compact XML archive reads QByteArray as Latin-1 instead of UTF-8";
		}
	}

	return QString();
}


void CArchiveTagTypeTest::FillNestedModel(NestedModel& model)
{
	model.name = "Group \"name\"";
	model.flag = true;
	model.items = {"first", "", "third, with comma"};
	model.points = {{1, 2}, {-3, 4}};
}


QByteArray CArchiveTagTypeTest::Write(ArchiveKind kind, TextFieldsModel& model, bool serializeHeader)
{
	switch (kind){
	case AK_JSON:{
			iser::CJsonMemWriteArchive archive(nullptr, serializeHeader);
			return model.Serialize(archive) ? archive.GetData() : QByteArray();
		}

	case AK_XML:{
			iser::CXmlStringWriteArchive archive(nullptr, serializeHeader);
			return model.Serialize(archive) ? archive.GetString() : QByteArray();
		}

	case AK_COMPACT_XML:{
			iser::CCompactXmlMemWriteArchive archive(nullptr, serializeHeader);
			return model.Serialize(archive) ? archive.GetString() : QByteArray();
		}

	case AK_BINARY:{
			iser::CMemoryWriteArchive archive(nullptr, serializeHeader);
			if (!model.Serialize(archive)){
				return QByteArray();
			}

			return QByteArray(static_cast<const char*>(archive.GetBuffer()), archive.GetBufferSize());
		}
	}

	return QByteArray();
}


bool CArchiveTagTypeTest::Read(ArchiveKind kind, const QByteArray& archiveData, TextFieldsModel& model)
{
	switch (kind){
	case AK_JSON:{
			iser::CJsonMemReadArchive archive(archiveData);
			return model.Serialize(archive);
		}

	case AK_XML:{
			iser::CXmlStringReadArchive archive(archiveData);
			return model.Serialize(archive);
		}

	case AK_COMPACT_XML:{
			iser::CCompactXmlMemReadArchive archive(archiveData);
			return model.Serialize(archive);
		}

	case AK_BINARY:{
			iser::CMemoryReadArchive archive(archiveData.constData(), archiveData.size());
			return model.Serialize(archive);
		}
	}

	return false;
}


I_ADD_TEST(CArchiveTagTypeTest);


