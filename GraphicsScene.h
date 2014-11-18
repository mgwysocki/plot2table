#ifndef __MGRAPHICSSCENE_H
#define __MGRAPHICSSCENE_H

class QGraphicsSceneMouseEvent;
class QPointF;
class QString;
class GraphicsImageItem;
class GraphicsPointItem;
class ZoomedView;
class PointList;

#include <QCursor>
#include <QGraphicsScene>
#include "ErrorModes.h"

class GraphicsScene : public QGraphicsScene
{
 Q_OBJECT

 public:
  GraphicsScene(QObject* parent=0);
  void set_zoomed_view(ZoomedView* zv) {_zoomed_view = zv;}

  void set_image_pixmap(const QPixmap &);
  void adjust_image_size();
  void scale_image(double factor);
  void set_scale_factor(double x) {_scale_factor=x;}
  QRectF get_image_rect() const;

 public slots:
  void image_click_event(QPointF point);
  void point_color_changed();
  void axis_point_set(int, QPointF);
  void add_point(QPointF);
  void add_low_error(QPointF);
  void add_high_error(QPointF);
  void remove_all_points();
  void remove_last_point();
  void remove_point(int);

 signals:
  void image_doesnt_fit(QSize);
  void movemouse(QPointF);

 protected:
  void drawItems(QPainter* painter, int numItems, QGraphicsItem* items[], 
		 const QStyleOptionGraphicsItem options[], QWidget* widget);
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

  GraphicsImageItem* _image_item;
  QList<GraphicsPointItem*> point_item_list_;
/*   QGraphicsItemGroup* low_error_items_; */
/*   QGraphicsItemGroup* high_error_items_; */
  double _scale_factor;

  QCursor point_cursor_;
  QCursor low_error_cursor_;
  QCursor high_error_cursor_;

  QPoint _press_pos;
  QPen _point_pen;

  bool _point0_set;
  bool _point1_set;
  QPointF _point0;
  QPointF _point1;

  PointList* point_list_;

  QColor _point0_color;
  QColor _point1_color;
  QColor _point_color;

  ZoomedView* _zoomed_view;
};

#endif
