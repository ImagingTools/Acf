#ifndef i2d_CRectangle_included
#define	i2d_CRectangle_included


#include "istd/CRange.h"
#include "istd/CIndex2d.h"

#include "iser/ISerializable.h"

#include "i2d/IObject2d.h"


namespace i2d
{


class CRectangle: virtual public IObject2d
{
public :
	CRectangle();
	CRectangle(double left, double top, double right, double bottom);
	CRectangle(const CRectangle& rect);
	CRectangle(const CVector2d& topLeft, const CVector2d& bottomRight);
	explicit CRectangle(const istd::CIndex2d& size);

	const CRectangle& operator=(const CRectangle& other);

	bool Contains(const CVector2d& point) const;
	bool Contains(const istd::CIndex2d& point) const;
	bool Contains(const CRectangle& rect) const;
	double GetWidth() const;
	double GetHeight() const;
	bool IsEmpty() const;
	bool IsValid() const;

	bool IsIntersectedBy(const CRectangle& other) const;
	CRectangle GetIntersection(const CRectangle& other) const;
	CRectangle GetUnion(const CRectangle& other) const;

	double GetLeft() const;
	double GetTop() const;
	double GetRight() const;
	double GetBottom() const;

	CVector2d GetTopLeft() const;
	CVector2d GetBottomLeft() const;
	CVector2d GetTopRight() const;
	CVector2d GetBottomRight() const;

	// reimplemented (IObject2d)
	virtual CVector2d GetCenter() const;
	virtual CRectangle GetBoundingBox() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CRange m_verticalRange;
	istd::CRange m_horizontalRange;
};
	
	
} // namespace i2d


#endif // !i2d_CRectangle_included
