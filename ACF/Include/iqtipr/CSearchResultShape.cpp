#include "iqtipr/CSearchResultShape.h"


#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "iipr/CSearchFeature.h"


namespace iqtipr
{


// public methods

// reimplemented (imod::IObserver)

void CSearchResultShape::BeforeUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	for (int itemIndex = 0; itemIndex < m_items.GetCount(); itemIndex++){
		removeFromGroup(m_items.GetAt(itemIndex));
	}

	m_items.Reset();
}


void CSearchResultShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	const iipr::IFeaturesContainer* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		iipr::IFeaturesContainer::Features features = objectPtr->GetFeatures();

		for (int featureIndex = 0; featureIndex < int(features.size()); featureIndex++){
			const iipr::IFeature* featurePtr = features[featureIndex];
			I_ASSERT(featurePtr != NULL);
			I_ASSERT(featurePtr->GetPosition().GetElementsCount() == 2);

			double angle = 0;

			const iipr::CSearchFeature* searchFeaturePtr = dynamic_cast<const iipr::CSearchFeature*>(featurePtr);
			if (searchFeaturePtr != NULL){
				angle = searchFeaturePtr->GetAngle();
			}

			double x = featurePtr->GetPosition()[0];
			double y = featurePtr->GetPosition()[1];

			CModelPointShape* itemPtr = new CModelPointShape(x, y, 25, 25, NULL, NULL, angle);
			itemPtr->setPen(QPen(Qt::yellow));
			addToGroup(itemPtr);

			m_items.PushBack(itemPtr);
		}
	}

	update();
}


// public methods

CSearchResultShape::CModelPointShape::CModelPointShape(double x, double y, double w, double h, QGraphicsItem* parentPtr, QGraphicsScene* scenePtr, double angle)
	:BaseClass(x, y, w, h, parentPtr, scenePtr),
	m_angle(angle)
{
}


// protected methods

// reimplemented (QGraphicsRectItem)

void CSearchResultShape::CModelPointShape::paint(QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widgetPtr)
{
	BaseClass::paint(p, option, widgetPtr);
	
	double radius = rect().width() / 2.0;
	double x = pos().x();
	double y = pos().y();

	p->rotate(m_angle);

	p->drawLine(x - radius, y, x + radius, y);
	p->drawLine(x, y - radius, x, y - radius);
}


} // namespace iqtipr

