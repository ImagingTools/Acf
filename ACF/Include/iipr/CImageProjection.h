#ifndef iipr_CImageProjection_included
#define iipr_CImageProjection_included


#include "iimg/IBitmap.h"
#include "istd/CIndex2d.h"
#include "imath/TVarVector.h"


namespace iipr
{
	

class CImageProjection: virtual public istd::IPolymorphic
{
public:
	typedef std::vector<double> ProjectionData;

	CImageProjection();
	CImageProjection(
				const iimg::IBitmap& bitmap, 
				const istd::CIndex2d& startPoint, 
				const istd::CIndex2d& endPoint);

	const ProjectionData& GetProjectionData() const;

private:
	void CreateProjection(
				const iimg::IBitmap& bitmap, 
				const istd::CIndex2d& startPoint, 
				const istd::CIndex2d& endPoint);
private:
	ProjectionData m_projectionData;
};


} // namespace iipr


#endif // !iipr_CImageProjection_included

