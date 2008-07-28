#ifndef iqtipr_CSearchResultShape_included
#define iqtipr_CSearchResultShape_included


#include <QGraphicsItemGroup>


#include "istd/TPointerVector.h"

#include "imod/TSingleModelObserverBase.h"

#include "iipr/IFeaturesContainer.h"


namespace iqtipr
{


class CSearchResultShape: 
			public QGraphicsItemGroup, 
			public imod::TSingleModelObserverBase<iipr::IFeaturesContainer>
{
public:
	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

private:
	class CModelPointShape: public QGraphicsEllipseItem
	{
	public:
		typedef QGraphicsEllipseItem BaseClass;

		CModelPointShape(double x, double y, double w, double h, QGraphicsItem* parentPtr = NULL, QGraphicsScene* scenePtr = NULL, double angle = 0);
	
	protected:
		// reimplemented (QGraphicsRectItem)
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	private:
		double m_angle;
	};

	typedef istd::TPointerVector<QGraphicsItem> Items;

	Items m_items;
};


} // namespace iqtipr


#endif // !iqtipr_CSearchResultShape_included

