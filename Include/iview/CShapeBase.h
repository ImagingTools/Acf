#ifndef iview_CShapeBase_included
#define iview_CShapeBase_included


// ACF includes
#include "imod/IModel.h"


#include "iview/IShape.h"
#include "iview/TShapeBase.h"


namespace iview
{


class CShapeBase: public TShapeBase<IShape>
{
public:
	typedef TShapeBase<IShape> BaseClass;

	CShapeBase();
	CShapeBase(const CShapeBase& shape);
	virtual ~CShapeBase();

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

private:
	imod::IModel* m_modelPtr;
};


} // namespace iview


#endif // !iview_CShapeBase_included


