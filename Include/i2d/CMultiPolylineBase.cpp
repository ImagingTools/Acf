#include "i2d/CMultiPolylineBase.h"


namespace i2d
{


void CMultiPolylineBase::SelectAllSegments(bool selected)
{
	int segmentsCount = GetSegmentsCount();
	for(int i = 0; i < segmentsCount; i++){
		SelectSegment(i, selected);
	}
}


double CMultiPolylineBase::GetMaxParallel() const
{
	double maxWidth = 0;
	int parallelsCount = GetParallelsCount();

	if(parallelsCount > 0){
		int segmentsCount = GetSegmentsCount();

		for(int segmentIndex = 0; segmentIndex < segmentsCount; segmentIndex++){
			for (int parallelIndex = 0; parallelIndex < parallelsCount; ++parallelIndex){
				double width = GetParallel(segmentIndex, parallelIndex);
				if (width > maxWidth){
					maxWidth = width;
				}
			}
		}
	}

	return maxWidth;
}


} // namespace i2d

