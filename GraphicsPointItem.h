#ifndef __MGRAPHICSPOINTITEM_H
#define __MGRAPHICSPOINTITEM_H

#include <QGraphicsItem>
#include <QPainter>
class QPointF;

class GraphicsPointItem : public QGraphicsItem
{

 public:
  enum Type {AxisPoint0, AxisPoint1, Point, HighError, LowError};

  GraphicsPointItem(QPointF &point,
                    Type t = Point,
                    QGraphicsItem* parent = 0);
  
  QRectF boundingRect() const;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
  
 protected:
  QPointF point_;
  Type type_;
  QPen pen_;
};

#endif
