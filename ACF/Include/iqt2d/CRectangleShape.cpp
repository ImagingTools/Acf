#include "iqt2d/CRectangleShape.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IObserver)

void CRectangleShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	update();
}


} // namespace iqt2d

