
#include "GraphicsPointItem.h"
#include "PointList.h"

#include <iostream>

GraphicsPointItem::GraphicsPointItem(QPointF &point,
                                     Type t,
                                     QGraphicsItem* parent) :
  QGraphicsItem(parent),
  point_(point),
  type_(t),
  pen_(Qt::SolidLine)
{
  PointList* pl = PointList::instance();
  if(type_==AxisPoint0) {
    pen_.setColor(pl->get_axis_point0_color());
  } else if(type_==AxisPoint1) {
    pen_.setColor(pl->get_axis_point1_color());
  } else if(type_==Point) {
    pen_.setColor(pl->get_point_color());
  }
}

QRectF GraphicsPointItem::boundingRect() const
{
  return QRectF(point_.x()-3, point_.y()-3, 6, 6);
}

void GraphicsPointItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* , QWidget*)
{
  painter->setPen(pen_);

  QRectF point_rect(0, 0, 4, 4);
  point_rect.moveCenter(point_);
  painter->drawRect(point_rect);

  if(type_==HighError) {
    painter->drawLine(point_.x()-2, point_.y(), point_.x()+2, point_.y());
    painter->drawLine(point_.x(), point_.y(), point_.x(), point_.y()+2);

  } else if(type_==LowError) {
    painter->drawLine(point_.x()-2, point_.y(), point_.x()+2, point_.y());
    painter->drawLine(point_.x(), point_.y(), point_.x(), point_.y()-2);
  }
  return;
}
