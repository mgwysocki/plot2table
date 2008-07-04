#ifndef __MGRAPHICSPOINTITEM_H
#define __MGRAPHICSPOINTITEM_H

#include <QGraphicsItem>
class QPointF;

class GraphicsPointItem : public QGraphicsItem
{

 public:
  GraphicsPointItem(QPointF &point,
		    QGraphicsItem* parent = 0, 
		    QGraphicsScene* scene = 0);

  QRectF boundingRect() const;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
	     QWidget* widget);

 protected:
  QPointF _point;
};

#endif
