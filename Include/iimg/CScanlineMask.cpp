#include "iimg/CScanlineMask.h"


// STL includes
#include <set>

// Qt includes
#include <QtCore/qmath.h>


namespace iimg
{


// public methods

CScanlineMask::CScanlineMask()
:	m_isEmpty(true)
{
}


bool CScanlineMask::IsBitmapRegionEmpty() const
{
	return m_isEmpty;
}


i2d::CRect CScanlineMask::GetBoundingBox() const
{
	return m_boundingBox;
}


const CScanlineMask::PixelRanges* CScanlineMask::GetPixelRanges(int lineIndex) const
{
	int rangeIndex = lineIndex - m_boundingBox.GetTop();

	if ((rangeIndex >= 0) && (rangeIndex < int(m_scanlines.size()))){
		return m_scanlines[rangeIndex];
	}

	return NULL;
}


bool CScanlineMask::CreateFromGeometry(const i2d::IObject2d& geometry, const i2d::CRect* clipAreaPtr)
{
	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(&geometry);
	if (annulusPtr != NULL){
		CreateFromAnnulus(*annulusPtr, clipAreaPtr);

		return true;
	}

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(&geometry);
	if (circlePtr != NULL){
		CreateFromCircle(*circlePtr, clipAreaPtr);

		return true;
	}

	const i2d::CRectangle* rectanglePtr = dynamic_cast<const i2d::CRectangle*>(&geometry);
	if (rectanglePtr != NULL){
		CreateFromRectangle(*rectanglePtr, clipAreaPtr);

		return true;
	}

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(&geometry);
	if (polygonPtr != NULL){
		CreateFromPolygon(*polygonPtr, clipAreaPtr);

		return true;
	}

	return false;
}


void CScanlineMask::CreateFromCircle(const i2d::CCircle& circle, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(circle.GetBoundingBox(), clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetImage();

		return;
	}

	const i2d::CVector2d& center = circle.GetPosition();
	double radius = circle.GetRadius();
	double radius2 = radius * radius;

	int linesCount = m_boundingBox.GetHeight();

	m_scanlines.resize(linesCount);
	m_rangesContainer.resize(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		PixelRanges& rangeList = m_rangesContainer[lineIndex];

		m_scanlines[lineIndex] =  NULL;

		double y = (lineIndex + m_boundingBox.GetTop() - center.GetY());
		double radiusDiff2 = radius2 - y * y;

		if (radiusDiff2 >= 0){
			double radiusDiff = qSqrt(radiusDiff2);
			int left = int(center.GetX() - radiusDiff);
			int right = int(center.GetX() + radiusDiff);

			if (clipAreaPtr != NULL){
				if (left < clipAreaPtr->GetLeft()){
					left = clipAreaPtr->GetLeft();
				}

				if (right > clipAreaPtr->GetRight()){
					right = clipAreaPtr->GetRight();
				}
			}

			if (left < right){
				rangeList.push_back(istd::CIntRange(left, right));

				m_scanlines[lineIndex] = &rangeList;

				m_isEmpty = false;
			}
		}
	}
}


void CScanlineMask::CreateFromRectangle(const i2d::CRectangle& rect, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(rect, clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetImage();

		return;
	}

	m_isEmpty = false;

	int linesCount = m_boundingBox.GetHeight();

	m_scanlines.resize(linesCount);

	m_rangesContainer.resize(1);
	PixelRanges& rangeList = m_rangesContainer.front();
	rangeList.push_back(m_boundingBox.GetHorizontalRange());

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		m_scanlines[lineIndex] = &rangeList;	// set all lines to the same range
	}
}


void CScanlineMask::CreateFromAnnulus(const i2d::CAnnulus& annulus, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(annulus.GetBoundingBox(), clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetImage();

		return;
	}

	const i2d::CVector2d& center = annulus.GetCenter();
	double outerRadius = annulus.GetOuterRadius();
	double outerRadius2 = outerRadius * outerRadius;

	double innerRadius = annulus.GetInnerRadius();
	double innerRadius2 = innerRadius * innerRadius;

	int linesCount = int(m_boundingBox.GetHeight());

	double centerX = center.GetX();
	double centerY = center.GetY();

	m_scanlines.resize(linesCount);
	m_rangesContainer.resize(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		PixelRanges& rangeList = m_rangesContainer[lineIndex];
		double y = (lineIndex + m_boundingBox.GetTop() - centerY);

		double outputRadiusDiff2 = outerRadius2 - y * y;

		if (outputRadiusDiff2 < 0){
			m_scanlines[lineIndex] =  NULL;

			continue;
		}

		double outputRadiusDiff = qSqrt(outputRadiusDiff2);
		int outerLeft = int(centerX - outputRadiusDiff);
		int outerRight = int(centerX + outputRadiusDiff);

		if (clipAreaPtr != NULL){
			if (outerLeft < clipAreaPtr->GetLeft()){
				outerLeft = clipAreaPtr->GetLeft();
			}

			if (outerRight > clipAreaPtr->GetRight()){
				outerRight = clipAreaPtr->GetRight();
			}
		}

		double innerRadiusDiff2 = innerRadius2 - y * y;
		if (innerRadiusDiff2 >= 0){
			double innerRadiusDiff = qSqrt(innerRadiusDiff2);

			int innerLeft = int(centerX - innerRadiusDiff);
			int innerRight = int(centerX + innerRadiusDiff);

			if (innerLeft < innerRight){
				if (outerLeft < innerLeft){
					rangeList.push_back(istd::CIntRange(outerLeft, innerLeft));
				}

				if (innerRight < outerRight){
					rangeList.push_back(istd::CIntRange(innerRight, outerRight));
				}

				continue;
			}
		}
		
		if (outerLeft < outerRight){
			rangeList.push_back(istd::CIntRange(outerLeft, outerRight));
		}

		if (!rangeList.isEmpty()){
			m_scanlines[lineIndex] = &rangeList;

			m_isEmpty = false;
		}
		else{
			m_scanlines[lineIndex] =  NULL;
		}
	}
}


void CScanlineMask::CreateFromPolygon(const i2d::CPolygon& polygon, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(polygon.GetBoundingBox(), clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetImage();

		return;
	}

	int linesCount = m_boundingBox.GetHeight();

	// QVector of lines by Y coordinates;
	// every line is QList of X coordinates of the polygon lines points 
	QVector< std::set<int> > scanVector(linesCount);

	int nodesCount = polygon.GetNodesCount();
	for (int i = 0; i < nodesCount; i++){
		int nextIndex = i + 1;
		if (nextIndex >= nodesCount){
			nextIndex = 0;
		}

		i2d::CVector2d startPoint = polygon.GetNode(i);
		i2d::CVector2d endPoint = polygon.GetNode(nextIndex);

		double y1 = startPoint.GetY() - m_boundingBox.GetTop();
		double y2 = endPoint.GetY() - m_boundingBox.GetTop();

		double x1 = startPoint.GetX();
		double x2 = endPoint.GetX();

		if (y2 < y1){
			qSwap(y1, y2);
			qSwap(x1, x2);
		}

		int firstLine = qMax(int(y1), 0);
		int lastLine = qMin(int(y2), linesCount);

		if (firstLine < lastLine){
			double deltaX = (x2 - x1) / (y2 - y1);

			// add line to list of points
			double positionX = x1 + (firstLine - y1) * deltaX;
			for (int lineIndex = firstLine; lineIndex < lastLine; ++lineIndex){
				int x = int(positionX);

				std::set<int> points = scanVector[lineIndex];

				std::set<int>::iterator foundIter = points.find(x);
				if (foundIter != points.end()){
					points.erase(foundIter);
				}
				else{
					points.insert(x);
				}

				positionX += deltaX;
			}
		}
	}

	// build the scan ranges
	m_scanlines.resize(linesCount);
	m_rangesContainer.resize(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		PixelRanges& rangeList = m_rangesContainer[lineIndex];

		const std::set<int>& lineList = scanVector.at(lineIndex);
		Q_ASSERT((lineList.size() % 2) == 0);	// pair number of points should be calculated

		int rangesCount = int(lineList.size()) / 2;

		std::set<int>::const_iterator iter = lineList.begin();
		for (int i = 0; i < rangesCount; ++i){
			int left = *(iter++);
			Q_ASSERT(iter != lineList.end());
			int right = *(iter++);
			Q_ASSERT(iter != lineList.end());

			if (clipAreaPtr != NULL){
				if (left < clipAreaPtr->GetLeft()){
					left = clipAreaPtr->GetLeft();
				}

				if (right > clipAreaPtr->GetRight()){
					right = clipAreaPtr->GetRight();
				}
			}

			if (left < right){
				rangeList.push_back(istd::CIntRange(left, right));
			}
		}

		if (!rangeList.isEmpty()){
			m_scanlines[lineIndex] = &rangeList;
			m_isEmpty = false;
		}
		else{
			m_scanlines[lineIndex] =  NULL;
		}
	}
}


CScanlineMask CScanlineMask::GetUnion(const CScanlineMask& mask) const
{
	CScanlineMask result;

	GetUnion(mask, result);

	return result;
}


void CScanlineMask::GetUnion(const CScanlineMask& mask, CScanlineMask& result) const
{
	result.m_boundingBox = m_boundingBox.GetUnion(mask.m_boundingBox);
	result.m_isEmpty = m_isEmpty || mask.m_isEmpty;

	result.m_scanlines.resize(result.m_boundingBox.GetHeight());

	for (int y = result.m_boundingBox.GetTop(); y < result.m_boundingBox.GetBottom(); ++y){
		const PixelRanges* rangesPtr = NULL;
		const PixelRanges* maskRangesPtr = NULL;

		int lineIndex = y - m_boundingBox.GetTop();
		if ((lineIndex >= 0) && (lineIndex < m_scanlines.size())){
			rangesPtr = m_scanlines[lineIndex];
		}

		int maskLineIndex = y - mask.m_boundingBox.GetTop();
		if ((maskLineIndex >= 0) && (maskLineIndex < mask.m_scanlines.size())){
			maskRangesPtr = mask.m_scanlines[lineIndex];
		}

		if (rangesPtr != NULL){
			result.m_rangesContainer.push_back(PixelRanges());

			PixelRanges& unionRanges = result.m_rangesContainer.back();

			if (maskRangesPtr != NULL){
				GetLineUnion(*rangesPtr, *maskRangesPtr, unionRanges);
			}
			else{
				unionRanges = *rangesPtr;
			}

			result.m_scanlines[y - result.m_boundingBox.GetTop()] = &unionRanges;
		}
		else if (maskRangesPtr != NULL){
			result.m_rangesContainer.push_back(PixelRanges());

			PixelRanges& unionRanges = result.m_rangesContainer.back();

			unionRanges = *maskRangesPtr;

			result.m_scanlines[y - result.m_boundingBox.GetTop()] = &unionRanges;
		}
		else{
			result.m_scanlines[y - result.m_boundingBox.GetTop()] = NULL;
		}
	}
}


void CScanlineMask::Union(const CScanlineMask& mask)
{
	*this = GetUnion(mask);
}


CScanlineMask CScanlineMask::GetIntersection(const CScanlineMask& mask) const
{
	CScanlineMask result;

	GetIntersection(mask, result);

	return result;
}


void CScanlineMask::GetIntersection(const CScanlineMask& mask, CScanlineMask& result) const
{
	result.m_boundingBox = m_boundingBox.GetUnion(mask.m_boundingBox);
	result.m_isEmpty = true;

	result.m_scanlines.resize(result.m_boundingBox.GetHeight());

	for (int y = result.m_boundingBox.GetTop(); y < result.m_boundingBox.GetBottom(); ++y){
		int lineIndex = y - m_boundingBox.GetTop();
		int maskLineIndex = y - mask.m_boundingBox.GetTop();
		if (		(lineIndex >= 0) && (lineIndex < m_scanlines.size()) &&
					(maskLineIndex >= 0) && (maskLineIndex < mask.m_scanlines.size())){
			const PixelRanges* rangesPtr = m_scanlines[lineIndex];
			const PixelRanges* maskRangesPtr = mask.m_scanlines[lineIndex];

			PixelRanges resultRanges;
			GetLineIntersection(*rangesPtr, *maskRangesPtr, resultRanges);

			if (!resultRanges.isEmpty()){
				result.m_rangesContainer.push_back(resultRanges);

				PixelRanges& intersectedRanges = result.m_rangesContainer.back();

				result.m_scanlines[y - result.m_boundingBox.GetTop()] = &intersectedRanges;

				result.m_isEmpty = false;
			}
			else{
				result.m_scanlines[y - result.m_boundingBox.GetTop()] = NULL;
			}
		}
		else{
			result.m_scanlines[y - result.m_boundingBox.GetTop()] = NULL;
		}
	}
}


void CScanlineMask::Intersection(const CScanlineMask& mask)
{
	*this = GetIntersection(mask);
}


CScanlineMask CScanlineMask::GetTranslated(int dx, int dy) const
{
	CScanlineMask result;

	GetTranslated(dx, dy, result);

	return result;
}


void CScanlineMask::GetTranslated(int dx, int dy, CScanlineMask& result) const
{
	result = *this;

	result.Translate(dx, dy);
}


void CScanlineMask::Translate(int dx, int dy)
{
	m_boundingBox.SetTop(m_boundingBox.GetTop() + dy);
	m_boundingBox.SetBottom(m_boundingBox.GetBottom() + dy);

	if (dx != NULL){
		m_boundingBox.SetLeft(m_boundingBox.GetLeft() + dx);
		m_boundingBox.SetRight(m_boundingBox.GetRight() + dx);

		for (		RangesContainer::Iterator lineIter = m_rangesContainer.begin();
					lineIter != m_rangesContainer.end();
					++lineIter){
			PixelRanges& lineRanges = *lineIter;

			for (		PixelRanges::Iterator rangeIter = lineRanges.begin();
						rangeIter != lineRanges.end();
						++rangeIter){
				istd::CIntRange& range = *rangeIter;

				range.GetMinValueRef() += dx;
				range.GetMaxValueRef() += dx;
			}
		}
	}
}


// reimplemented (iimg::IRasterImage)

bool CScanlineMask::IsEmpty() const
{
	for (		Scanlines::ConstIterator iter = m_scanlines.constBegin();
				iter != m_scanlines.constEnd();
				++iter){
		if (*iter != NULL){
			return false;
		}
	}

	return true;
}


void CScanlineMask::ResetImage()
{
	m_rangesContainer.clear();
	m_scanlines.clear();

	m_boundingBox = i2d::CRect::GetEmpty();

	m_isEmpty = true;
}


istd::CIndex2d CScanlineMask::GetImageSize() const
{
	return m_boundingBox.GetRightBottom();
}


int CScanlineMask::GetComponentsCount() const
{
	return 1;
}


icmm::CVarColor CScanlineMask::GetColorAt(const istd::CIndex2d& position) const
{
	int scanLine = position.GetY() - m_boundingBox.GetTop();
	if ((scanLine >= 0) && (scanLine < m_scanlines.size())){
		const PixelRanges* rangesPtr = m_scanlines[scanLine];
		if (rangesPtr != NULL){
			for (		PixelRanges::ConstIterator rangeIter = rangesPtr->constBegin();
						rangeIter != rangesPtr->constEnd();
						++rangeIter){
				const istd::CIntRange& range = *rangeIter;
				if (position.GetX() >= range.GetMaxValue()){
					break;
				}

				if (position.GetX() >= range.GetMinValue()){
					return icmm::CVarColor(1, 1);
				}
			}
		}
	}

	return icmm::CVarColor(1, 0);
}


bool CScanlineMask::SetColorAt(const istd::CIndex2d& /*position*/, const icmm::CVarColor& /*color*/)
{
	return false;	// this is not editable using color set at pixel index
}


// reimplemented (iser::ISerializable)

bool CScanlineMask::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag minYTag("MinY", "Minimal Y");
	static iser::CArchiveTag maxYTag("MaxY", "Maximal Y (exclusive)");
	static iser::CArchiveTag scanlinesTag("Scanelines", "List of mask scan lines");
	static iser::CArchiveTag rangesTag("Ranges", "List of ranges for single scan line");
	static iser::CArchiveTag rangeTag("Range", "Single mask range");

	bool retVal = true;

	if (archive.IsStoring()){
		int minY = m_boundingBox.GetTop();

		retVal = retVal && archive.BeginTag(minYTag);
		retVal = retVal && archive.Process(minY);
		retVal = retVal && archive.EndTag(minYTag);

		int scanLinesCount = m_scanlines.size();

		retVal = retVal && archive.BeginMultiTag(scanlinesTag, rangesTag, scanLinesCount);
		for (		Scanlines::ConstIterator lineIter = m_scanlines.constBegin();
					lineIter != m_scanlines.constEnd();
					++lineIter){
			const PixelRanges* rangesPtr = *lineIter;
			int rangesCount = 0;

			if (rangesPtr != NULL){
				rangesCount = rangesPtr->size();
			}

			retVal = retVal && archive.BeginMultiTag(rangesTag, rangeTag, rangesCount);

			for (int rangeIndex = 0; rangeIndex < rangesCount; ++rangeIndex){
				istd::CIntRange range = rangesPtr->at(rangeIndex);
				retVal = retVal && archive.BeginTag(rangeTag);
				retVal = retVal && archive.Process(range.GetMinValueRef());
				retVal = retVal && archive.Process(range.GetMaxValueRef());
				retVal = retVal && archive.EndTag(rangeTag);
			}

			retVal = retVal && archive.EndTag(rangesTag);
		}
	}
	else{
		int minY = 0;

		retVal = retVal && archive.BeginTag(minYTag);
		retVal = retVal && archive.Process(minY);
		retVal = retVal && archive.EndTag(minYTag);

		m_rangesContainer.clear();
		m_scanlines.clear();

		int scanLinesCount = 0;

		retVal = retVal && archive.BeginMultiTag(scanlinesTag, rangesTag, scanLinesCount);

		if (!retVal || (scanLinesCount < 0)){
			return false;
		}

		m_scanlines.resize(scanLinesCount);

		for (int lineIndex = 0; lineIndex < scanLinesCount; ++lineIndex){
			int rangesCount = 0;

			retVal = retVal && archive.BeginMultiTag(rangesTag, rangeTag, rangesCount);

			if (!retVal || (rangesCount < 0)){
				CScanlineMask::ResetImage();

				return false;
			}

			if (rangesCount > 0){
				m_rangesContainer.push_back(PixelRanges());

				PixelRanges& ranges = m_rangesContainer.back();

				for (int rangeIndex = 0; rangeIndex < rangesCount; ++rangeIndex){
					istd::CIntRange range;
					retVal = retVal && archive.BeginTag(rangeTag);
					retVal = retVal && archive.Process(range.GetMinValueRef());
					retVal = retVal && archive.Process(range.GetMaxValueRef());
					retVal = retVal && archive.EndTag(rangeTag);

					if (!retVal){
						CScanlineMask::ResetImage();

						return false;
					}

					ranges.push_back(range);
				}

				m_scanlines[lineIndex] = &ranges;
			}

			retVal = retVal && archive.EndTag(rangesTag);
		}
	}

	return retVal;
}


// static methods

void CScanlineMask::GetLineUnion(const PixelRanges& line1, const PixelRanges& line2, PixelRanges& result)
{
	PixelRanges::ConstIterator iter1 = line1.constBegin();
	PixelRanges::ConstIterator iter2 = line2.constBegin();
	for (;;){
		if (iter1 != line1.constEnd()){
			const istd::CIntRange& range1 = *iter1;

			if (iter2 != line2.constEnd()){
				const istd::CIntRange& range2 = *iter2;

				if (range1.GetMaxValue() < range2.GetMinValue()){
					result.push_back(range1);

					++iter1;
				}
				else if (range2.GetMaxValue() < range1.GetMinValue()){
					result.push_back(range2);

					++iter2;
				}
				else{
					int rangeBegin = qMin(range1.GetMinValue(), range2.GetMinValue());
					int rangeEnd = qMax(range1.GetMaxValue(), range2.GetMaxValue());

					++iter1;
					++iter2;

					for (;;){
						if ((iter1 != line1.constEnd()) && (iter1->GetMinValue() <= rangeEnd)){	// if first range connects to the current range without gap..
							if (iter1->GetMinValue() > rangeEnd){	// if the range extends current range, its end will be used
								rangeEnd = iter1->GetMinValue();
							}

							++iter1;
						}
						else if ((iter2 != line1.constEnd()) && (iter2->GetMinValue() <= rangeEnd)){	// if first range connects to the current range without gap..
							if (iter2->GetMinValue() > rangeEnd){	// if the range extends current range, its end will be used
								rangeEnd = iter2->GetMinValue();
							}

							++iter2;
						}
						else{
							break;
						}
					}

					result.push_back(istd::CIntRange(rangeBegin, rangeEnd));
				}
			}
			else{
				result.push_back(range1);

				++iter1;
			}
		}
		else if (iter2 != line2.constEnd()){
			const istd::CIntRange& range2 = *iter2;

			result.push_back(range2);

			++iter2;
		}
		else{
			break;	// finish
		}
	}
}


void CScanlineMask::LineUnion(const PixelRanges& line, PixelRanges& result)
{
	PixelRanges::Iterator resultIter = result.begin();

	for (		PixelRanges::ConstIterator iter = line.constBegin();
				iter != line.constEnd();
				++iter){
		const istd::CIntRange& range = *iter;

		for (; (resultIter != result.end()) && (range.GetMinValue() > resultIter->GetMaxValue()); ++resultIter);	// skip non colliding ranges

		if (resultIter != result.end()){
			Q_ASSERT(range.GetMinValue() <= resultIter->GetMaxValue());

			if (range.GetMaxValue() < resultIter->GetMinValue()){	// our new range is before
				resultIter = result.insert(resultIter, range);
			}
			else{
				if (range.GetMinValue() < resultIter->GetMinValue()){	// our new range extends the range on the left side
					resultIter->SetMinValue(range.GetMinValue());
				}

				if (range.GetMaxValue() > resultIter->GetMaxValue()){	// our new range extends the range on the right side
					int maxRight = range.GetMaxValue();

					// remove contained ranges
					PixelRanges::Iterator removeIter = resultIter;
					for (		++resultIter;
								(removeIter != result.end()) && (range.GetMaxValue() > removeIter->GetMinValue());
								removeIter = result.erase(removeIter)){
						if (removeIter->GetMaxValue() > maxRight){
							maxRight = removeIter->GetMaxValue();
						}
					}

					resultIter->SetMaxValue(maxRight);

					resultIter = removeIter;
				}
			}
		}
		else{
			// at the end we can simply insert elements
			resultIter = result.insert(resultIter, range);
		}
	}
}


void CScanlineMask::GetLineIntersection(const PixelRanges& line1, const PixelRanges& line2, PixelRanges& result)
{
	PixelRanges::ConstIterator iter1 = line1.constBegin();
	PixelRanges::ConstIterator iter2 = line2.constBegin();
	for (;;){
		if (iter1 != line1.constEnd()){
			const istd::CIntRange& range1 = *iter1;

			if (iter2 != line2.constEnd()){
				const istd::CIntRange& range2 = *iter2;

				if (range1.GetMaxValue() <= range2.GetMinValue()){
					++iter1;
				}
				else if (range2.GetMaxValue() <= range1.GetMinValue()){
					++iter2;
				}
				else{
					int rangeBegin = qMax(range1.GetMinValue(), range2.GetMinValue());
					int rangeEnd = qMin(range1.GetMaxValue(), range2.GetMaxValue());

					result.push_back(istd::CIntRange(rangeBegin, rangeEnd));

					if (range1.GetMaxValue() < range2.GetMaxValue()){
						++iter1;
					}
					else{
						++iter2;
					}
				}
			}
			else{
				break;	// finish
			}
		}
		else{
			break;	// finish
		}
	}
}


void CScanlineMask::LineIntersection(const PixelRanges& line, PixelRanges& result)
{
	PixelRanges::Iterator resultIter = result.begin();

	for (		PixelRanges::ConstIterator iter = line.constBegin();
				iter != line.constEnd();){
		const istd::CIntRange& range = *iter;

		// remove non colliding ranges
		for (; (resultIter != result.end()) && (range.GetMinValue() > resultIter->GetMaxValue()); resultIter = result.erase(resultIter));

		if (resultIter == result.end()){
			// no more elements in result - finish
			return;
		}

		Q_ASSERT(range.GetMinValue() <= resultIter->GetMaxValue());

		if (range.GetMaxValue() > resultIter->GetMinValue()){	// our new range overlaps range in result
			if (range.GetMaxValue() < resultIter->GetMaxValue()){	// our new range cuts the range on the left side
				// divide the result range into 2 segments
				resultIter = result.insert(resultIter, istd::CIntRange(resultIter->GetMinValue(), range.GetMaxValue()));
				++resultIter;
				resultIter->SetMinValue(range.GetMaxValue() + 1);

				++iter;
			}
			else if (range.GetMaxValue() > resultIter->GetMaxValue()){	// our new range contains the result range
				++resultIter;
			}
		}
		else{
			++iter;
		}
	}
}


// protected methods

void CScanlineMask::SetBoundingBox(const i2d::CRectangle& objectBoundingBox, const i2d::CRect* clipAreaPtr)
{
	m_boundingBox.SetLeft(int(qFloor(objectBoundingBox.GetLeft())));
	m_boundingBox.SetRight(int(qCeil(objectBoundingBox.GetRight())));
	m_boundingBox.SetTop(int(qFloor(objectBoundingBox.GetTop())));
	m_boundingBox.SetBottom(int(qCeil(objectBoundingBox.GetBottom())));

	if (clipAreaPtr != NULL){
		m_boundingBox.Intersection(*clipAreaPtr);
	}
}


} // namespace iimg


