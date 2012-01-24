#ifndef i2d_TPolylineExPrimitive_included
#define i2d_TPolylineExPrimitive_included


// ACF includes
#include "i2d/CPolyline.h"


namespace i2d
{


template<class SegmentData>
class TPolylineExPrimitive: public CPolyline
{
public:
	typedef CPolyline BaseClass;

	/** Get segment user data.
	 */
	virtual const SegmentData& GetSegmentData(int segmentIndex) const;
	/** Set segment user data.
	 */
	virtual void SetSegmentData(int segmentIndex, const SegmentData& data);
	/** Remove node and segment.
	 */
	virtual bool RemoveSegment(int node, int segment);

	// reimplemented (i2d::CPolygon)
	virtual bool InsertNode(const i2d::CVector2d& node);
	virtual bool InsertNode(int index, const i2d::CVector2d& node);
	virtual void Clear();

	// reimplemented (i2d::CPolyline)
	virtual void SetClosed(bool state = true);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	::std::vector<SegmentData> m_segmentsData;
};


template<class SegmentData>
inline const typename SegmentData& TPolylineExPrimitive<SegmentData>::GetSegmentData(int segmentIndex) const
{
	I_ASSERT(segmentIndex >= 0);
	I_ASSERT(segmentIndex < int(m_segmentsData.size()));
	return m_segmentsData[segmentIndex];
}


template<class SegmentData>
inline void TPolylineExPrimitive<SegmentData>::SetSegmentData(int segmentIndex, const SegmentData& data)
{
	I_ASSERT(segmentIndex >= 0);
	I_ASSERT(segmentIndex < int(m_segmentsData.size()));

	m_segmentsData[segmentIndex] = data;
}


template<class SegmentData>
inline bool TPolylineExPrimitive<SegmentData>::RemoveSegment(int node, int segment)
{
	I_ASSERT(node >= 0);
	I_ASSERT(segment >= 0);
	I_ASSERT(segment < int(m_segmentsData.size()));

	node %= GetNodesCount();
	::std::vector<SegmentData>::iterator iter = m_segmentsData.begin();
	iter += segment;
	m_segmentsData.erase(iter);

	return RemoveNode(node);
}


// reimplemented (i2d::CPolygon)

template<class SegmentData>
inline bool TPolylineExPrimitive<SegmentData>::InsertNode(const i2d::CVector2d& node)
{
	int lastSegmentIndex = GetSegmentsCount() - 1;

	if (BaseClass::InsertNode(node)){
		int segmentsCount = GetSegmentsCount();
		if (lastSegmentIndex >= 0){
			m_segmentsData.resize(segmentsCount, m_segmentsData[lastSegmentIndex]);
		}
		else{
			m_segmentsData.resize(segmentsCount);
		}
		return true;
	}
	else{
		return false;
	}
}


template<class SegmentData>
inline bool TPolylineExPrimitive<SegmentData>::InsertNode(int index, const i2d::CVector2d& node)
{
	int nodesCount = GetNodesCount();
	if (nodesCount >= 1){
		SegmentData newSegment;

		if (nodesCount == 1){
			m_segmentsData.push_back(newSegment);
			if (IsClosed()){
				m_segmentsData.push_back(newSegment);
			}
		}
		else{
			if(index == 0){
				if (IsClosed()){
					newSegment = m_segmentsData.back();
				}
				else{
					newSegment = m_segmentsData.front();
				}
			}
			else{
				if (index >= nodesCount){
					newSegment = m_segmentsData.back();
				}
				else{
					newSegment = m_segmentsData[index - 1];
				}
			}

			::std::vector<SegmentData>::iterator iter = m_segmentsData.begin();
			iter += index;

			m_segmentsData.insert(iter, newSegment);
		}
	}

	return BaseClass::InsertNode(index, node);
}


template<class SegmentData>
inline void TPolylineExPrimitive<SegmentData>::Clear()
{
	m_segmentsData.clear();

	BaseClass::Clear();
}


// reimplemented (i2d::CPolyline)

template<class SegmentData>
inline void TPolylineExPrimitive<SegmentData>::SetClosed(bool state)
{
	if (IsClosed() == state){
		return;
	}

	if (state){
		m_segmentsData.push_back(m_segmentsData.back());
	}
	else{
		m_segmentsData.pop_back();
	}

	BaseClass::SetClosed(state);
}


// reimplemented (iser::ISerializable)

template<class SegmentData>
inline bool TPolylineExPrimitive<SegmentData>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag segmentTag("SegmentData", "Segment Data");
	static iser::CArchiveTag tag("PolylineEx", "PolylineEx");

	int segmentsCount = GetSegmentsCount();

	bool retVal = retVal && archive.BeginMultiTag(tag, segmentTag, segmentsCount);

	retVal = retVal && BaseClass::Serialize(archive);

	if (!archive.IsStoring()){
		if (retVal){
			m_segmentsData.resize(segmentsCount);
		}
		else{
			return false;
		}
	}

	for (int segmentIndex = 0; segmentIndex < segmentsCount; ++segmentIndex){
		retVal = retVal && archive.BeginTag(segmentTag);
		retVal = retVal && archive.Process(m_segmentsData[segmentIndex]);
		retVal = retVal && archive.EndTag(segmentTag);
	}

	retVal = retVal && archive.EndTag(tag);

	return retVal;
}


} // namespace i2d


#endif // !i2d_TPolylineExPrimitive_included

