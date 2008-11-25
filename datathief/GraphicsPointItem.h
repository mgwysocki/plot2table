#ifndef __MGRAPHICSPOINTITEM_H
#define __MGRAPHICSPOINTITEM_H

#include <QGraphicsItem>
class QPointF;

#include <iostream>

class GraphicsPointItem : public QGraphicsItem
{

 public:
  enum Type {AxisPoint0, AxisPoint1, Point, HighError, LowError};

  GraphicsPointItem(QPointF &point,
		    Type t = Point,
		    QGraphicsItem* parent = 0, 
		    QGraphicsScene* scene = 0);

  //virtual ~GraphicsPointItem();

  static void set_axis_point0_color(QColor c) { axis_point0_color_ = c; }
  static void set_axis_point1_color(QColor c) { axis_point1_color_ = c; }
  static void set_point_color(QColor c) { point_color_ = c; }

  QRectF boundingRect() const;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
	     QWidget* widget);

 protected:
  QPointF point_;
  Type type_;

  static QColor axis_point0_color_;
  static QColor axis_point1_color_;
  static QColor point_color_;
};

#endif
