#ifndef iser_CPrimitiveTypesSerializer_included
#define iser_CPrimitiveTypesSerializer_included


// Qt includes
#include <QtCore/QDateTime>
#include <QtCore/QPointF>

// ACF includes
#include "istd/TRange.h"
#include "istd/TRanges.h"
#include "istd/TIndex.h"
#include "iser/IArchive.h"


namespace iser
{


/**
	Implementation of serialize method for some common data types.
*/
class CPrimitiveTypesSerializer
{
public:
	/**
		Serialize a range object.
	*/
	static bool SerializeRange(iser::IArchive& archive, istd::CRange& range);

	/**
		Serialize an integer-based range object.
	*/
	static bool SerializeIntRange(iser::IArchive& archive, istd::CIntRange& range);

	/**
		Serialize list of range objects.
	*/
	static bool SerializeRanges(iser::IArchive& archive, istd::CRanges& ranges);

	/**
		Serialize list of integer-based ranges.
	*/
	static bool SerializeIntRanges(iser::IArchive& archive, istd::CIntRanges& ranges);

	/**
		Serialize QDateTime object.
	*/
	static bool SerializeDateTime(iser::IArchive& archive, QDateTime& dateTime);

	/**
		Serialize a generic index object.
	*/
	template <int Dimensions>
	static bool SerializeIndex(iser::IArchive& archive, istd::TIndex<Dimensions>& index);

	/**
		Serialize a QPointF object.
	*/
	static bool SerializeQPointF(iser::IArchive& archive, QPointF& point);

	/**
		Serialize a QStringList sobject.
	*/
	static bool SerializeQStringList(
				iser::IArchive& archive,
				QStringList& stringList,
				const QByteArray& containerTagName = "Elements",
				const QByteArray& elementTagName = "Element");
};


// public template methods

template <int Dimensions>
bool CPrimitiveTypesSerializer::SerializeIndex(iser::IArchive& archive, istd::TIndex<Dimensions>& index)
{
	bool retVal = true;

	for (int i = 0; i < Dimensions; ++i){
		retVal = retVal && archive.Process(index[i]);
	}

	return retVal;
}


} // namespace iser


#endif // !iser_CPrimitiveTypesSerializer_included

