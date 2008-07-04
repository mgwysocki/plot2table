
#include <QPainter>
#include <iostream>

#include "GraphicsPointItem.h"

GraphicsPointItem::GraphicsPointItem(QPointF &point, QGraphicsItem* parent, 
				     QGraphicsScene* scene):
  QGraphicsItem(parent, scene),
  _point(point)
{}

QRectF GraphicsPointItem::boundingRect() const
{
  return QRectF(_point.x()-3, _point.y()-3, 6, 6);
}

void GraphicsPointItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
			       QWidget* widget = 0)
{
  static QPen pen(Qt::SolidLine);
  painter->setPen(pen);
  painter->drawRect(_point.x()-2, _point.y()-2, 4, 4);

  return;
}
