#ifndef iqt2d_CLine2dShape_included
#define iqt2d_CLine2dShape_included


#include <QGraphicsLineItem>

#include "imod/TSingleModelObserverBase.h"

#include "i2d/CLine2d.h"

#include "iqt2d/iqt2d.h"


namespace iqt2d
{


class CLine2dShape: public QGraphicsLineItem, public imod::TSingleModelObserverBase<i2d::CLine2d>
{
public:
	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
};


} // namespace iqt2d


#endif // !iqt2d_CLine2dShape_included


