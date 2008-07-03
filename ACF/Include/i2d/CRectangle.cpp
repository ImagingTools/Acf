#include "i2d/CRectangle.h"

#include "imath/CVector2d.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i2d
{	


CRectangle::CRectangle()
{
}


CRectangle::CRectangle(double top, double left, double bottom, double right)
{
	m_verticalRange.SetMinValue(top);
	m_verticalRange.SetMaxValue(bottom);

	m_horizontalRange.SetMinValue(left);
	m_horizontalRange.SetMaxValue(right);
}


CRectangle::CRectangle(const CRectangle& ref)
	:m_verticalRange(ref.m_verticalRange),
	m_horizontalRange(ref.m_horizontalRange)
{
}


CRectangle::CRectangle(const CPoint& topLeft, const CPoint& bottomRight)
{
	m_horizontalRange.SetMinValue(topLeft.GetX());
	m_horizontalRange.SetMaxValue(bottomRight.GetX());

	m_verticalRange.SetMinValue(topLeft.GetY());
	m_verticalRange.SetMaxValue(bottomRight.GetY());
}


const CRectangle& CRectangle::operator = (const CRectangle& ref)
{
	m_verticalRange = ref.m_verticalRange;
	m_horizontalRange = ref.m_horizontalRange;

	return *this;
}


bool CRectangle::IsInside(const CPoint& point) const
{
	return	(point.GetX() >= GetLeft()) && 
			(point.GetX() < GetRight()) && 
			(point.GetY() >= GetTop()) && 
			(point.GetY() < GetBottom());
}


bool CRectangle::IsInsideOf(const CRectangle& rect) const
{
	return		(rect.GetLeft() < GetLeft()) && 
				(rect.GetRight() > GetRight()) && 
				(rect.GetTop() < GetTop()) && 
				(rect.GetBottom() > GetBottom());
}


double CRectangle::GetWidth() const
{
	return double(GetRight() - GetLeft());
}


double CRectangle::GetHeight() const 
{
	return double(GetBottom() - GetTop());
}


bool CRectangle::IsEmpty() const
{
	return (GetWidth() > 0 && GetHeight() > 0);
}


bool CRectangle::IsIntersectedBy(const CRectangle& other) const
{
	return	IsInside(other.GetTopLeft()) || 
			IsInside(other.GetBottomLeft()) || 
			IsInside(other.GetTopRight()) || 
			IsInside(other.GetBottomRight()) ;
}


CRectangle CRectangle::GetIntersection(const CRectangle& other) const
{
	double outputLeft = istd::Min(other.GetLeft(), GetLeft());
	double outputTop = istd::Min(other.GetTop(), GetTop());
	double outputRight = istd::Max(other.GetRight(), GetRight());
	double outputBottom = istd::Max(other.GetBottom(), GetBottom());

	return CRectangle(outputLeft, outputRight, outputTop, outputBottom);
}


CRectangle CRectangle::GetUnion(const CRectangle& other) const
{
	double outputLeft = istd::Max(other.GetLeft(), GetLeft());
	double outputTop = istd::Max(other.GetTop(), GetTop());
	double outputRight = istd::Min(other.GetRight(), GetRight());
	double outputBottom = istd::Min(other.GetBottom(), GetBottom());

	return CRectangle(outputLeft, outputRight, outputTop, outputBottom);
}


double CRectangle::GetTop() const
{
	return m_verticalRange.GetMinValue();
}


double CRectangle::GetBottom() const
{
	return m_verticalRange.GetMaxValue();
}


double CRectangle::GetLeft() const
{
	return m_horizontalRange.GetMinValue();
}


double CRectangle::GetRight() const
{
	return m_horizontalRange.GetMaxValue();
}


i2d::IObject2d::CPoint CRectangle::GetTopLeft() const
{
	return CPoint(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue()); 
}


i2d::IObject2d::CPoint CRectangle::GetBottomLeft() const
{
	return CPoint(m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue()); 
}


i2d::IObject2d::CPoint CRectangle::GetTopRight() const
{
	return CPoint(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue()); 
}


i2d::IObject2d::CPoint CRectangle::GetBottomRight() const
{
	return CPoint(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue()); 
}


// reimplemented (i2d::IObject2d)

i2d::IObject2d::CPoint CRectangle::GetCenter() const
{
	return CPoint(GetWidth() / 2.0, GetHeight() / 2.0);
}


i2d::CRectangle CRectangle::GetBoundingBox() const
{
	return *this;
}


// reimplemented (iser::ISerializable)

bool CRectangle::Serialize(iser::IArchive& archive)
{
	double top = m_verticalRange.GetMinValue();
	double bottom = m_verticalRange.GetMaxValue();
	double left = m_horizontalRange.GetMinValue();
	double right = m_horizontalRange.GetMinValue();

	static iser::CArchiveTag topTag("Top", "Top");

	bool retVal = archive.BeginTag(topTag);
	retVal = retVal && archive.Process(top);
	retVal = retVal && archive.EndTag(topTag);

	static iser::CArchiveTag bottomTag("Bottom", "Bottom");
	retVal = retVal && archive.BeginTag(bottomTag);
	retVal = retVal && archive.Process(bottom);
	retVal = retVal && archive.EndTag(bottomTag);

	static iser::CArchiveTag leftTag("Left", "Left");
	retVal = retVal && archive.BeginTag(leftTag);
	retVal = retVal && archive.Process(left);
	retVal = retVal && archive.EndTag(leftTag);

	static iser::CArchiveTag rightTag("Right", "Right");
	retVal = retVal && archive.BeginTag(rightTag);
	retVal = retVal && archive.Process(right);
	retVal = retVal && archive.EndTag(rightTag);

	if (!archive.IsStoring()){
		m_verticalRange.SetMinValue(top);
		m_verticalRange.SetMaxValue(bottom);
		m_horizontalRange.SetMinValue(left);
		m_horizontalRange.SetMaxValue(right);
	}
	
	return retVal;
}

	
} // namespace i2d
