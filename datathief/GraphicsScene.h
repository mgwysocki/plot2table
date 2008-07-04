#ifndef __MGRAPHICSSCENE_H
#define __MGRAPHICSSCENE_H

class QGraphicsSceneMouseEvent;
class QPointF;
class QString;
class GraphicsImageItem;

#include <QGraphicsScene>
#include "ErrorModes.h"

class GraphicsScene : public QGraphicsScene
{
 Q_OBJECT

 public:
  GraphicsScene(QObject* parent=0);

  void set_image_pixmap(const QPixmap &);
  void adjust_image_size();
  void scale_image(double factor);
  void set_scale_factor(double x) {_scale_factor=x;}

  ErrorMode error_mode;

 public slots:
  void image_click_event(QPointF point);
  void remove_all_points();
  void remove_last_point();
  void save_points(double x0, double y0, double x1, double y1, bool logx, bool logy);
  void change_point_color(const QColor&);

 signals:
  void point0_set();
  void point1_set();
  void data_point_added();
  void image_doesnt_fit(QSize);
  void movemouse(QPointF);

 protected:
  void drawItems(QPainter* painter, int numItems, QGraphicsItem* items[], 
		 const QStyleOptionGraphicsItem options[], QWidget* widget);
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

  GraphicsImageItem* _image_item;
  double _scale_factor;

  QPoint _press_pos;
  QPen _point_pen;

  bool _point0_set;
  bool _point1_set;
  QPointF _point0;
  QPointF _point1;

  QList<QPointF> _click_points;
  QList<QPointF> _data_points;

  QColor _point0_color;
  QColor _point1_color;
  QColor _point_color;

  QPointF _convert_point(QPointF point, const double &, const double &, 
			 const double &, const double &, 
			 const bool &, const bool &);

};

#endif
