#ifndef icmpstr_CConnectorShape_included
#define icmpstr_CConnectorShape_included


#include <QGraphicsItem>


namespace icmpstr
{


class CComponentSceneItem;
class CRegistryView;


class CConnectorShape: public QGraphicsItem
{
public:
	typedef QGraphicsItem BaseClass;

	enum ConnectFlags
	{
		CF_FACTORY = 0x0001,
		CF_EMBEDDED  =0x0002
	};

	enum GeometricParams
	{
		GP_BRIDGE_LENGTH = 20
	};

    explicit CConnectorShape(
				int connectFlags = 0,
				QGraphicsItem* parent = NULL);
    
	// reimplemented (QGraphicsItem)
    virtual int type() const;
	virtual QPainterPath shape() const;

public slots:
	void SetBeginPoint(const QPointF& position, const QPointF& direction);
	void SetEndPoint(const QPointF& position, const QPointF& direction);
    
protected:
    void Adjust();

	// reimplemented (QGraphicsItem)
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
	int m_connectFlags;

	QPointF m_beginPosition;
	QPointF m_beginDirection;
	QPointF m_endPosition;
	QPointF m_endDirection;

	QPolygonF m_connectionLine;
};


} // namespace icmpstr


#endif // !icmpstr_CConnectorShape_included


