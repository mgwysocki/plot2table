
#include <QPainter>
#include <iostream>

#include "GraphicsPointItem.h"

QColor GraphicsPointItem::axis_point0_color_ = QColor("blue");
QColor GraphicsPointItem::axis_point1_color_ = QColor("green");
QColor GraphicsPointItem::point_color_ = QColor("red");

GraphicsPointItem::GraphicsPointItem(QPointF &point,
				     Type t,
				     QGraphicsItem* parent, 
				     QGraphicsScene* scene) :
  QGraphicsItem(parent, scene),
  point_(point),
  type_(t)
{}

// GraphicsPointItem::~GraphicsPointItem()
// {
//   QGraphicsItem::~QGraphicsItem();
//   return;
// }

QRectF GraphicsPointItem::boundingRect() const
{
  return QRectF(point_.x()-3, point_.y()-3, 6, 6);
}

void GraphicsPointItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
			      QWidget* widget = 0)
{
  static QPen pen(Qt::SolidLine);

  if(type_==AxisPoint0) {
    pen.setColor(axis_point0_color_);
    painter->setPen(pen);
    painter->drawRect(point_.x()-2, point_.y()-2, 4, 4);

  } else if(type_==AxisPoint1) {
    pen.setColor(axis_point1_color_);
    painter->setPen(pen);
    painter->drawRect(point_.x()-2, point_.y()-2, 4, 4);

  } else if(type_==Point) {
    pen.setColor(point_color_);
    painter->setPen(pen);
    painter->drawRect(point_.x()-2, point_.y()-2, 4, 4);

  } else if(type_==HighError) {
    pen.setColor(point_color_);
    painter->setPen(pen);
    painter->drawLine(point_.x()-2, point_.y(), point_.x()+2, point_.y());
    painter->drawLine(point_.x(), point_.y(), point_.x(), point_.y()+2);

  } else if(type_==LowError) {
    pen.setColor(point_color_);
    painter->setPen(pen);
    painter->drawLine(point_.x()-2, point_.y(), point_.x()+2, point_.y());
    painter->drawLine(point_.x(), point_.y(), point_.x(), point_.y()-2);
  }
  return;
}
