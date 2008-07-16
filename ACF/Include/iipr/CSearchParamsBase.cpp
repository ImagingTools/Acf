#include "iipr/CSearchParamsBase.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CSearchParamsBase::CSearchParamsBase()
:	m_calibrationPtr(NULL)
{
}


// reimplemented (iipr::ISearchParams)

void CSearchParamsBase::SetCalibrationPtr(const i2d::CTransform* calibrationPtr)
{
	m_calibrationPtr = calibrationPtr;
}


i2d::CRectangle CSearchParamsBase::GetSearchRegion(const i2d::CTransform* calibrationPtr) const
{
	const i2d::CTransform* calibPtr = (calibrationPtr != NULL) ? calibrationPtr : m_calibrationPtr;
	if (calibPtr != NULL){
		return GetCalibrated(*calibPtr, m_searchRegion);
	}

	return m_searchRegion;
}


i2d::CRectangle CSearchParamsBase::GetModelRegion(const i2d::CTransform* calibrationPtr) const
{
	const i2d::CTransform* calibPtr = (calibrationPtr != NULL) ? calibrationPtr : m_calibrationPtr;
	if (calibPtr != NULL){
		return GetCalibrated(*calibPtr, m_modelRegion);
	}

	return m_modelRegion;
}


double CSearchParamsBase::GetMinScore() const
{
	return m_minScore;
}


void CSearchParamsBase::SetMinScore(double minScore)
{
	if (m_minScore != minScore){
		istd::CChangeNotifier changeNotifier(this);

		m_minScore = minScore;
	}
}


const istd::CRange& CSearchParamsBase::GetAngleRange() const
{
	return m_angleRange;
}


void CSearchParamsBase::SetAngleRange(const istd::CRange& angleRange)
{
	if (m_angleRange != angleRange){
		istd::CChangeNotifier changeNotifier(this);

		m_angleRange = angleRange;
	}
}


const istd::CRange& CSearchParamsBase::GetScaleRange() const
{
	return m_scaleRange;
}


void CSearchParamsBase::SetScaleRange(const istd::CRange& scaleRange)
{
	if (m_scaleRange != scaleRange){
		istd::CChangeNotifier changeNotifier(this);

		m_scaleRange = scaleRange;
	}
}


int CSearchParamsBase::GetMatchesCount() const
{
	return m_matchesCount;
}


void CSearchParamsBase::SetMatchesCount(int matchesCount)
{
	if (m_matchesCount != matchesCount){
		istd::CChangeNotifier changeNotifier(this);

		m_matchesCount = matchesCount;
	}
}


// reimplemented (iser::ISerializable)

bool CSearchParamsBase::Serialize(iser::IArchive & archive)
{
	double startAngle = m_angleRange.GetMinValue();
	double endAngle = m_angleRange.GetMaxValue();
	double minScale = m_scaleRange.GetMinValue();
	double maxScale = m_scaleRange.GetMaxValue();

	static iser::CArchiveTag startAngleTag("StartAngle", "Minimum angle");
	bool retVal = archive.BeginTag(startAngleTag);
	retVal = retVal && archive.Process(startAngle);
	retVal = retVal && archive.EndTag(startAngleTag);

	static iser::CArchiveTag endAngleTag("EndAngle", "Maximum angle");
	retVal = retVal && archive.BeginTag(endAngleTag);
	retVal = retVal && archive.Process(endAngle);
	retVal = retVal && archive.EndTag(endAngleTag);

	static iser::CArchiveTag minScoreTag("MinScore", "Minimum match score");
	retVal = retVal && archive.BeginTag(minScoreTag);
	retVal = retVal && archive.Process(m_minScore);
	retVal = retVal && archive.EndTag(minScoreTag);

	static iser::CArchiveTag minScaleTag("MinScale", "Minimum scale");
	retVal = retVal && archive.BeginTag(minScaleTag);
	retVal = retVal && archive.Process(minScale);
	retVal = retVal && archive.EndTag(minScaleTag);

	static iser::CArchiveTag maxScaleTag("MaxScale", "Maximum scale");
	retVal = retVal && archive.BeginTag(maxScaleTag);
	retVal = retVal && archive.Process(maxScale);
	retVal = retVal && archive.EndTag(maxScaleTag);

	static iser::CArchiveTag matchesCountTag("MatchesCount", "Minimum model matches count");
	retVal = retVal && archive.BeginTag(matchesCountTag);
	retVal = retVal && archive.Process(m_matchesCount);
	retVal = retVal && archive.EndTag(matchesCountTag);

	static iser::CArchiveTag timeoutTag("Timeout", "Timout for model search");
	retVal = retVal && archive.BeginTag(timeoutTag);
	retVal = retVal && archive.Process(m_timeout);
	retVal = retVal && archive.EndTag(timeoutTag);

	static iser::CArchiveTag searchRegionTag("SearchRegion", "Region for the model search");
	retVal = retVal && archive.BeginTag(searchRegionTag);
	retVal = retVal && m_searchRegion.Serialize(archive);
	retVal = retVal && archive.EndTag(searchRegionTag);

	static iser::CArchiveTag modelRegionTag("SearchRegion", "Region for the model teaching");
	retVal = retVal && archive.BeginTag(modelRegionTag);
	retVal = retVal && m_modelRegion.Serialize(archive);
	retVal = retVal && archive.EndTag(modelRegionTag);

	if (!archive.IsStoring()){
		m_angleRange = istd::CRange(startAngle, endAngle);
		m_scaleRange = istd::CRange(minScale, maxScale);
	}

	return retVal;
}


// protected static members

i2d::CRectangle CSearchParamsBase::GetCalibrated(const i2d::CTransform&/* calibration*/, const i2d::CRectangle& region)
{
	return region;
}


} // namespace iipr

