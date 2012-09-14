#ifndef iview_CColorShema_included
#define iview_CColorShema_included


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "iview/CColorShemaBase.h"


namespace iview
{


/**
	Implementation of the color shema object.
	This class extends class iview::CColorShemaBase with implementation of
	storage of pens, brushes and fonts.
*/
class CColorShema: public CColorShemaBase
{
public:
    enum
	{
		/**
			Standard size of pens set.
		*/
		NORMAL_SET_SIZE = 16,
	};

	CColorShema();

	// reimplemented IColorShema
    virtual const QFont& GetFont(IColorShema::StandardFont fontType) const;
    virtual void SetFont(iview::IColorShema::StandardFont fontType, const QFont& font);
	virtual const QPen& GetPen(IColorShema::StandardPen penType) const;
	virtual void SetPen(iview::IColorShema::StandardPen penType, const QPen& pen);
	virtual const QBrush& GetBrush(IColorShema::StandardBrush brushType) const;
	virtual void SetBrush(iview::IColorShema::StandardBrush brushType, const QBrush& brush);

private:
	QPen m_pens[int(IColorShema::SP_LAST) + 1];
	QBrush m_brushes[int(IColorShema::SB_LAST) + 1];
	QFont m_fonts[int(IColorShema::SF_LAST) + 1];
};



inline void CColorShema::SetPen(IColorShema::StandardPen penType, const QPen& pen)
{
	m_pens[penType] = pen;
}


inline void CColorShema::SetBrush(IColorShema::StandardBrush brushType, const QBrush& brush)
{
    m_brushes[brushType] = brush;
}


inline void CColorShema::SetFont(IColorShema::StandardFont fontType, const QFont& font)
{
    m_fonts[fontType] = font;
}


inline const QFont& CColorShema::GetFont(IColorShema::StandardFont fontType) const
{
    return m_fonts[fontType];
}


inline const QPen& CColorShema::GetPen(IColorShema::StandardPen penType) const
{
	return m_pens[penType];
}


inline const QBrush& CColorShema::GetBrush(IColorShema::StandardBrush brushType) const
{
	return m_brushes[brushType];
}


} // namespace iview


#endif // !iview_CColorShema_included


