#include "iview/CColorShema.h"


// Qt includes
#include <QtGui/QApplication>
#include <QtGui/QPalette>


namespace iview
{


CColorShema::CColorShema()
{
	m_pens[SP_NORMAL] = QColor(0, 255, 0);
	m_pens[SP_SELECTED] = QColor(255, 255, 0);
	m_pens[SP_SELECTED].setWidthF(1.5);
	m_pens[SP_TICKER] = QColor(0, 192, 255);
	m_pens[SP_SELECTED_TICKER] = QColor(128, 0, 128);
	m_pens[SP_IMPORTANT] = QColor(255, 0, 255);
	m_pens[SP_IMPORTANT].setWidthF(1.5);
	m_pens[SP_ORIENT_DARK] = QColor(0, 0, 0);
	m_pens[SP_ORIENT_BRIGHT] = QColor(255, 255, 255);
	m_pens[SP_NORMAL_DASH] = QPen(QColor(0, 255, 255), 0, Qt::DashLine);
	m_pens[SP_SELECTED_DASH] = QPen(QColor(255, 255, 0), 0, Qt::DashLine);

	m_pens[SP_GUIDELINE1] = QColor(255, 0, 0, 80);
	m_pens[SP_GUIDELINE2] = QColor(255, 0, 0, 60);
	m_pens[SP_GUIDELINE3] = QColor(255, 0, 0, 30);

	m_pens[SP_BLACK] = QColor(0, 0, 0);
	m_pens[SP_WHITE] = QColor(255, 255, 255);
	m_pens[SP_RED] = QColor(255, 0, 0);
	m_pens[SP_GREEN] = QColor(0, 255, 0);
	m_pens[SP_BLUE] = QColor(0, 0, 255);
	m_pens[SP_CYAN] = QColor(0, 255, 255);
	m_pens[SP_YELLOW] = QColor(255, 255, 0);
	m_pens[SP_MAGENTA] = QColor(255, 0, 255);

	m_brushes[SB_NORMAL] = QBrush(QColor(128, 96, 96));
	m_brushes[SB_SELECTED] = QBrush(QColor(255, 128, 128));
	m_brushes[SB_TICKER] = QBrush(QColor(128, 128, 255));
	m_brushes[SB_SELECTED_TICKER] = QBrush(QColor(255, 128, 128));
	m_brushes[SB_IMPORTANT] = QBrush(QColor(0, 0, 255));
	m_brushes[SB_BACKGROUND] = qApp->palette().color(QPalette::Window);
	m_brushes[SB_HALF_TRANSPARENT] = QBrush(QColor(255, 0, 0, 50));
	m_brushes[SB_HALF_TRANSPARENT2] = QBrush(QColor(0, 0, 255, 50));
	m_brushes[SB_TRANSPARENT] = QBrush(QColor(0, 0, 0), Qt::NoBrush);

	m_fonts[iview::IColorShema::SF_NORMAL] = QFont("Arial");
}


} // namespace iview


