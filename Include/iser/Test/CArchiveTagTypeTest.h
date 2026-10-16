// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


/**
	Checks how the different archive implementations serialize single primitive values
	placed directly into a tag of type TT_LEAF or TT_UNKNOWN, including special characters.
*/
class CArchiveTagTypeTest: public QObject
{
	Q_OBJECT

public:
	enum ArchiveKind
	{
		AK_JSON,
		AK_XML,
		AK_COMPACT_XML,
		AK_BINARY
	};
	Q_ENUM(ArchiveKind)

	class TextFieldsModel: virtual public iser::ISerializable
	{
	public:
		explicit TextFieldsModel(iser::CArchiveTag::TagType tagType = iser::CArchiveTag::TT_UNKNOWN);

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive) override;

		bool operator==(const TextFieldsModel& other) const;

		QString text;
		QByteArray data;
		int number = 0;

	private:
		iser::CArchiveTag m_textTag;
		iser::CArchiveTag m_dataTag;
		iser::CArchiveTag m_numberTag;
	};

	class NestedModel: virtual public iser::ISerializable
	{
	public:
		struct Point
		{
			int x = 0;
			int y = 0;

			bool operator==(const Point& other) const
			{
				return (x == other.x) && (y == other.y);
			}
		};

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive) override;

		bool operator==(const NestedModel& other) const;

		QString name;
		bool flag = false;
		QStringList items;
		QList<Point> points;
	};

private Q_SLOTS:
	/**
		Prints the output of every archive kind for every tag type, as documentation of the different layouts.
	*/
	void DumpLayoutTest();
	void RoundTripTest_data();
	void RoundTripTest();
	void JsonLayoutTest_data();
	void JsonLayoutTest();
	void XmlLayoutTest_data();
	void XmlLayoutTest();
	void CompactXmlLayoutTest_data();
	void CompactXmlLayoutTest();
	void JsonNestedLayoutTest();
	void JsonLegacyValuesReadTest_data();
	void JsonLegacyValuesReadTest();
	void JsonLegacyNestedReadTest();

private:
	static void AddValueRows(bool includeAllArchives);
	static void AddTagTypeRows();
	static QString GetKnownIssue(ArchiveKind kind, const QByteArray& valueName);
	static void FillNestedModel(NestedModel& model);
	static QByteArray Write(ArchiveKind kind, TextFieldsModel& model, bool serializeHeader = true);
	static bool Read(ArchiveKind kind, const QByteArray& archiveData, TextFieldsModel& model);
};


