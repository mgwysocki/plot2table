
#include <QtGui>

#include "GraphicsScene.h"
#include "GraphicsPointItem.h"
#include "GraphicsImageItem.h"

#include <QGraphicsView>

#include <cmath>
#include <iostream>
using namespace std;

GraphicsScene::GraphicsScene(QObject* parent) :
  QGraphicsScene(parent),
  _point_pen(Qt::SolidLine),
  _point0_set(false),
  _point1_set(false),
  _point0_color(Qt::blue),
  _point1_color(Qt::green),
  _point_color(Qt::red)
{
  _image_item = new GraphicsImageItem(0,0);
  _image_item->setZValue(-100.);
  
  _point_pen.setColor(_point_color);
  _point_pen.setWidth(1);
}


void GraphicsScene::set_image_pixmap(const QPixmap &image_pixmap)
{
  _image_item->setPixmap(image_pixmap);
  this->addItem(_image_item);  
  this->setSceneRect( QRectF(QPointF(0,0), image_pixmap.size()) );

  QGraphicsView* view = views().at(0);
  if(image_pixmap.width() > view->width() ||
     image_pixmap.height() > view->height() )
    emit image_doesnt_fit(image_pixmap.size());

  _scale_factor = 1.0;
  return;
}


void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  std::cout << "GraphicsScene::mouseMoveEvent()" << std::endl;
  emit movemouse( event->scenePos() );
  QGraphicsScene::mouseMoveEvent(event);
  return;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  _press_pos = event->screenPos();
  QGraphicsScene::mousePressEvent(event);
  return;
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  if( _press_pos == event->screenPos() ){
    std::cout << "Point clicked: " << event->screenPos().x() << "," << event->screenPos().y() << std::endl;
    //emit point_clicked( event->pos() );
    image_click_event( event->scenePos() );
  }
  return;
}


void GraphicsScene::image_click_event(QPointF point) 
{
  std::cout << "GraphicsScene::image_click_event( QPoint point(" 
	    << point.x() << ", " << point.y() << ") )" << std::endl;
  
  //QPointF pointf(mapToScene(point));
  QPointF pointf(point);
  if( !this->sceneRect().contains(pointf) ) return;

  if( !_point0_set ) {
    _point0 = pointf / _scale_factor;
    _point0_set = true;
    emit point0_set();
    
  } else if( !_point1_set ) {
    _point1 = pointf / _scale_factor;
    _point1_set = true;
    emit point1_set();

  } else {
    _click_points.append( (pointf/_scale_factor) );
    this->addItem( new GraphicsPointItem(pointf) );
//     QGraphicsRectItem* item = this->addRect(QRectF(pointf.x(), pointf.y(), 4, 4), 
// 					      _point_pen);
    std::cout << "GraphicsScene: added rect (" << pointf.x() << "," << pointf.y() << ")" << std::endl;
    emit data_point_added();
    std::cout << "Points: " << this->items().size() << std::endl;
    std::cout << "SceneRect: " << this->width() << "," << this->height() << std::endl;

  }
  return;
}

void GraphicsScene::remove_last_point()
{
  _click_points.removeLast();
  return;
}

void GraphicsScene::remove_all_points()
{
  _click_points.clear();
  return;
}

void GraphicsScene::save_points(double x0, double y0, double x1, double y1, bool logx, bool logy)
{
#ifdef DEBUG
  QString filename("points.out");
#else
  QString filename = QFileDialog::getOpenFileName(this,
						  tr("Open File"), 
						  QDir::currentPath());
#endif
  QFile outfile(filename);
  if( !outfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    return;
  QTextStream out(&outfile);

  if( error_mode == Symmetric ){
    QPointF point;
    for(int i=0; i<_click_points.size(); ++i) {
      point = _click_points[i];
      point = _convert_point(point, x0, y0, x1, y1, logx, logy);
      out << point.x() << " " << point.y();
      float y = point.y();

      point = _click_points[++i];
      point = _convert_point(point, x0, y0, x1, y1, logx, logy);
      out << " 0.0 " << fabs(point.y() - y) << endl;
    }
    
  } else if( error_mode == Asymmetric ) {
    QPointF point;
    for(int i=0; i<_click_points.size(); ++i) {
      point = _click_points[i];
      point = _convert_point(point, x0, y0, x1, y1, logx, logy);
      out << point.x() << " " << point.y();
      float y = point.y();

      point = _click_points[++i];
      point = _convert_point(point, x0, y0, x1, y1, logx, logy);
      out << " 0.0 0.0 " << fabs(point.y() - y) << endl;

      point = _click_points[++i];
      point = _convert_point(point, x0, y0, x1, y1, logx, logy);
      out << " " << fabs(point.y() - y) << endl;
    }

  } else {
    QPointF point;
    foreach(point, _click_points) {
      point = _convert_point(point, x0, y0, x1, y1, logx, logy);
      out << point.x() << " " << point.y() << endl;
    }
  }
  outfile.close();
  return;
}

// void GraphicsScene::_construct_image()
// {
//   QPixmap pixmap(QPixmap::fromImage(_image));
//   QPainter painter(&pixmap);
//   painter.drawImage(_image.rect(), _image, _image.rect());
//   QPen pen(point0_color);
//   pen.setWidth(2);
//   if(_point0_set) {
//     painter.setPen(pen);
//     int x = (int) _point0.x();
//     int y = (int) _point0.y();
//     painter.drawLine(x-8, y, x+8, y);
//     painter.drawLine(x, y-8, x, y+8);
//   }

//   if(_point1_set) {
//     pen.setColor(point1_color);
//     painter.setPen(pen);
//     int x = (int) _point1.x();
//     int y = (int) _point1.y();
//     painter.drawLine(x-8, y, x+8, y);
//     painter.drawLine(x, y-8, x, y+8);
//   }

//   QPointF point;
//   foreach(point, _click_points) {
//     pen.setColor(_point_color);
//     pen.setWidth(1);
//     painter.setPen(pen);
//     int x = (int) point.x();
//     int y = (int) point.y();
// //     painter.drawLine(x-8, y, x+8, y);
// //     painter.drawLine(x, y-8, x, y+8);
//     painter.drawRect(x-1, y-1, 2, 2);
//   }

//   painter.end();
//   _image_label->setPixmap(pixmap);
  
//   return;
// }

QPointF GraphicsScene::_convert_point(QPointF point, 
				       const double &graph_x0, const double &graph_y0, 
				       const double &graph_x1, const double &graph_y1,
				       const bool &logx, const bool &logy)
{
  double x0 = _point0.x();
  double y0 = _point0.y();
  double x1 = _point1.x();
  double y1 = _point1.y();

  double delta_x_coord = (graph_x1-graph_x0);
  double delta_x_screen = (x1-x0);
  double delta_y_coord = (graph_y1-graph_y0);
  double delta_y_screen = (y1-y0);

  QPointF pointf;
  if(logx && logy)
    pointf = QPointF(pow(10, (log10(graph_x1)-log10(graph_x0))*(point.x()-x0)/delta_x_screen + log10(graph_x0)),
 		     pow(10, (log10(graph_y1)-log10(graph_y0))*(point.y()-y0)/delta_y_screen + log10(graph_y0)));
  else if(logx)
    pointf = QPointF(pow(10, (log10(graph_x1)-log10(graph_x0))*(point.x()-x0)/delta_x_screen + log10(graph_x0)), 
		     (delta_y_coord/delta_y_screen) * (point.y()-y0) + graph_y0);
  else if(logy)
    pointf = QPointF((delta_x_coord/delta_x_screen) * (point.x()-x0) + graph_x0, 
		     pow(10, (log10(graph_y1)-log10(graph_y0))*(point.y()-y0)/delta_y_screen + log10(graph_y0)));
  else
    pointf = QPointF((delta_x_coord/delta_x_screen) * (point.x()-x0) + graph_x0, 
		     (delta_y_coord/delta_y_screen) * (point.y()-y0) + graph_y0);
  return pointf;
}

void GraphicsScene::change_point_color(const QColor &c)
{
  _point_color = c;
  //_construct_image();
  return;
}


void GraphicsScene::drawItems(QPainter *painter, int numItems,
			       QGraphicsItem *items[],
			       const QStyleOptionGraphicsItem options[],
			       QWidget *widget)
 {
   for (int i=0; i<numItems; ++i) {
     // Draw the item
     painter->save();
     painter->setWorldTransform(items[i]->sceneTransform(), true);
     items[i]->paint(painter, &options[i], widget);
     painter->restore();
   }
   return;
 }
