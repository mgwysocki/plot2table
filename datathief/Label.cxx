
#include <QPaintEvent>
#include <QPainter>

#include "Label.h"

#include <iostream>

Label::Label(QWidget* parent):
  QLabel(parent)
{
  setAttribute(Qt::WA_NoSystemBackground, true);
}

void Label::paintEvent(QPaintEvent* event)
{
  int w( width() );
  int h( height() );

  QPainter painter(this);
  QPen pen(Qt::SolidLine);
  painter.setPen(pen);
  for(int i=0; i<w; i+=40)
    painter.drawEllipse((w-i)/2, (h-i)/2, i, i);

  painter.drawLine(0, h/2, w, h/2);
  painter.drawLine(w/2, 0, w/2, h);
  painter.end();
  return;
}
