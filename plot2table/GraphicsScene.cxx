
#include <QtGui>
#include <QGraphicsView>
#include <QPixMap>

#include "GraphicsScene.h"
#include "GraphicsPointItem.h"
#include "GraphicsImageItem.h"
#include "ZoomedView.h"
#include "PointList.h"

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
  _point_color(Qt::red),
  _zoomed_view(0)
{
  point_list_ = PointList::instance();

  _image_item = new GraphicsImageItem(0,0);
  _image_item->setZValue(-100.);
  
  _point_pen.setColor(_point_color);
  _point_pen.setWidth(1);

  point_cursor_ = Qt::CrossCursor;
  _image_item->setCursor( point_cursor_ );

  QPixmap low_error_pm(":/icons/cursor_low_error.png");
  low_error_cursor_ = QCursor(low_error_pm, 16, 31);

  QPixmap high_error_pm(":/icons/cursor_high_error.png");
  high_error_cursor_ = QCursor(high_error_pm, 16, 0);
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
  //std::cout << "GraphicsScene::mouseMoveEvent()" << std::endl;
  //emit movemouse( event->scenePos() );
  if(_zoomed_view)
    _zoomed_view->centerOn( event->scenePos() );
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

  point_list_->point_clicked( (pointf/_scale_factor) );

  //_image_item->setCursor( point_cursor_ );

  std::cout << "Points: " << point_list_->size() << std::endl;
  std::cout << "SceneRect: " << this->width() << "," << this->height() << std::endl;
  return;
}

// Signaled from the PointList
void GraphicsScene::axis_point_set(int i, QPointF pointf)
{
  QPointF p = pointf*_scale_factor;
  if(i==0) new GraphicsPointItem(p, GraphicsPointItem::AxisPoint0, 0, this);
  else     new GraphicsPointItem(p, GraphicsPointItem::AxisPoint1, 0, this);
}

// Signaled from the PointList
void GraphicsScene::add_point(QPointF pointf)
{
  QPointF p = pointf*_scale_factor;
  GraphicsPointItem* gpi = new GraphicsPointItem(p, GraphicsPointItem::Point, 0, this);
  point_item_list_.append(gpi);

  if( point_list_->get_error_mode() == Asymmetric ){
    _image_item->setCursor( low_error_cursor_ );
  }
}

// Signaled from the PointList
void GraphicsScene::add_low_error(QPointF pointf)
{
  QPointF p = pointf*_scale_factor;
  GraphicsPointItem* parent = point_item_list_.last();
  new GraphicsPointItem(p, GraphicsPointItem::LowError, parent);

  if( point_list_->get_error_mode() == Asymmetric ){
    _image_item->setCursor( high_error_cursor_ );
  }
}

// Signaled from the PointList
void GraphicsScene::add_high_error(QPointF pointf)
{
  QPointF p = pointf*_scale_factor;
  GraphicsPointItem* parent = point_item_list_.last();
  new GraphicsPointItem(p, GraphicsPointItem::HighError, parent);

  if( point_list_->get_error_mode() == Asymmetric ){
    _image_item->setCursor( point_cursor_ );
  }
}


void GraphicsScene::remove_last_point() 
{
  QGraphicsItem* gi = point_item_list_.takeLast();
  point_list_->remove_last();
  delete gi;
}

void GraphicsScene::remove_point(int i)
{
  QGraphicsItem* gi = point_item_list_.takeAt(i);
  point_list_->remove(i);
  delete gi;
}

void GraphicsScene::remove_all_points()
{
  point_list_->clear();

  int N = point_item_list_.size();
  for(int i=0; i<N; i++) {
    QGraphicsItem* gi = point_item_list_.takeLast();
    delete gi;
  }
}


void GraphicsScene::point_color_changed()
{
  foreach(GraphicsPointItem* item, point_item_list_) {
    item->update();
  }
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
