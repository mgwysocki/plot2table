#include <QPaintEvent>
#include <QPainter>

#include "ZoomedView.h"

#include <iostream>
using namespace std;

ZoomedView::ZoomedView(QWidget* parent):
  QGraphicsView(parent)
{
  this->setUpdatesEnabled(true);
  _overlay = new MZoomedOverlay(this);
  //_overlay->raise();
  _overlay->setGeometry(0, 0, width(), height());
}

void ZoomedView::resizeEvent(QResizeEvent* e)
{
  QGraphicsView::resizeEvent(e);
  _overlay->resize(this->size());
//   printf("ZoomedView::resizeEvent():  view %dx%d   overlay %dx%d\n", 
// 	 width(), height(), _overlay->width(), _overlay->height());
  return;
}


MZoomedOverlay::MZoomedOverlay(QWidget* parent) :
  QLabel::QLabel(parent)
{
  this->setAttribute(Qt::WA_NoSystemBackground, true);
  this->setUpdatesEnabled(true);
}

void MZoomedOverlay::paintEvent(QPaintEvent* e)
{
  QLabel::paintEvent(e);

  int w( width() );
  int h( height() );

  QPainter painter(this);
  QPen pen(Qt::SolidLine);
  painter.setPen(pen);
  for(int i=0; i<max(w,h); i+=40)
    painter.drawEllipse((w-i)/2, (h-i)/2, i, i);

  painter.drawLine(0, h/2, w, h/2);
  painter.drawLine(w/2, 0, w/2, h);
  painter.end();
  return;
}
