#include "iipr/CIterativeProcessorParams.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CIterativeProcessorParams::CIterativeProcessorParams()
:	m_iterationsCount(1)
{
}


int CIterativeProcessorParams::GetIterationsCount() const
{
	return m_iterationsCount;
}


void CIterativeProcessorParams::SetIterationsCount(int iterationsCount)
{
	I_ASSERT(iterationsCount > 0);

	if (m_iterationsCount != iterationsCount){
		istd::CChangeNotifier changeNotifier(this);

		m_iterationsCount = iterationsCount;
	}
}


// reimplemented (iser::ISerializable)

bool CIterativeProcessorParams::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag iterationsCountTag("Iterations", "Number of filter iterations");
	bool retVal = archive.BeginTag(iterationsCountTag);
	retVal = retVal && archive.Process(m_iterationsCount);
	retVal = retVal && archive.EndTag(iterationsCountTag);

	return retVal;
}


} // namespace iipr

