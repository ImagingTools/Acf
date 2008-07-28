#ifndef iqt2d_CRectangleShape_included
#define iqt2d_CRectangleShape_included


#include <QGraphicsRectItem>

#include "istd/CIndex2d.h"

#include "imod/TSingleModelObserverBase.h"

#include "i2d/CRectangle.h"

#include "iqt2d/iqt2d.h"


namespace iqt2d
{


class CRectangleShape: public QGraphicsRectItem, public imod::TSingleModelObserverBase<i2d::CRectangle>
{
public:
	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
};


} // namespace iqt2d


#endif // !iqt2d_CRectangleShape_included


