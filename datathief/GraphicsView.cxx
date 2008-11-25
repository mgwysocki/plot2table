
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#include "GraphicsView.h"

#include <iostream>

GraphicsView::GraphicsView(QWidget* parent):
  QGraphicsView(parent)
  //  _overlay((QWidget*) this)
{
  //  _overlay.setGeometry(0, 0, width(), height());
}

// void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
// {
//   emit point_clicked( event->pos() );
//   return;
// }

// void GraphicsView::resizeEvent(QResizeEvent* e)
// {
//   QGraphicsView::resizeEvent(e);
//   _overlay.resize(this->size());
//   printf("ZoomedView::resizeEvent():  view %dx%d   overlay %dx%d\n", 
// 	 width(), height(), _overlay.width(), _overlay.height());
//   return;
// }


MCursorOverlay::MCursorOverlay(QWidget* parent) :
  QLabel::QLabel(parent),
  cursor_in(false)
{
  this->setMouseTracking(true);
  this->setUpdatesEnabled(true);
}

void MCursorOverlay::mouseMoveEvent(QMouseEvent* event)
{
  if( event->buttons() == Qt::NoButton ){
    std::cout << "GraphicsView::mouseMoveEvent(event)" << std::endl;
    cursor_position = event->pos();
    //event->ignore();
    printf("(%d, %d)\n", cursor_position.x(), cursor_position.y());
    emit movescene( cursor_position );
    //repaint();
    //paintEvent( new QPaintEvent(rect()) );
  }
  update();
  return;
}

void MCursorOverlay::enterEvent(QEvent*)
{
  cursor_in = true;
  return;
}

void MCursorOverlay::leaveEvent(QEvent*)
{
  cursor_in = false;
  update();
  return;
}

void MCursorOverlay::paintEvent(QPaintEvent* event)
{
  QLabel::paintEvent(event);
  std::cout << "MCursorOverlay::paintEvent(event)" << std::endl;
  if(cursor_in) {
    QPainter painter(this);
    QPen pen(Qt::DotLine);
    painter.setPen(pen);
    //painter.setBrush(this->foregroundBrush());

    painter.drawLine(0, cursor_position.y(), width(), cursor_position.y());
    painter.drawLine(cursor_position.x(), 0, cursor_position.x(), height());
    painter.end();
  }
  return;
}

