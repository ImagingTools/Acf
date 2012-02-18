#ifndef iview_CColorShemaBase_included
#define iview_CColorShemaBase_included


// Qt includes
#include <QPainter>


// ACF includes
#include "iview/IColorShema.h"


namespace iview
{


class CColorShemaBase: public IColorShema
{
public:
    enum
	{
		TICKER_SIZE = 4,
		SMALL_TICKER_SIZE = 2,
		LOGICAL_LINE_WIDTH = 4
	};

	// reimplemented (IColorShema)
	virtual void Assign(const IColorShema& colorShema);
	virtual void DrawTicker(QPainter& drawContext, istd::CIndex2d point, IColorShema::TickerType tickerType) const;
	virtual const i2d::CRect& GetTickerBox(IColorShema::TickerType tickerType) const;
	virtual int GetLogicalLineWidth() const;

protected:
     // static attributes
	static i2d::CRect s_tickersBoxes[(int)TT_LAST + (int)1];
};


inline int CColorShemaBase::GetLogicalLineWidth() const
{
	return LOGICAL_LINE_WIDTH;
}


} // namespace iview


#endif // !iview_CColorShemaBase_included



