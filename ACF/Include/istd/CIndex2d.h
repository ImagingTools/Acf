#ifndef istd_CIndex2d_included
#define istd_CIndex2d_included


#include "istd/TIndex.h"


namespace istd
{


class CIndex2d: public TIndex<2>
{
public:
	typedef TIndex<2> BaseClass;

	CIndex2d();
	CIndex2d(int x, int y);

	int GetX() const;
	void SetX(int value);
	int GetY() const;
	void SetY(int value);
};


// inline methods

inline CIndex2d::CIndex2d()
{
}


inline CIndex2d::CIndex2d(int x, int y)
{
	SetAt(0, x);
	SetAt(1, y);
}


inline int CIndex2d::GetX() const
{
	return GetAt(0);
}


inline void CIndex2d::SetX(int value)
{
	SetAt(0, value);
}


inline int CIndex2d::GetY() const
{
	return GetAt(1);
}


inline void CIndex2d::SetY(int value)
{
	SetAt(1, value);
}


} // namespace istd


#endif // !istd_CIndex2d_included


