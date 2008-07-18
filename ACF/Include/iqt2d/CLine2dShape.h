#ifndef iqt2d_CLine2dShape_included
#define iqt2d_CLine2dShape_included


#include <QGraphicsRectItem>

#include "istd/CIndex2d.h"

#include "imod/TSingleModelObserverBase.h"

#include "i2d/CLine2d.h"

#include "iqt2d/iqt2d.h"


namespace iqt2d
{


class CLine2dShape: public QGraphicsItem, public imod::TSingleModelObserverBase<i2d::CLine2d>
{
public:
	// reimplemented (QGraphicsRectItem)
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
};


} // namespace iqt2d


#endif // !iqt2d_CLine2dShape_included


