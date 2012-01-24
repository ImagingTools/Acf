#ifndef i2d_TMultiPolyline_included
#define i2d_TMultiPolyline_included


// STL includes
#include <algorithm>
#include <string>


// ACF includes
#include "iser/CArchiveTag.h"

#include "i2d/CMultiPolylineBase.h"


namespace i2d
{


template<class SegmentData>
class TMultiPolyline: public CMultiPolylineBase
{
public:
	typedef CPolyline BaseClass;

	TMultiPolyline();

	// reimplemented (i2d::CMultiPolylineBase)
	virtual void SetParallelsCount(int count);
	virtual int GetParallelsCount() const;
	virtual void SetParallel(int segmentIndex, int parallelIndex, double width);
	virtual double GetParallel(int segmentIndex, int parallelIndex) const;
	virtual double GetMaxParallel() const;
	virtual double GetMinParallel() const;
	virtual void SetSegmentData(int segmentIndex, const SegmentData& data);
	virtual const iser::ISerializable& GetSegmentData(int segmentIndex) const;
	virtual bool RemoveSegment(int node, int segment);
	virtual void SelectAllSegments(bool selected = true);
	virtual void SelectSegment(int segmentIndex, bool selected = true);
	virtual bool IsSegmentSelected(int segmentIndex) const;

	// reimplemented (i2d::CPolygon)
	virtual bool InsertNode(const i2d::CVector2d& node);
	virtual bool InsertNode(int index, const i2d::CVector2d& node);
	virtual void Clear();

	// reimplemented (i2d::CPolyline)
	virtual void SetClosed(bool state = true);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	class CSegment
	{
	public:
		bool Serialize(iser::IArchive& archive){
			bool retVal = archive.BeginTag(segmentTag);
			m_data.Serialize(archive);

			for (int i = 0; i < int(m_widthList.size()); i++){
				retVal = retVal && archive.Process(m_widthList[i]);
			}

			if (!archive.IsStoring()){
				m_selected = false;
			}

			retVal = retVal && archive.EndTag(segmentTag);

			return retVal;
		}

		CSegment operator + (const CSegment& x) const
		{
			CSegment result(*this);
			I_ASSERT(result.m_widthList.size() == x.m_widthList.size());

			for (int i = 0 ; i < int(result.m_widthList.size()); i++){
				result.m_widthList[i] += x.m_widthList[i];
			}
	//			result.m_data += x.m_data;

			return result;
		}
		CSegment operator /(double x) const
		{
			CSegment result(*this);

			result /= x;
			return result;
		}
		CSegment& operator /=(double x)
		{
			for (int i = 0 ; i < int(this->m_widthList.size()); i++){
				this->m_widthList[i] /= x;
			}
	//			this->m_data /= x;
			return *this;
		}
	};

	CSegment CalculateAverage(const CSegment& first, const CSegment& second) const;

	public:
		::std::vector<double> m_widthList;
		bool m_selected;
		SegmentData m_data;

private:
	::std::vector<CSegment> m_segmentList;
};


template<class SegmentData>
inline TMultiPolyline<SegmentData>::TMultiPolyline()
{

}


template<class SegmentData>
inline typename TMultiPolyline<SegmentData>::CSegment TMultiPolyline<SegmentData>::CalculateAverage(const CSegment& first, const CSegment& second) const
{
	return (first + second) / 2.0;
}


template<class SegmentData>
inline void TMultiPolyline<SegmentData>::SetClosed(bool state)
{
	if (IsClosed() == state){
		return;
	}

	if (state){
		CSegment segment = (m_segmentList.front() + m_segmentList.back()) / 2.0;
		segment.m_selected = true;
		m_segmentList.push_back(segment);
	}
	else{
		m_segmentList.pop_back();
	}

	BaseClass::SetClosed(state);
}


template<class SegmentData>
inline bool TMultiPolyline<SegmentData>::InsertNode(const i2d::CVector2d& node)
{
	int nodesCount = GetNodesCount();
	if (nodesCount >= 1){
		if (nodesCount == 1){
			CSegment segment;
			segment.m_selected = true;
			m_segmentList.push_back(segment);
			if (IsClosed()){
				m_segmentList.push_back(segment);
			}
		}
		else{
			m_segmentList.push_back(m_segmentList.back());
			m_segmentList.back().m_selected = true;
		}

	}

	return BaseClass::InsertNode(node);
}


template<class SegmentData>
inline bool TMultiPolyline<SegmentData>::InsertNode(int index, const i2d::CVector2d& node)
{
	int nodesCount = GetNodesCount();
	if (nodesCount >= 1){
		CSegment emptySegment;
		emptySegment.m_selected = true;
		if (nodesCount == 1){
			m_segmentList.push_back(emptySegment);
			if (IsClosed()){
				m_segmentList.push_back(emptySegment);
			}
		}
		else{
			CSegment& segmentRef = emptySegment;
			if (index == 0){
				if (IsClosed()){
					segmentRef = m_segmentList.back();
				}
				else{
					segmentRef = m_segmentList.front();
				}
			}
			else{
				if (index >= nodesCount){
					segmentRef = m_segmentList.back();
				}
				else{
					segmentRef = m_segmentList[index - 1];
				}
			}

			::std::vector<CSegment>::iterator iter = m_segmentList.begin();
			iter += index;

			::std::vector<CSegment>::iterator curr = m_segmentList.insert(iter, segmentRef);
			(*curr).m_selected = true;
		}
	}

	return BaseClass::InsertNode(index, node);
}


template<class SegmentData>
inline void TMultiPolyline<SegmentData>::Clear()
{
	m_segmentList.clear();
	BaseClass::Clear();
}


template<class SegmentData>
inline bool TMultiPolyline<SegmentData>::RemoveSegment(int node, int segment)
{
	I_ASSERT(node >= 0);
	I_ASSERT(segment >= 0 && segment < int(m_segmentList.size()));

	node %= GetNodesCount();
	::std::vector<CSegment>::iterator iter = m_segmentList.begin();
	iter += segment;
	m_segmentList.erase(iter);

	return RemoveNode(node);
}


template<class SegmentData>
void TMultiPolyline<SegmentData>::SetParallelsCount(int count)
{
	I_ASSERT(count > 0);

	for (int segmentIndex = 0; segmentIndex < int(m_segmentList.size()); segmentIndex++){
		m_segmentList[segmentIndex].m_widthList.resize(count);
	}
}



template<class SegmentData>
int TMultiPolyline<SegmentData>::GetParallelsCount() const
{
	if (m_segmentList.size() == 0)
		return 0;

	return m_segmentList[0].m_widthList.size();
}


template<class SegmentData>
void TMultiPolyline<SegmentData>::SetParallel(int segmentIndex, int parallelIndex, double width)
{
	I_ASSERT(segmentIndex >= 0 && segmentIndex < int(m_segmentList.size()));
	I_ASSERT(parallelIndex >= 0 && parallelIndex < GetParallelsCount());

	m_segmentList[segmentIndex].m_widthList[parallelIndex] = width;
}


template<class SegmentData>
double TMultiPolyline<SegmentData>::GetParallel(int segmentIndex, int parallelIndex) const
{
	I_ASSERT(segmentIndex >= 0 && segmentIndex < int(m_segmentList.size()));
	I_ASSERT(parallelIndex >= 0 && parallelIndex < GetParallelsCount());

	return m_segmentList[segmentIndex].m_widthList[parallelIndex];
}


template<class SegmentData>
void TMultiPolyline<SegmentData>::SetSegmentData(int segmentIndex, const SegmentData& data)
{
	I_ASSERT(segmentIndex >= 0 && segmentIndex < int(m_segmentList.size()));

	m_segmentList[segmentIndex].m_data = data;
}


template<class SegmentData>
const iser::ISerializable& TMultiPolyline<SegmentData>::GetSegmentData(int segmentIndex) const
{
	I_ASSERT(segmentIndex >= 0 && segmentIndex < int(m_segmentList.size()));

	return m_segmentList[segmentIndex].m_data;
}


template<class SegmentData>
void TMultiPolyline<SegmentData>::SelectAllSegments(bool selected)
{
	for (int i = 0; i < int(m_segmentList.size()); i++){
		m_segmentList[i].m_selected = selected;
	}
}


template<class SegmentData>
void TMultiPolyline<SegmentData>::SelectSegment(int segmentIndex, bool selected)
{
	I_ASSERT(segmentIndex >= 0 && segmentIndex < int(m_segmentList.size()));

	m_segmentList[segmentIndex].m_selected = selected;
}


template<class SegmentData>
bool TMultiPolyline<SegmentData>::IsSegmentSelected(int segmentIndex) const
{
	I_ASSERT(segmentIndex >= 0 && segmentIndex < int(m_segmentList.size()));

	return m_segmentList[segmentIndex].m_selected;
}


template<class SegmentData>
double TMultiPolyline<SegmentData>::GetMaxParallel() const
{
	double maxValue = -_qel_HUGE_NUMBER;

	if (GetParallelsCount() > 0){
		::std::vector<double>::const_iterator max_it;

		for (int segmentIndex = 0; segmentIndex < int(m_segmentList.size()); segmentIndex++){
			max_it = ::std::max_element(m_segmentList[segmentIndex].m_widthList.begin(),
										m_segmentList[segmentIndex].m_widthList.end());
			if (maxValue < *max_it){
				maxValue = *max_it;
			}
		}
	}

	return maxValue;
}


template<class SegmentData>
double TMultiPolyline<SegmentData>::GetMinParallel() const
{
	double minValue = _qel_HUGE_NUMBER;

	if (GetParallelsCount() > 0){
		::std::vector<double>::const_iterator min_it;

		for (int segmentIndex = 0; segmentIndex < int(m_segmentList.size()); segmentIndex++){
			min_it = ::std::min_element(m_segmentList[segmentIndex].m_widthList.begin(),
										m_segmentList[segmentIndex].m_widthList.end());
			if (minValue > *min_it){
				minValue = *min_it;
			}
		}
	}

	return minValue;
}


template<class SegmentData>
bool TMultiPolyline<SegmentData>::Serialize(iser::IArchive& archive)
{
	int segmentCount = m_segmentList.size();
	int parallelCount = GetParallelsCount();

	bool retVal = true;

	retVal = retVal && CPolyline::Serialize(archive);

	static iser::CArchiveTag multiPolylineTag("MultiPolyline", "MultiPolyline");
	static iser::CArchiveTag segmentTag("Segment", "Segment");
	retVal = retVal && archive.BeginMultiTag(multiPolylineTag, segmentTag, segmentCount);
	retVal = retVal && archive.Process(parallelCount);

	if (!archive.IsStoring() && retVal){
		m_segmentList.resize(segmentCount);
		SetParallelsCount(parallelCount);
	}

	for (int segmentIndex = 0; segmentIndex < segmentCount; ++segmentIndex){
		retVal = retVal && m_segmentList[segmentIndex].Serialize(archive);
	}
	retVal = retVal && archive.EndTag(multiPolylineTag);

	return retVal;
}


} // namespace i2d


#endif // !i2d_TMultiPolyline_included

