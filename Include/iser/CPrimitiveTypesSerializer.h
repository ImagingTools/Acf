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
	Implementation range serializer.

	\ingroup Main
*/
class CPrimitiveTypesSerializer
{
public:
	static bool SerializeRange(iser::IArchive& archive, istd::CRange& range);
	static bool SerializeIntRange(iser::IArchive& archive, istd::CIntRange& range);
	static bool SerializeRanges(iser::IArchive& archive, istd::CRanges& ranges);
	static bool SerializeIntRanges(iser::IArchive& archive, istd::CIntRanges& ranges);
	static bool SerializeDateTime(iser::IArchive& archive, QDateTime& dateTime);

	template <int Dimensions>
	static bool SerializeIndex(iser::IArchive& archive, istd::TIndex<Dimensions>& index);

	static bool SerializeQPointF(iser::IArchive& archive, QPointF& point);
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

