#include "iser/CPrimitiveTypesSerializer.h"


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "iser/CArchiveTag.h"


namespace iser
{


// public static methods

bool CPrimitiveTypesSerializer::SerializeRange(iser::IArchive& archive, istd::CRange& range)
{
	static iser::CArchiveTag minValueTag("MinValue", "Minimal range value");
	static iser::CArchiveTag maxValueTag("MaxValue", "Maximal range value");

	bool retVal = true;

	retVal = retVal && archive.BeginTag(minValueTag);
	retVal = retVal && archive.Process(range.GetMinValueRef());
	retVal = retVal && archive.EndTag(minValueTag);

	retVal = retVal && archive.BeginTag(maxValueTag);
	retVal = retVal && archive.Process(range.GetMaxValueRef());
	retVal = retVal && archive.EndTag(maxValueTag);

	return retVal;
}


bool CPrimitiveTypesSerializer::SerializeIntRange(iser::IArchive& archive, istd::CIntRange& range)
{
	static iser::CArchiveTag minValueTag("MinValue", "Minimal range value");
	static iser::CArchiveTag maxValueTag("MaxValue", "Maximal range value");

	bool retVal = true;

	retVal = retVal && archive.BeginTag(minValueTag);
	retVal = retVal && archive.Process(range.GetMinValueRef());
	retVal = retVal && archive.EndTag(minValueTag);

	retVal = retVal && archive.BeginTag(maxValueTag);
	retVal = retVal && archive.Process(range.GetMaxValueRef());
	retVal = retVal && archive.EndTag(maxValueTag);

	return retVal;
}


bool CPrimitiveTypesSerializer::SerializeRanges(iser::IArchive& archive, istd::CRanges& ranges)
{
	static iser::CArchiveTag beginStateTag("BeginState", "Begin state");
	static iser::CArchiveTag switchPointsTag("SwitchPoints", "List of switch point positions");
	static iser::CArchiveTag positionTag("Position", "Switch point position");

	bool retVal = true;

	bool isStoring = archive.IsStoring();

	istd::CRanges::SwitchPoints& switchPoints = ranges.GetSwitchPointsRef();

	retVal = retVal && archive.BeginTag(beginStateTag);
	if (isStoring){
		bool beginState = ranges.GetBeginState();

		retVal = retVal && archive.Process(beginState);
	}
	else{
		bool beginState = false;

		retVal = retVal && archive.Process(beginState);

		ranges.SetBeginState(beginState);
	}

	retVal = retVal && archive.EndTag(beginStateTag);

	int pointsCount = int(switchPoints.size());

	retVal = retVal && archive.BeginMultiTag(switchPointsTag, positionTag, pointsCount);

	if (!retVal){
		return false;
	}

	if (isStoring){
		for (		istd::CRanges::SwitchPoints::iterator iter = switchPoints.begin();
					iter != switchPoints.end();
					++iter){
			retVal = retVal && archive.BeginTag(positionTag);

			double position = 0;
			retVal = retVal && archive.Process(position);
			switchPoints.insert(position);

			retVal = retVal && archive.EndTag(positionTag);
		}
	}
	else{
		switchPoints.clear();

		for (int i = 0; i < pointsCount; ++i){
			retVal = retVal && archive.BeginTag(positionTag);

			double position = 0;
			retVal = retVal && archive.Process(position);
			switchPoints.insert(position);

			retVal = retVal && archive.EndTag(positionTag);
		}
	}

	retVal = retVal && archive.EndTag(switchPointsTag);

	return retVal;
}


bool CPrimitiveTypesSerializer::SerializeIntRanges(iser::IArchive& archive, istd::CIntRanges& ranges)
{
	static iser::CArchiveTag beginStateTag("BeginState", "Begin state");
	static iser::CArchiveTag switchPointsTag("SwitchPoints", "List of switch point positions");
	static iser::CArchiveTag positionTag("Position", "Switch point position");

	bool retVal = true;

	bool isStoring = archive.IsStoring();

	istd::CIntRanges::SwitchPoints& switchPoints = ranges.GetSwitchPointsRef();

	retVal = retVal && archive.BeginTag(beginStateTag);
	if (isStoring){
		bool beginState = ranges.GetBeginState();

		retVal = retVal && archive.Process(beginState);
	}
	else{
		bool beginState = false;

		retVal = retVal && archive.Process(beginState);

		ranges.SetBeginState(beginState);
	}

	retVal = retVal && archive.EndTag(beginStateTag);

	int pointsCount = int(switchPoints.size());

	retVal = retVal && archive.BeginMultiTag(switchPointsTag, positionTag, pointsCount);

	if (!retVal){
		return false;
	}

	if (isStoring){
		for (		istd::CIntRanges::SwitchPoints::iterator iter = switchPoints.begin();
					iter != switchPoints.end();
					++iter){
			retVal = retVal && archive.BeginTag(positionTag);

			int position = 0;
			retVal = retVal && archive.Process(position);
			switchPoints.insert(position);

			retVal = retVal && archive.EndTag(positionTag);
		}
	}
	else{
		switchPoints.clear();

		for (int i = 0; i < pointsCount; ++i){
			retVal = retVal && archive.BeginTag(positionTag);

			int position = 0;
			retVal = retVal && archive.Process(position);
			switchPoints.insert(position);

			retVal = retVal && archive.EndTag(positionTag);
		}
	}

	retVal = retVal && archive.EndTag(switchPointsTag);

	return retVal;
}


bool CPrimitiveTypesSerializer::SerializeDateTime(iser::IArchive& archive, QDateTime& dateTime)
{
	const QString TimeFormat("yyyy-MM-dd hh-mm-ss zzz");

	if (archive.IsStoring()){
		QString dateTimeString = dateTime.toString(TimeFormat);

		return archive.Process(dateTimeString);
	}
	else{
		QString dateTimeString;

		if (archive.Process(dateTimeString)){
			dateTime = QDateTime::fromString(dateTimeString, TimeFormat);
		}
	}

	return false;
}


} // namespace iser


