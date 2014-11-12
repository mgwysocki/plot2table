#include "LabelButton.h"
#include <QMouseEvent>

LabelButton::LabelButton(QWidget* parent, Qt::WindowFlags f) :
  QLabel::QLabel(parent, f),
  clicked_(false)
{}

LabelButton::LabelButton(const QString &text, QWidget* parent, Qt::WindowFlags f) :
  QLabel::QLabel(text, parent, f),
  clicked_(false)
{}


void LabelButton::mousePressEvent( QMouseEvent* event )
{
  if( rect().contains(event->pos()) )
    clicked_ = true;
  return;
}

void LabelButton::mouseReleaseEvent( QMouseEvent* event )
{
  if( rect().contains(event->pos()) ){
    clicked_ = false;
    emit clicked();
  }
  return;
}
