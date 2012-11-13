#ifndef i2d_COrientedCircle_included
#define i2d_COrientedCircle_included


#include "i2d/CCircle.h"


namespace i2d
{	


/**
	Definition of graphical circle object with orientatedOutside.
*/
class COrientedCircle: public CCircle
{
public:
	typedef CCircle BaseClass;

	COrientedCircle();
	COrientedCircle(double radius, const CVector2d& center, bool orientatedOutside);

	bool IsOrientedOutside() const;
	void SetOrientedOutside(bool orientatedOutside);

	bool operator==(const COrientedCircle& circle) const;
	bool operator!=(const COrientedCircle& circle) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	bool m_orientatedOutside;
};


// inline methods

inline bool COrientedCircle::IsOrientedOutside() const
{
	return m_orientatedOutside;
}


} // namespace i2d


#endif // !i2d_COrientedCircle_included


