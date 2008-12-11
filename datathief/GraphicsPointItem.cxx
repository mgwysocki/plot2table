
#include <QPainter>
#include <iostream>

#include "PointList.h"
#include "GraphicsPointItem.h"

GraphicsPointItem::GraphicsPointItem(QPointF &point,
				     Type t,
				     QGraphicsItem* parent, 
				     QGraphicsScene* scene) :
  QGraphicsItem(parent, scene),
  point_(point),
  type_(t)
{}


QRectF GraphicsPointItem::boundingRect() const
{
  return QRectF(point_.x()-3, point_.y()-3, 6, 6);
}

void GraphicsPointItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
			      QWidget* widget = 0)
{
  static QPen pen(Qt::SolidLine);
  PointList* pl = PointList::instance();

  if(type_==AxisPoint0) {
    pen.setColor(pl->get_axis_point0_color());
    painter->setPen(pen);
    painter->drawRect(point_.x()-2, point_.y()-2, 4, 4);

  } else if(type_==AxisPoint1) {
    pen.setColor(pl->get_axis_point1_color());
    painter->setPen(pen);
    painter->drawRect(point_.x()-2, point_.y()-2, 4, 4);

  } else if(type_==Point) {
    pen.setColor(pl->get_point_color());
    painter->setPen(pen);
    painter->drawRect(point_.x()-2, point_.y()-2, 4, 4);

  } else if(type_==HighError) {
    pen.setColor(pl->get_point_color());
    painter->setPen(pen);
    painter->drawLine(point_.x()-2, point_.y(), point_.x()+2, point_.y());
    painter->drawLine(point_.x(), point_.y(), point_.x(), point_.y()+2);

  } else if(type_==LowError) {
    pen.setColor(pl->get_point_color());
    painter->setPen(pen);
    painter->drawLine(point_.x()-2, point_.y(), point_.x()+2, point_.y());
    painter->drawLine(point_.x(), point_.y(), point_.x(), point_.y()-2);
  }
  return;
}
