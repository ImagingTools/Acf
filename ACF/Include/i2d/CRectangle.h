#ifndef i2d_CRectangle_included
#define	i2d_CRectangle_included


#include "i2d/IObject2d.h"

#include "iser/ISerializable.h"

#include "istd/CRange.h"


namespace i2d
{	


class CRectangle:	virtual public i2d::IObject2d, 
					virtual public iser::ISerializable
{
public :
	CRectangle();
	CRectangle(double top, double left, double bottom, double right);
	CRectangle(const CRectangle& rect);
	CRectangle(const CPoint& topLeft, const CPoint& bottomRight);

	const CRectangle& operator = (const CRectangle& other);

	bool IsInside(const CPoint& pt) const;
	bool IsInsideOf(const CRectangle& rect) const;
	double GetWidth() const;
	double GetHeight() const;
	bool IsEmpty() const;

	bool IsIntersectedBy(const CRectangle& other) const;
	CRectangle GetIntersection(const CRectangle& other) const;
	CRectangle GetUnion(const CRectangle& other) const;

	double GetTop() const;
	double GetBottom() const;
	double GetLeft() const;
	double GetRight() const;

	CPoint GetTopLeft() const;
	CPoint GetBottomLeft() const;
	CPoint GetTopRight() const;
	CPoint GetBottomRight() const;

	// reimplemented (i2d::IObject2d)
	virtual CPoint GetCenter() const;
	virtual CRectangle GetBoundingBox() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CRange m_verticalRange;
	istd::CRange m_horizontalRange;
};
	
	
} // namespace i2d


#endif // !i2d_CRectangle_included
