#ifndef iipr_CSearchResultSet_included
#define iipr_CSearchResultSet_included


#include <vector>


#include "iipr/ISearchResultSet.h"


namespace iipr
{


class CSearchResultSet: virtual public iipr::ISearchResultSet
{
public:
	CSearchResultSet();

	void AddPosition(const i2d::CVector2d& position);
	void AddScale(const i2d::CVector2d& scale );
	void AddAngle(double angle);
	void AddScore(double score);
	void SetMatchesCount(int matchesCount);
	void SetTime(double time);

	// reimplemented (iipr::ISearchResultSets)
	virtual int GetMatchesCount() const;
	virtual double GetTime() const;
	virtual i2d::CTransform GetTransform(int index) const;
	virtual i2d::CVector2d GetScale(int index) const;
	virtual double GetAngle(int index) const;
	virtual i2d::CVector2d GetPosition(int index) const;
	virtual double GetScore(int index) const;
	virtual void Reset();

protected:
	int m_matchesCount;
	double m_time;
	std::vector<i2d::CVector2d> m_scales;
	std::vector<double> m_angles;
	std::vector<i2d::CVector2d> m_positions;
	std::vector<double> m_scores;
};


} // namespace iipr


#endif //!iipr_CSearchResultSet_included

