#include "i2d/CCircle.h"
#include "i2d/CRectangle.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i2d
{	


CCircle::CCircle()
{
	m_radius = 0.0;
}


CCircle::CCircle(double radius, const CVector2d& center)
{
	m_radius = radius;
	m_center = center;
}


double CCircle::GetRadius() const
{
	return m_radius;
}


// reimplemented (IObject2d)

CVector2d CCircle::GetCenter() const
{
	return m_center;
}


CRectangle CCircle::GetBoundingBox() const
{
	return CRectangle(
				m_center.GetX() - m_radius, 
				m_center.GetY() - m_radius, 
				m_center.GetX() + m_radius, 
				m_center.GetY() + m_radius); 
}


// reimplemented (iser::ISerializable)

bool CCircle::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag centerTag("Center", "Circle center");
	retVal = retVal && archive.BeginTag(centerTag);
	retVal = retVal && m_center.Serialize(archive);
	retVal = retVal && archive.EndTag(centerTag);

	static iser::CArchiveTag radiusTag("Radius", "Circle radius");
	retVal = retVal && archive.BeginTag(radiusTag);
	retVal = retVal && archive.Process(m_radius);
	retVal = retVal && archive.EndTag(radiusTag);

	return retVal;
}


} // namespace i2d

