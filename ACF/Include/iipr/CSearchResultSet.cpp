#include "iipr/CSearchResultSet.h"


namespace iipr
{


CSearchResultSet::CSearchResultSet()
{
	Reset();
}


void CSearchResultSet::AddPosition(const i2d::CVector2d& position)
{
	m_positions.push_back(position);
}



void CSearchResultSet::AddScale(const i2d::CVector2d& scale )
{
	m_scales.push_back(scale);
}


void CSearchResultSet::AddAngle(double angle)
{
	m_angles.push_back(angle);
}


void CSearchResultSet::AddScore(double score)
{
	m_scores.push_back(score);
}


void CSearchResultSet::SetMatchesCount(int matchesCount)
{
	m_matchesCount = matchesCount;
}


void CSearchResultSet::SetTime(double time)
{
	m_time = time;
}


// reimplemented (iipr::ISearchResultSets)

int CSearchResultSet::GetMatchesCount() const
{
	return m_matchesCount;
}


double CSearchResultSet::GetTime() const
{
	return m_time;
}


i2d::CTransform CSearchResultSet::GetTransform(int index) const
{
	i2d::CTransform retVal(GetPosition(index), GetScale(index), GetAngle(index));

	return retVal;
}


i2d::CVector2d CSearchResultSet::GetScale(int index) const
{
	I_ASSERT(index >= 0 && index < (int)m_scales.size());

	return m_scales.at(index);
}


double CSearchResultSet::GetAngle(int index) const
{
	I_ASSERT(index >= 0 && index < (int)m_angles.size());

	return m_angles.at(index);
}


i2d::CVector2d CSearchResultSet::GetPosition(int index) const
{
	I_ASSERT(index >= 0 && index < (int)m_positions.size());

	return m_positions.at(index);
}


double CSearchResultSet::GetScore(int index) const
{
	I_ASSERT(index >= 0 && index < (int)m_scores.size());

	return m_scores.at(index);
}


void CSearchResultSet::Reset()
{
	m_matchesCount = 0;
	m_time = 0.0;
	m_scales.clear();
	m_angles.clear();
	m_positions.clear();
	m_scores.clear();
}


} // namespace iipr

