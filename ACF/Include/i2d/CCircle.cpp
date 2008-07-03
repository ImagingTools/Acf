#include "i2d/CCircle.h"
#include "i2d/CRectangle.h"


namespace i2d
{	


CCircle::CCircle()
{
	m_radius = 0.0;
}


CCircle::CCircle(double radius, const CPoint& center)
{
	m_radius = radius;
	m_center = center;
}


double CCircle::GetRadius() const
{
	return m_radius;
}


// reimplemented (i2d::IObject2d)

i2d::IObject2d::CPoint CCircle::GetCenter() const
{
	return m_center;
}


i2d::CRectangle CCircle::GetBoundingBox() const
{
	return i2d::CRectangle(
				m_center.GetY() - m_radius, 
				m_center.GetX() - m_radius, 
				m_center.GetY() + m_radius, 
				m_center.GetX() + m_radius); 
}


} // namespace i2d

