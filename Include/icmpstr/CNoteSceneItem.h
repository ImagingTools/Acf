#ifndef icmpstr_CNoteSceneItem_included
#define icmpstr_CNoteSceneItem_included


#include <QGraphicsItemGroup>


namespace icmpstr
{


class CComponentSceneItem;


class CNoteSceneItem: public QGraphicsItemGroup
{
public:
	typedef QGraphicsItemGroup BaseClass;

    CNoteSceneItem(
				CComponentSceneItem *parentComponent, 
				QGraphicsItem *parent = NULL, 
				QGraphicsScene *scene = NULL);
    
	virtual ~CNoteSceneItem();

    CComponentSceneItem* GetParentComponent() const;
    void SetParentComponent(CComponentSceneItem* parentComponentPtr);

    void Adjust();

	// reimplemented (QGraphicsItem)
    virtual int type() const;
	virtual QPainterPath shape() const;
    
protected:
	// reimplemented (QGraphicsItem)
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    CComponentSceneItem* m_parentComponent;

	QPolygonF m_connectionLine;
	QPointF m_touchPoint;
};


} // namespace icmpstr


#endif // !icmpstr_CNoteSceneItem_included

